#include "display.cpp"
#include "beeper.cpp"
#include "led.cpp"

ESC esc;
Display mDisplay(esc);
Beeper beeper;
BalanceLEDs led;


void setup() {
  
  Serial.begin(115200);

  esc.setup();
  mDisplay.setup();
  beeper.setup();
  led.setup();
}

void loop() {
  esc.loop();
  mDisplay.loop();
  beeper.loop();
  led.loop(esc.erpm);

  if(esc.pitch > 10){
    beeper.queueThreeShort();    
  }

  if(esc.roll > 10){
    beeper.queueLong();    
  }

}
