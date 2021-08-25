#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

constexpr auto ledPin = 2;
constexpr auto ledCount = 24;
Adafruit_NeoPixel ring(ledCount, ledPin, NEO_GRB + NEO_KHZ800);

constexpr auto stableScanCount = 10;
class TouchState {
	bool isTouched = false;
	bool isTouchedScan[stableScanCount];
	int nextIndex = 0;

public:
	TouchState() {
		for (auto index = 0; index < stableScanCount; ++index) {
			isTouchedScan[index] = false;
		}
	}

	void Scan() {
		constexpr auto touchPin = 15;
		constexpr auto touchMin = 15;
		const auto currentTouch = touchRead(touchPin);
		isTouchedScan[nextIndex] = currentTouch < touchMin;
		nextIndex = (nextIndex + 1) % stableScanCount;
		isTouched = GetStableScan();
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

	bool IsTouched() const {
		return isTouched;
	}
};

void setup() {
	ring.begin();
	ring.clear();
	ring.show();
}

void test() {
	for (auto index = 0; index < ledCount; ++index) {
		ring.setPixelColor(index, ring.Color(0, 0x80, 0x80));
	}
	ring.show();
}

void rainbowCycle(int wait, uint32_t& pixelHueOffset, uint8_t brightness) {
	constexpr auto maxHue = 65536;
	constexpr auto maxSaturation = 255;
	for (auto index = 0; index < ledCount; ++index) {
		auto pixelHueValue = pixelHueOffset + (index * maxHue / ledCount);
		ring.setPixelColor(index, ring.gamma32(ring.ColorHSV(pixelHueValue, maxSaturation, brightness)));
	}
	ring.show();
	delay(wait);
	constexpr auto hueStep = 255;
	pixelHueOffset += hueStep;
}

void rainbow(int wait) {
	uint32_t pixelHueOffset = 0;
	constexpr auto maxBrightness = 255;
	auto brightness = 0;
	while (++brightness < maxBrightness) {
		rainbowCycle(wait, pixelHueOffset, brightness);
	}
	auto start = millis();
	const auto minRainbowMs = 10000;
	while (millis() - start < minRainbowMs) {
		rainbowCycle(wait, pixelHueOffset, brightness);
	}
	while (--brightness > 0) {
		rainbowCycle(wait, pixelHueOffset, brightness);
	}
	ring.clear();
	ring.show();
}

constexpr auto loopIntervalMs = 20;

void loop() {
	static auto isOn = false;
	static TouchState touchState;
	static bool wasTouched = false;
	touchState.Scan();
	if (wasTouched != touchState.IsTouched()) {
		wasTouched = touchState.IsTouched();
		if (isOn) {
			isOn = false;
			ring.clear();
			ring.show();
		} else {
			isOn = true;
			test();
		}
	}
	delay(loopIntervalMs);
}
