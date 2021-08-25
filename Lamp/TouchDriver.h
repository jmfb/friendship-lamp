#pragma once
#include "Arduino.h"

using TouchEvent = void (*)();

constexpr auto stableScanCount = 3;
constexpr auto touchPin = 15;

class TouchDriver {
	bool isTouched = false;
	bool isTouchedScan[stableScanCount];
	int nextIndex = 0;
	TouchEvent handleTouched = nullptr;
	TouchEvent handleReleased = nullptr;

public:
	TouchDriver() {
		for (auto& scan : isTouchedScan) {
			scan = false;
		}
	}

	void Setup() {
		pinMode(touchPin, INPUT_PULLUP);
	}

	void Loop() {
		Scan();
		if (IsChangingTouchedState()) {
			FireEvent();
		}
	}

	void OnTouched(TouchEvent handler) {
		handleTouched = handler;
	}

	void OnReleased(TouchEvent handler) {
		handleReleased = handler;
	}

private:
	void Scan() {
		constexpr auto touchThreshold = 15;
		const auto currentTouch = touchRead(touchPin);
		isTouchedScan[nextIndex] = currentTouch < touchThreshold;
		nextIndex = (nextIndex + 1) % stableScanCount;
	}

	bool IsChangingTouchedState() {
		const auto wasTouched = isTouched;
		isTouched = GetStableScan();
		return wasTouched != isTouched;
	}

	bool GetStableScan() const {
		for (auto scan : isTouchedScan) {
			if (!scan) {
				return false;
			}
		}
		return true;
	}

	void FireEvent() const {
		if (isTouched) {
			if (handleTouched) {
				handleTouched();
			}
		} else {
			if (handleReleased) {
				handleReleased();
			}
		}
	}
};
