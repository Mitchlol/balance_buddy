#include "./src/ss_oled/src/ss_oled.h"
#include "esc.cpp"

// Not enough memory
//#define USE_BACKBUFFER

#ifdef USE_BACKBUFFER
static uint8_t ucBackBuffer[1024];
#else
static uint8_t *ucBackBuffer = NULL;
#endif

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are the pin numbers for the M5Stack Atom default I2C
#define SDA_PIN A4
#define SCL_PIN A5
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let ss_oled figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 0

// My oled
#define MY_OLED OLED_128x64
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

class BalanceDisplay {
  private:
    SSOLED ssoled;
    long currentMillis;
    long previousMillis;
    int fps;

    char buffer[32];
  public:
    void setup(){
      int rc;
      // The I2C SDA/SCL pins set to -1 means to use the default Wire library
      // If pins were specified, they would be bit-banged in software
      // This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
      // The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
      //    that can be configured as either SPI or I2C
      //
      // oledInit(SSOLED *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)
      
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
        if (rc != OLED_NOT_FOUND){
          Serial.print("Display FOunt");
          Serial.println();
          char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D",(char *)"SH1106 @ 0x3C",(char *)"SH1106 @ 0x3D"};
          oledFill(&ssoled, 0, 1);
          oledWriteString(&ssoled, 0,0,0,msgs[rc], FONT_NORMAL, 0, 1);
          oledSetBackBuffer(&ssoled, ucBackBuffer);
          delay(1000);
        }else{
          Serial.print("Display Not FOund");
          Serial.println();
        }
    }

    void loop(double tempMosfet, double dutyCycle, double voltage, uint16_t balanceState, uint16_t switchState, double adc1, double adc2){
//    oledFill(&ssoled, 0, 1);

    currentMillis = millis();
    fps = 1000/(currentMillis - previousMillis);
    previousMillis = currentMillis;
    sprintf(buffer, "%d                 ", fps);
    oledWriteString(&ssoled, 0, 0, 6, buffer, FONT_NORMAL, 0, 1);
    

//    oledWriteString(&ssoled, 0, 16, 0,(char *)String(millis()), FONT_NORMAL, 0, 1);
//    oledWriteString(&ssoled, 0, 0, 1,(char *)"Written by Larry Bank", FONT_SMALL, 1, 1);
//    oledWriteString(&ssoled, 0, 0, 3,(char *)"**Demo**", FONT_STRETCHED, 0, 1);
//    oledWriteString(&ssoled, 0, 9, 6,(char *)"for AVR", FONT_STRETCHED, 0, 1);
//    oledDumpBuffer(&ssoled, NULL);


//      oled.home();
//
      // Line 1: Voltage and Temp
      sprintf(buffer, "%sV ", String(voltage, 1).c_str());
      oledWriteString(&ssoled, 0, 0, 0, buffer, FONT_12x16 , 0, 1);
      sprintf(buffer, "%sc", String(tempMosfet, 1).c_str());
      oledWriteString(&ssoled, 0, 128 - (12*5), 0, buffer, FONT_12x16 , 0, 1);

      // Line 2: ADC Switches
      sprintf(buffer, "ADC1: %s", String(adc1, 2).c_str());
      oledWriteString(&ssoled, 0, 0, 2, buffer, FONT_6x8, 0, 1);
      sprintf(buffer, "ADC2: %s", String(adc2, 2).c_str());
      oledWriteString(&ssoled, 0, 0, 3, buffer, FONT_6x8, 0, 1);
      if(switchState == 0){
        sprintf(buffer, "OFF ");
      }else if(switchState == 1){
        sprintf(buffer, "HALF");
      }else{
        sprintf(buffer, "ON  ");
      }
      oledWriteString(&ssoled, 0, 128 - (4*16), 2, buffer, FONT_16x16 , 0, 1);

      // Line 3: Balance state
      if(balanceState == 0){
        sprintf(buffer, "Calibrating           ");
      }else if(balanceState == 1){
        sprintf(buffer, "Running               ");
      }else if(balanceState == 2){
        sprintf(buffer, "Run: Tiltback Duty    ");
      }else if(balanceState == 3){
        sprintf(buffer, "Run: Tiltback HV      ");
      }else if(balanceState == 4){
        sprintf(buffer, "Run: Tiltback LV      ");
      }else if(balanceState == 5){
        sprintf(buffer, "Run: Tilt Constant    ");
      }else if(balanceState == 6){
        sprintf(buffer, "Fault: Pitch Angle    ");
      }else if(balanceState == 7){
        sprintf(buffer, "Fault: Roll Angle     ");
      }else if(balanceState == 8){
        sprintf(buffer, "Fault: Switch Half    ");
      }else if(balanceState == 9){
        sprintf(buffer, "Fault: Switch Full    ");
      }else if(balanceState == 10){
        sprintf(buffer, "Fault: Duty           ");
      }else if(balanceState == 11){
        sprintf(buffer, "Initial               ");
      }else{
        sprintf(buffer, "Initial %D            ", balanceState);
      }
      oledWriteString(&ssoled, 0, 0, 4, buffer, FONT_6x8 , 0, 1);

      // Line 4: Duty Cycle (up to 64 verical bars)
//      for(float i = 0; i < fabsf(dutyCycle); i += 0.015){
//        oled.print("|");
//      }
//      oled.print("                           ");
//      oled.println();
      dutyCycle = 1;
      sprintf(buffer, "%s%*s", buffer, fabsf(dutyCycle) * 22, "");
//      for(float i = 0; i < fabsf(dutyCycle); i += 0.045454545){
//        oledWriteString(&ssoled, 0, i, 6, buffer, FONT_6x8 , 1, 1);
//        oledWriteString(&ssoled, 0, i, 7, buffer, FONT_6x8 , 1, 1);
//      }
      oledWriteString(&ssoled, 0, 0, 7, buffer, FONT_6x8 , 1, 1);
    }
};
