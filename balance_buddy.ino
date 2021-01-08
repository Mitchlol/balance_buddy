#include "display.cpp"
#include "beeper.cpp"

ESC esc;
Display mDisplay(esc);
Beeper beeper;


void setup() {
  
  Serial.begin(115200);

  esc.setup();
  mDisplay.setup();
  beeper.setup();
}

void loop() {
  esc.loop();
  mDisplay.loop();
  beeper.loop();
//  delay(1000);

  if(esc.pitch > 10){
    beeper.queueThreeShort();    
  }

  if(esc.roll > 10){
    beeper.queueLong();    
  }

}
