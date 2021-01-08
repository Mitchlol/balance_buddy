#include <stdint.h>
#include <arduino.h>

class Beeper {
  private:
    struct Beep{
       int startOffset = 0; 
       int frequency = 0;
       bool triggered = false;
    };
    long beepStartTime = 0;
    bool isBeeping = false;
    int beepsLength = 0;
    struct Beep beeps[20];
  public:
    
    void setup(){
    }

    void loop(){
      if(isBeeping){
        long timeInBeeping = millis() - beepStartTime;
        for(int i = 0; i < beepsLength; i++){
          if(timeInBeeping > beeps[i].startOffset && beeps[i].triggered == false){
            beeps[i].triggered = true;
            if(beeps[i].frequency > 0){
              tone(4, beeps[i].frequency);
            }else{
              noTone(4);
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
      beepsLength = 7;
      // On 1
      beeps[0].startOffset = 0;
      beeps[0].frequency = 440;
      beeps[0].triggered = false;
      // Off
      beeps[1].startOffset = 250;
      beeps[1].frequency = 0;
      beeps[1].triggered = false;
      // On 2
      beeps[2].startOffset = 350;
      beeps[2].frequency = 880;
      beeps[2].triggered = false;
      // Off
      beeps[3].startOffset = 600;
      beeps[3].frequency = 0;
      beeps[3].triggered = false;
      // On 3
      beeps[4].startOffset = 700;
      beeps[4].frequency = 1760;
      beeps[4].triggered = false;
      // off
      beeps[5].startOffset = 950;
      beeps[5].frequency = 0;
      beeps[5].triggered = false;
      // off off
      beeps[6].startOffset = 1050;
      beeps[6].frequency = 0;
      beeps[6].triggered = false;
    }

    void queueLong(){
      if(isBeeping){
        return;
      }
      
      beepStartTime = millis();
      isBeeping = true;
      beepsLength = 3;
      // On 1
      beeps[0].startOffset = 0;
      beeps[0].frequency = 440;
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
};
