#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "TouchDriver.h"

constexpr auto ledPin = 2;
constexpr auto ledCount = 24;
Adafruit_NeoPixel ring{ledCount, ledPin};

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

TouchDriver touchDriver;

void setup() {
	touchDriver.Setup();
	touchDriver.OnTouched([](){
		const auto teal = ring.Color(0, 0x80, 0x80);
		showUniformColor(teal);
	});
	touchDriver.OnReleased([](){
		turnLampOff();
	});
	ring.begin();
	turnLampOff();
}

void loop() {
	touchDriver.Loop();
	constexpr auto loopIntervalMs = 20;
	delay(loopIntervalMs);
}
