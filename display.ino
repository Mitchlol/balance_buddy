#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>


SSD1306AsciiWire oled;

void displaySetup(){
  Wire.begin();
  Wire.setClock(400000L);
  
  oled.begin(&Adafruit128x64, 0x3c);
  oled.setFont(Adafruit5x7);
}

void displayLoop(ESC *esc){
//  oled.clear();
  oled.home();
  oled.set2X();
  
//  oled.clearToEOL();
  oled.print("P: ");
  oled.print(esc->pitch);
  oled.println("    ");

  oled.print("R: ");
  oled.print(esc->roll);
  oled.println("    ");

//  oled.println("abcdefghijkl");
//  oled.println(".;:'^*&%$#S_");
}
