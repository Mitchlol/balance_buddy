#include "SSD1306Ascii.h"
#include "config.h"

class BalanceDisplay
{
private:
#ifdef USE_I2C
  SSD1306AsciiWire oled;
#else
  SSD1306AsciiSpi oled;
#endif
  long lastRenderMillis = 0;
  int line = 1;

public:
  void setup()
  {
#ifdef USE_I2C
    Wire.begin();
    Wire.setClock(400000L);
    oled.begin(&Adafruit128x64, 0x3c);
#else
    oled.begin(&Adafruit128x64, CS_PIN, DC_PIN, RES_PIN);
#endif
  }

  void loop(double tempMosfet, double dutyCycle, double voltage, uint16_t balanceState, uint16_t switchState, double adc1, double adc2)
  {
    if (lastRenderMillis + REFRESH_INTERVAL < millis())
    {
      lastRenderMillis = millis();

      switch (line) // only write one line at a time to save cycle time
      {
      case 1:
        oled.home();

        // Line 1: Voltage and Temp
        oled.setFont(ZevvPeep8x16);
        oled.set1X();
        oled.print(voltage);
        oled.print("V");
        oled.print("  ");
        oled.print(tempMosfet);
        oled.println("c");
        break;

      case 2:
        // Line 2: ADC Switches
        oled.setFont(Adafruit5x7);
        oled.set1X();
        oled.print("ADC1: ");
        oled.print(adc1);
        oled.set2X();
        if (switchState == 0)
        {
          oled.print(" OFF ");
        }
        else if (switchState == 1)
        {
          oled.print(" HALF");
        }
        else
        {
          oled.print(" ON  ");
        }
        oled.set1X();
        oled.println();
        oled.print("ADC2: ");
        oled.println(adc2);
        break;

      case 3:
        // Line 3: Balance state
        oled.setFont(TimesNewRoman13);
        if (balanceState == 0)
        {
          oled.println("Calibrating             ");
        }
        else if (balanceState == 1)
        {
          oled.println("Running                 ");
        }
        else if (balanceState == 2)
        {
          oled.println("Run: Tiltback Duty      ");
        }
        else if (balanceState == 3)
        {
          oled.println("Run: Tiltback HV        ");
        }
        else if (balanceState == 4)
        {
          oled.println("Run: Tiltback LV        ");
        }
        else if (balanceState == 5)
        {
          oled.println("Run: Tilt Constant      ");
        }
        else if (balanceState == 6)
        {
          oled.println("Fault: Pitch Angle      ");
        }
        else if (balanceState == 7)
        {
          oled.println("Fault: Roll Angle       ");
        }
        else if (balanceState == 8)
        {
          oled.println("Fault: Switch Half      ");
        }
        else if (balanceState == 9)
        {
          oled.println("Fault: Switch Full      ");
        }
        else if (balanceState == 10)
        {
          oled.println("Fault: Duty             ");
        }
        else if (balanceState == 11)
        {
          oled.println("Initial                 ");
        }
        else
        {
          oled.print("Unknown: ");
          oled.print(balanceState);
          oled.println("                ");
        }
        break;

      case 4:
        // Line 4: Duty Cycle (up to 64 verical bars)
        oled.setFont(TimesNewRoman16_bold);
        for (float i = 0; i < fabsf(dutyCycle); i += 0.015)
        {
          oled.print("|");
        }
        oled.print("                           ");
        oled.println();

        line = 0;
        break;
      }

      line++;
    }
  }
};
