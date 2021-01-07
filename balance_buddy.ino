#include "display.cpp"

ESC esc;
Display mDisplay(esc);


void setup() {
  
  Serial.begin(115200);

  esc.setup();
  mDisplay.setup();
}

void loop() {
  esc.loop();
  mDisplay.loop();
  delay(1000);
}
