ss_oled (Small Simple OLED library)<br>
-----------------------------------
Project started 1/15/2017<br>
Copyright (c) 2017 BitBank Software, Inc.<br>
Written by Larry Bank<br>
bitbank@pobox.com<br>
<br>
![ss_oled](/demo.jpg?raw=true "ss_oled")
<br>
The purpose of this code is to easily control SH1106/SSD1306 OLED
displays using a minimum of FLASH and RAM. The displays can be connected to
the traditional I2C bus or you can use any 2 GPIO pins to define a virtual
I2C bus. On AVR microcontrollers, there is an optimized option to speed up
access to the GPIO pins to allow speeds which match or exceed normal I2C
speeds. The pins are numbered with the Port letter as the first digit followed
by the bit number. For example, To use bit 0 of Port B, you would reference
pin number 0xb0. Includes the unique feature that the init function can optionally detect the display address (0x3C or 0x3D) and the controller type (SSD1306, SH1106 or
SH1107).<br>
I try to support as many OLEDs as I can. I was able to justify buying a bunch
of different sized SSD1306 displays because they're around $2 each. A generous patron
donated money so that I could purchase Pimoroni's 128x128 OLED and add support for it.
It uses the SH1107 controller and behaves very similarly to the SH1106.
<br>

Features:<br>
---------<br>
- Supports any number of simultaneous displays of any type (mix and match)<br>
- Optionally detect the display address and type (I2C only)<br>
- Supports 72x40, 96x16, 64x32, 128x32, 128x64, 128x128 (SH1107) and 132x64 (SH1106) display sizes<br>
- Drive displays from I2C, SPI or any 2 GPIO pins (virtual I2C)<br>
- 5 sizes of fixed fonts (6x8, 8x8, 12x16, 16x16, 16x32)<br>
- Deferred rendering allows preparing a back buffer, then displaying it (usually faster)<br>
- Text scrolling features (vertical and horizontal)<br>
- Text cursor position with optional line wrap<br>
- a function to load a Windows BMP file<br>
- Pixel drawing on SH1106/7 without needing backing RAM<br>
- Optimized Bresenham line drawing<br>
- Optimized Bresenham outline and filled ellipse drawing<br>
- Optimized outline and filled rectangle drawing<br>
- Optional backing RAM for drawing pixels for systems with enough RAM<br>
- 16x16 Tile/Sprite drawing at any angle.<br>
- Run full frame animations at high frame rates with a simple API<br>
- Light enough to run on an ATtiny85<br> 
<br>
This code depends on the BitBang_I2C library. You can download it here:<br>
<br>
https://github.com/bitbank2/BitBang_I2C
<br>

Instructions for use:<br>
---------------------<br>
Start by initializing the library. Either using hardware I2C, bit-banged I2C or SPI to talk to the display. For I2C, the
address of the display will be detected automatically (either 0x3c or 0x3d). The typical MCU only allows setting the I2C speed up to 400Khz, but the SSD1306 displays can handle a much faster signal. With the bit-bang code, you can usually specify a stable 800Khz clock and with Cortex-M0 targets, the hardware I2C can be told to be almost any speed, but the displays I've tested tend to stop working beyond 1.6Mhz. After initializing the display you can begin drawing text or graphics on it. The final parameter of all of the drawing functions is a render flag. When true, the graphics will be sent to the internal backing buffer (when available) and sent to the display. You optionally pass the library a backing buffer (if your MCU has enough RAM) with the oledSetBackBuffer() function. When the render flag is false, the graphics will only be drawn into the internal buffer. Once you're ready to send the pixels to the display, call oledDumpBuffer(NULL) and it will copy the internal buffer in its entirety to the display. The text drawing function now has a scroll offset parameter. This tells it how many pixels of the text to skip before drawing the text at the given destination coordinates. For example, if you pass a value of 20 for the scroll offset and are using an 8-pixel wide font (FONT_NORMAL), the first two and a half characters will not be drawn; the second half of the third and subsequent characters will be drawn starting at the x/y you specified. This allows you to create a scrolling text effect by repeatedly calling the oledWriteString() function with progressively larger scroll offset values to make the text scroll from right to left.<br> 

If you find this code useful, please consider sending a donation.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SR4F44J2UR8S4)

