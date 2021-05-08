# Balance Buddy
Arduino based accessory board for the balance app in BV's BLDC firmware.

## Features
1. Directional LEDs for headlight/taillight with dimming.
1. Buzzer for over speed, low voltage, and high speed half switch warnings.
1. A teeny tiny small display thats so small you cant even see it with data you probbaly wont be able to read (it's smol).
1. It uses CAN bus, so there are NO DOWNSIDES to adding it. JUST DO IT!

## Parts List
1. Adruino Nano
1. MCP2515 Can Bus Module Board
1. 5v piezo active buzzer
1. 2x LED Strips (WS2812 GRB is the default, but anything supported by FastLED should work)
1. SSD1306 128x64 Display (4 pin, any color, the blue/yellow works nice too)

# Wiring
## Can Module: 
Be sure you short the can termination pins on the module if needed (if your can setup is just an esc and this, you need it)  
![](./wiki/images/can_bus_wiring.png)
For more info see https://github.com/autowp/arduino-mcp2515
### 2. Piezo:
**if you use a fancy buzzer/speaker make sure it wont blow the nano**
![](./wiki/images/piezo_wiring.png)
### 3. LEDs:
**if you wire lots of LEDs make sure it wont blow the nano**
![](./wiki/images/led_wiring.png)
### 4. SSD1306:
![](./wiki/images/display_wiring.png)

# Configuration
## Options and pins
Features are designed to be configured VIA the constants
1. esc.cpp: Configure CAN bus IDs, you must match the ID set in the VESC Tool
1. balance_beeper.cpp: Configure wiring and alerts
1. balance_leds.cpp: Configure wiritng, LED Type, Brightnes, etc.
1. balance_display.cpp: Nothing configureable yet

## Compiling/Installing
All the required libraries are included, just hit the upload button in Arduino IDE

# Why does my balance buddy keep crashing?!!!
If youre using a display, it is likely related to [bus capacitence](https://hackaday.com/2017/02/08/taking-the-leap-off-board-an-introduction-to-i2c-over-long-wires/) and [wire.h hangs](https://github.com/OpenAgricultureFoundation/openag_brain/issues/236)
In my case using minimal length wires, and a 2.2k pull up from scl to 3.3v solved my issues.
