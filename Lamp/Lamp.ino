#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

using TouchEvent = void (*)();

constexpr auto stableScanCount = 10;
constexpr auto touchPin = 15;
class TouchDriver {
	bool isTouched = false;
	bool isTouchedScan[stableScanCount];
	int nextIndex = 0;
	TouchEvent handleTouched = nullptr;
	TouchEvent handleReleased = nullptr;

public:
	TouchDriver() {
		for (auto index = 0; index < stableScanCount; ++index) {
			isTouchedScan[index] = false;
		}
	}

	void Setup() {
		pinMode(touchPin, INPUT_PULLUP);
	}

	void Loop() {
		Scan();
		const auto wasTouched = isTouched;
		isTouched = GetStableScan();
		const auto isChangingTouchedState = wasTouched != isTouched;
		if (isChangingTouchedState) {
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
		constexpr auto touchMin = 15;
		const auto currentTouch = touchRead(touchPin);
		isTouchedScan[nextIndex] = currentTouch < touchMin;
		nextIndex = (nextIndex + 1) % stableScanCount;
	}

	bool GetStableScan() const {
		auto count = 0;
		for (auto index = 0; index < stableScanCount; ++index) {
			if (isTouchedScan[index]) {
				++count;
			}
		}
		return count > 5;
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

constexpr auto ledPin = 2;
constexpr auto ledCount = 24;
Adafruit_NeoPixel ring{ledCount, ledPin/*, NEO_GRB + NEO_KHZ800*/};

void showUniformColor(uint32_t color) {
	for (auto index = 0; index < ledCount; ++index) {
		ring.setPixelColor(index, color);
	}
	ring.show();
}

void turnLampOff() {
	ring.clear();
	ring.show();
}

void toggleLamp() {
	static auto isOn = false;
	if (isOn) {
		isOn = false;
		turnLampOff();
	} else {
		isOn = true;
		const auto teal = ring.Color(0, 0x80, 0x80);
		showUniformColor(teal);
	}
}

TouchDriver touchDriver;

void setup() {
	touchDriver.Setup();
	touchDriver.OnTouched(toggleLamp);
	ring.begin();
	turnLampOff();
}

void loop() {
	touchDriver.Loop();
	constexpr auto loopIntervalMs = 20;
	delay(loopIntervalMs);
}
