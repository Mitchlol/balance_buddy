#include <Arduino.h>
#include "balance_display.cpp"
#include "balance_beeper.cpp"
#include "balance_leds.cpp"

ESC esc;
BalanceDisplay balanceDisplay;
BalanceBeeper balanceBeeper;
BalanceLEDs balanceLEDs;


void setup() {
  
  Serial.begin(115200);

  esc.setup();
  balanceDisplay.setup();
  balanceBeeper.setup();
  balanceLEDs.setup();
}

void loop() {
  esc.loop();
  balanceDisplay.loop(esc.tempMosfet, esc.dutyCycle, esc.voltage, esc.balanceState, esc.switchState, esc.adc1, esc.adc2);
  balanceBeeper.loop(esc.dutyCycle, esc.erpm, esc.switchState, esc.voltage);
  balanceLEDs.loop(esc.erpm, esc.switchState);
  // No delay? #YOLO
}
