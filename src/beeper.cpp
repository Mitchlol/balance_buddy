#include <stdint.h>
#include <arduino.h>

class Beeper {
  private:
    struct Beep{
       int startOffset = 0; 
       int frequency = 0;
       bool triggered = false;
    };
    int pin;
    long beepStartTime = 0;
    bool isBeeping = false;
    int beepsLength = 0;
    struct Beep beeps[20];
  public:
    Beeper(int beeper_pin){
      pin = beeper_pin;
    }
    
    void setup(){
    }

    void loop(){
      if(isBeeping){
        long timeInBeeping = millis() - beepStartTime;
        for(int i = 0; i < beepsLength; i++){
          if(timeInBeeping > beeps[i].startOffset && beeps[i].triggered == false){
            beeps[i].triggered = true;
            if(beeps[i].frequency > 0){
              tone(pin, beeps[i].frequency);
            }else{
              noTone(pin);
            }
            if(i == beepsLength -1){
              isBeeping = false;
            }
          }
        }
      }
    }

    void queueThreeShort(){
      if(isBeeping){
        return;
      }
      
      beepStartTime = millis();
      isBeeping = true;
      beepsLength = 5;
      // On 1
      beeps[0].startOffset = 0;
      beeps[0].frequency = 2000;
      beeps[0].triggered = false;
      // On 2
      beeps[1].startOffset = 100;
      beeps[1].frequency = 2300;
      beeps[1].triggered = false;
      // On 3
      beeps[2].startOffset = 200;
      beeps[2].frequency = 2600;
      beeps[2].triggered = false;
      // On 4
      beeps[3].startOffset = 300;
      beeps[3].frequency = 2900;
      beeps[3].triggered = false;
      // off
      beeps[4].startOffset = 600;
      beeps[4].frequency = 0;
      beeps[4].triggered = false;
    }

    void queueShortSingle(){
      if(isBeeping){
        return;
      }
      
      beepStartTime = millis();
      isBeeping = true;
      beepsLength = 3;
      // On 1
      beeps[0].startOffset = 0;
      beeps[0].frequency = 2600;
      beeps[0].triggered = false;
      // Off
      beeps[1].startOffset = 300;
      beeps[1].frequency = 0;
      beeps[1].triggered = false;
      // off off
      beeps[2].startOffset = 300;
      beeps[2].frequency = 0;
      beeps[2].triggered = false;
    }

    void queueLongSingle(){
      if(isBeeping){
        return;
      }
      
      beepStartTime = millis();
      isBeeping = true;
      beepsLength = 3;
      // On 1
      beeps[0].startOffset = 0;
      beeps[0].frequency = 2600;
      beeps[0].triggered = false;
      // Off
      beeps[1].startOffset = 750;
      beeps[1].frequency = 0;
      beeps[1].triggered = false;
      // off off
      beeps[2].startOffset = 1000;
      beeps[2].frequency = 0;
      beeps[2].triggered = false;
    }

    void queueSad(){
      if(isBeeping){
        return;
      }
      
      beepStartTime = millis();
      isBeeping = true;
      beepsLength = 5;
      // Sad
      beeps[0].startOffset = 0;
      beeps[0].frequency = 2900;
      beeps[0].triggered = false;
      // Sad
      beeps[1].startOffset = 100;
      beeps[1].frequency = 2600;
      beeps[1].triggered = false;
      // Sad
      beeps[2].startOffset = 200;
      beeps[2].frequency = 2300;
      beeps[2].triggered = false;
      // Sad
      beeps[3].startOffset = 300;
      beeps[3].frequency = 2000;
      beeps[3].triggered = false;
      // off 
      beeps[4].startOffset = 600;
      beeps[4].frequency = 0;
      beeps[4].triggered = false;
    }
};
