# Friendship Lamp

## Local Setup

Install the Arduino IDE via Chocolatey:

```
choco install arduino -y
```

Go to `File -> Preferences` and set `Additional Board Manager URLs:` to:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Go to `Tools -> Board... -> Boards Manager`, search for `esp32` and click Install.
This will install the `ESP32 Dev Module` arduino board that we will be using.

Go to `Tools -> Manage Libraries`, search for `Adafruit NeoPixel` and click Install.
This will install the `"Adafruit_NeoPixel.h"` library to drive the pixel ring hardware.

[Adafruit NeoPixel Library Reference](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use)

Plug in your lamp to a USB port on your computer.

Go to `Tools -> Board... -> ESP32 Arduino -> ESP32 Dev Module` to choose the correct board.

Go to `Tools -> Port...` and choose the correct COM port for your lamp (e.g. COM3).
If you have multiple to choose from, try unplugging and and plugging back in your lamp to see which option goes away and comes back.
