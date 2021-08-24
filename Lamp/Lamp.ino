#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

constexpr auto ledPin = 2;
constexpr auto ledCount = 24;
Adafruit_NeoPixel ring(ledCount, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.clear();
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

void loop() {
  constexpr auto touchPin = 15;
  constexpr auto touchMin = 15;
  const auto isTouched = touchRead(touchPin) < touchMin;
  if (isTouched) {
    const auto wait = 20;
    rainbow(wait);
  }
}
