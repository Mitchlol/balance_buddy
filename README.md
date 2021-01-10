# Balance Buddy (More detailed guide planned)
Arduino based accessory board for the balance app in BV's BLDC firmware.

## Features
1. It uses CAN bus, so who even cares what it does, there are NO DOWNSIDES to adding it. JUST DO IT!
1. Directional LEDs for headlight/taillight.
1. Buzzer for over speed, low voltage, and high speed half switch warnings.
1. A teeny tiny small display thats so small you cant even see it with data you probbaly wont be able to read (it's smol).

## Parts List
1. Adruino Nano
1. MCP2515 Can Bus Module Board
1. 5v piezo buzzer
1. 2x LED Strips (WS2811 GRB is the default, but anything supported by FastLED should work)
1. SSD1306 128x64 Display

## Wiring
1. Can Module: see https://github.com/autowp/arduino-mcp2515
1. Piezo: D4 (if you use a fancy buzzer/speaker make sure it wont blow the nano)
1. LEDs: D5 & D6
1. SSD1306: Wire to i2c

## Configuration
Features are designed to be configured VIA the conatants in the 3 feature files balance_beeper.cpp/balance_leds.cpp/balance_display.cpp

## Building
Youll need to install these libraies:
https://github.com/FastLED/FastLED
https://github.com/greiman/SSD1306Ascii
