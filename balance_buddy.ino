#include "esc.cpp"

ESC esc;


void setup() {
  
  
  Serial.begin(115200);
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");

  esc.setup();
  displaySetup();
}

void loop() {
  esc.loop();
  displayLoop(&esc);
  delay(100);
}
