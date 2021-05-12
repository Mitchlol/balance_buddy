#include "beeper.cpp"

#define BEEPER_PIN 4

#define PLAY_STARTUP true
#define DUTY_CYCLE_ALERT 0.85 // 0 to disable
#define SWITCH_ERPM 1000 // 0 to disable
#define LOW_VOLTAGE 52.0 // 0 to disable
#define LOW_VOLTAGE_INTERVAL 30 * 1000 // every 30 seconds

class BalanceBeeper {
  private:
    Beeper beeper;

    bool switchStateLatch = false;
    long lastLowVoltageMillis = 0;
  public:
    BalanceBeeper() :
      beeper(BEEPER_PIN){
    }
    void setup(){ 
      beeper.setup();
      if(PLAY_STARTUP){
        beeper.queueThreeShort();
      }
    }

    void loop(double dutyCycle, double erpm, uint16_t switchState, double voltage){
      // Non latching beeps for Duty Cycle
      if(fabsf(dutyCycle) > DUTY_CYCLE_ALERT && DUTY_CYCLE_ALERT > 0){
        beeper.queueShortSingle();
      }

      // Latching beep for HALF footpad state
      if(switchState == 1 && fabsf(erpm) > SWITCH_ERPM && SWITCH_ERPM > 0 && switchStateLatch == false){
        switchStateLatch = true;
        beeper.queueLongSingle();
      }else if(switchState != 1){
        switchStateLatch = false;
      }

      // Low voltage, time based repeat
      if(voltage < LOW_VOLTAGE && LOW_VOLTAGE > 0 && lastLowVoltageMillis + LOW_VOLTAGE_INTERVAL < millis()){
        beeper.queueSad();
        lastLowVoltageMillis = millis();
      }

      beeper.loop();
    }

};
