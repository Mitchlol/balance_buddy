#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include "esc.cpp"

class Display {
  private:
    SSD1306AsciiWire oled;
    ESC &esc;
  public:
    Display(ESC &esc): esc(esc){};
    void setup(){
      Wire.begin();
      Wire.setClock(400000L);
      
      oled.begin(&Adafruit128x64, 0x3c);
      oled.setFont(Adafruit5x7);
    }

    void loop(){
      oled.home();
      oled.set2X();
      
      oled.print("P: ");
      oled.print(esc.pitch);
      oled.println("    ");
    
      oled.print("R: ");
      oled.print(esc.roll);
      oled.println("    ");

      oled.print(esc.voltage);
      oled.println("V    ");

      oled.print(esc.erpm);
      oled.println(" <-ADC1    ");
    }
};
