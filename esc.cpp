#include <SPI.h>
#include "./mcp2515.h"

typedef enum {
  CAN_PACKET_SET_DUTY = 0,
  CAN_PACKET_SET_CURRENT,
  CAN_PACKET_SET_CURRENT_BRAKE,
  CAN_PACKET_SET_RPM,
  CAN_PACKET_SET_POS,
  CAN_PACKET_FILL_RX_BUFFER,
  CAN_PACKET_FILL_RX_BUFFER_LONG,
  CAN_PACKET_PROCESS_RX_BUFFER,
  CAN_PACKET_PROCESS_SHORT_BUFFER,
  CAN_PACKET_STATUS,
  CAN_PACKET_SET_CURRENT_REL,
  CAN_PACKET_SET_CURRENT_BRAKE_REL,
  CAN_PACKET_SET_CURRENT_HANDBRAKE,
  CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
  CAN_PACKET_STATUS_2,
  CAN_PACKET_STATUS_3,
  CAN_PACKET_STATUS_4,
  CAN_PACKET_PING,
  CAN_PACKET_PONG,
  CAN_PACKET_DETECT_APPLY_ALL_FOC,
  CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
  CAN_PACKET_CONF_CURRENT_LIMITS,
  CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
  CAN_PACKET_CONF_CURRENT_LIMITS_IN,
  CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
  CAN_PACKET_CONF_FOC_ERPMS,
  CAN_PACKET_CONF_STORE_FOC_ERPMS,
  CAN_PACKET_STATUS_5,
  CAN_PACKET_POLL_TS5700N8501_STATUS,
  CAN_PACKET_CONF_BATTERY_CUT,
  CAN_PACKET_CONF_STORE_BATTERY_CUT,
  CAN_PACKET_SHUTDOWN,
  CAN_PACKET_IO_BOARD_ADC_1_TO_4,
  CAN_PACKET_IO_BOARD_ADC_5_TO_8,
  CAN_PACKET_IO_BOARD_ADC_9_TO_12,
  CAN_PACKET_IO_BOARD_DIGITAL_IN,
  CAN_PACKET_IO_BOARD_SET_OUTPUT_DIGITAL,
  CAN_PACKET_IO_BOARD_SET_OUTPUT_PWM,
  CAN_PACKET_BMS_V_TOT,
  CAN_PACKET_BMS_I,
  CAN_PACKET_BMS_AH_WH,
  CAN_PACKET_BMS_V_CELL,
  CAN_PACKET_BMS_BAL,
  CAN_PACKET_BMS_TEMPS,
  CAN_PACKET_BMS_HUM,
  CAN_PACKET_BMS_SOC_SOH_TEMP_STAT
} CAN_PACKET_ID;
    
class ESC {
  private:
    MCP2515 mcp2515;
  
    int local_id = 0x03;
    int vesc_id = 0x02;

    struct can_frame responses[10];
    int respnsesLength = 0;

    uint8_t readBuffer[50];
    uint8_t readBufferLength = 0;
    uint8_t readBufferInfo[8];
    uint8_t readBufferInfoLength = 0;

    
  public:
    // RT Data vars
    double tempMosfet;
    double tempMotor;
    double dutyCycle;
    double erpm;
    double voltage;
    uint8_t fault;
    
    // Balance vars
    double pidOutput;
    double pitch;
    double roll;
    uint32_t loopTime;
    double motorCurrent;
    double motorPosition;
    uint16_t balanceState;
    uint16_t switchState;
    double adc1;
    double adc2;
    
    ESC::ESC() :
      mcp2515(10){
    }
    void setup(){
      SPI.begin();
      
      mcp2515.reset();
      mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
      mcp2515.setNormalMode();

      // For testing
      getBalance();
    }

    void loop(){
      getRealtimeData();
      getBalance();
    }
    void ping(){
      //80001101 1 1 
      //80001201 1 1 
      // 8000 dunno what this is
      // 11/12 the can command id
      // 01 the id of the esc we are talking to
      // 1 length
      // 1 id
      struct can_frame canMsg1;
      canMsg1.can_id  = (uint32_t (0x8000) << 16) + (uint16_t (CAN_PACKET_PING) << 8) + vesc_id;
      canMsg1.can_dlc = 0x01;
      canMsg1.data[0] = local_id;
      mcp2515.sendMessage(&canMsg1);
    }
  
  void pong(){
    struct can_frame canMsg1;
    canMsg1.can_id  = (uint32_t (0x8000) << 16) + (uint16_t (CAN_PACKET_PONG) << 8) + vesc_id;
    canMsg1.can_dlc = 0x01;
    canMsg1.data[0] = local_id;
    mcp2515.sendMessage(&canMsg1);
  }
  
  void getFirmware(){
    //80000803 3 2 0 0 
    struct can_frame canMsg1;
    canMsg1.can_id  = (uint32_t (0x8000) << 16) + (uint16_t (CAN_PACKET_PROCESS_SHORT_BUFFER) << 8) + vesc_id;
    canMsg1.can_dlc = 0x03;
    canMsg1.data[0] = local_id;
    canMsg1.data[1] = 0x00;
    canMsg1.data[2] = 0x00;
    mcp2515.sendMessage(&canMsg1);
  }
  
  void sendFirmware(){
    //02:41:23.906 -> 80000503 8 0 0 5 1 43 68 65 61 
    //02:41:23.906 -> 80000503 8 7 70 20 46 4F 43 65 72 
    //02:41:23.906 -> 80000503 8 E 20 32 0 21 0 48 0 
    struct can_frame canMsg1;
    canMsg1.can_id  = (uint32_t (0x8000) << 16) + (uint16_t (CAN_PACKET_FILL_RX_BUFFER) << 8) + vesc_id;
    canMsg1.can_dlc = 0x08;
    canMsg1.data[0] = 0x00;
    canMsg1.data[1] = 0x00;
    canMsg1.data[2] = 0x05;
    canMsg1.data[3] = 0x01;
    canMsg1.data[4] = 0x43;
    canMsg1.data[5] = 0x68;
    canMsg1.data[6] = 0x65;
    canMsg1.data[7] = 0x61;
    mcp2515.sendMessage(&canMsg1);
  
    Serial.print(canMsg1.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg1.can_dlc, HEX); // print DLC
    Serial.print(" ");
    for (int i = 0; i<canMsg1.can_dlc; i++)  {  // print the data
      Serial.print(canMsg1.data[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
    
  
    canMsg1.data[0] = 0x07;
    canMsg1.data[1] = 0x70;
    canMsg1.data[2] = 0x20;
    canMsg1.data[3] = 0x46;
    canMsg1.data[4] = 0x4F;
    canMsg1.data[5] = 0x43;
    canMsg1.data[6] = 0x65;
    canMsg1.data[7] = 0x72;
    mcp2515.sendMessage(&canMsg1);
  
    Serial.print(canMsg1.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg1.can_dlc, HEX); // print DLC
    Serial.print(" ");
    for (int i = 0; i<canMsg1.can_dlc; i++)  {  // print the data
      Serial.print(canMsg1.data[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
    
  
    canMsg1.data[0] = 0x0E;
    canMsg1.data[1] = 0x20;
    canMsg1.data[2] = 0x32;
    canMsg1.data[3] = 0x00;
    canMsg1.data[4] = 0x21;
    canMsg1.data[5] = 0x00;
    canMsg1.data[6] = 0x48;
    canMsg1.data[7] = 0x00;
    mcp2515.sendMessage(&canMsg1);
  
    Serial.print(canMsg1.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg1.can_dlc, HEX); // print DLC
    Serial.print(" ");
    for (int i = 0; i<canMsg1.can_dlc; i++)  {  // print the data
      Serial.print(canMsg1.data[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
    
  }

  void getRealtimeData(){
    
    //80000803 3 2 0 0 
    struct can_frame canMsg1;
    canMsg1.can_id  = (uint32_t (0x8000) << 16) + (uint16_t (CAN_PACKET_PROCESS_SHORT_BUFFER) << 8) + vesc_id;
    canMsg1.can_dlc = 0x07;
    canMsg1.data[0] = local_id;
    canMsg1.data[1] = 0x00;
    canMsg1.data[2] = 0x32;
    // mask
    canMsg1.data[3] = 0x00;
    canMsg1.data[4] = 0x00;
    canMsg1.data[5] = B10000001;
    canMsg1.data[6] = B11000011;
    mcp2515.sendMessage(&canMsg1);
  
    batchRead();
    parseRealtimeData();
  }

  bool parseRealtimeData(){
    if(readBufferLength != 0x12 || readBuffer[0] != 0x32){
      return;
    }
    tempMosfet = (((int16_t)readBuffer[5] << 8) + ((int16_t)readBuffer[6])) / 10.0;
    tempMotor = (((int16_t)readBuffer[7] << 8) + ((int16_t)readBuffer[8])) / 10.0;
    dutyCycle = (((int16_t)readBuffer[9] << 8) + ((int16_t)readBuffer[10])) / 1000.0;
    erpm = (((int32_t)readBuffer[11] << 24) + ((int32_t)readBuffer[12] << 16) + ((int32_t)readBuffer[13] << 8) + ((int32_t)readBuffer[14]));
    voltage = (((int16_t)readBuffer[15] << 8) + ((int16_t)readBuffer[16])) / 10.0;
    fault = readBuffer[17];

//    Serial.print("Mosfet Temp: ");
//    Serial.print(tempMosfet);
//    Serial.println();
//    Serial.print("voltage: ");
//    Serial.print(voltage);
//    Serial.println();
//    Serial.print("faultoll: ");
//    Serial.print(fault);
//    Serial.println();
  }
  
  void getBalance(){
    
    //80000803 3 2 0 0 
    struct can_frame canMsg1;
    canMsg1.can_id  = (uint32_t (0x8000) << 16) + (uint16_t (CAN_PACKET_PROCESS_SHORT_BUFFER) << 8) + vesc_id;
    canMsg1.can_dlc = 0x03;
    canMsg1.data[0] = local_id;
    canMsg1.data[1] = 0x00;
    canMsg1.data[2] = 0x4F;
  //  canMsg1.data[2] = 0x00;
    mcp2515.sendMessage(&canMsg1);
  
    batchRead();
    parseBalance();
  }

  

  bool parseBalance(){
    if(readBufferLength != 0x25 || readBuffer[0] != 0x4F){
      return;
    }
    pidOutput = (((int32_t)readBuffer[1] << 24) + ((int32_t)readBuffer[2] << 16) + ((int32_t)readBuffer[3] << 8) + ((int32_t)readBuffer[4])) / 1000000.0;
    pitch = (((int32_t)readBuffer[5] << 24) + ((int32_t)readBuffer[6] << 16) + ((int32_t)readBuffer[7] << 8) + ((int32_t)readBuffer[8])) / 1000000.0;
    roll = (((int32_t)readBuffer[9] << 24) + ((int32_t)readBuffer[10] << 16) + ((int32_t)readBuffer[11] << 8) + ((int32_t)readBuffer[12])) / 1000000.0;
    loopTime;
    motorCurrent;
    motorPosition;
    balanceState;
    switchState;
    adc1;
    adc2;

//    Serial.print("PID Output: ");
//    Serial.print(pidOutput);
//    Serial.println();
//    Serial.print("Pitch: ");
//    Serial.print(pitch);
//    Serial.println();
//    Serial.print("Roll: ");
//    Serial.print(roll);
//    Serial.println();
  }
  
  void printFrame(struct can_frame *frame){
    Serial.print(frame->can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(frame->can_dlc, HEX); // print DLC
    Serial.print(" ");
    for (int i = 0; i < frame->can_dlc; i++)  {  // print the data
      Serial.print(frame->data[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  bool batchRead(){
    
    respnsesLength = 0;
    for(int j = 0; j < 1000 && respnsesLength < 10; j++){
      if (mcp2515.readMessage(&responses[respnsesLength]) == MCP2515::ERROR_OK) {
        respnsesLength++;
      }
    }

//    for(int i = 0; i < respnsesLength; i++){
//      printFrame(&responses[i]);
//    }
  
    
    // Clear buffer
    readBufferLength = 0;
    for(int i = 0; i < 50; i++){
      readBuffer[i] = 0; //(Not really necessary ?)
    }
    readBufferInfoLength = 0;
    for(int i = 0; i < 8; i++){
      readBufferInfo[i] = 0; //(Not really necessary ?)
    }
    // Convert can frames to full buffer
    for(int i = 0; i < respnsesLength; i++){
      if(responses[i].can_id == 0x80000000 + ((uint16_t)CAN_PACKET_FILL_RX_BUFFER << 8) + local_id){
        for(int j = 1; j < responses[i].can_dlc; j++){
          readBuffer[responses[i].data[0]+j-1] = responses[i].data[j];
          readBufferLength++;
        }
      }else if(responses[i].can_id == 0x80000000 + ((uint16_t)CAN_PACKET_PROCESS_RX_BUFFER << 8) + local_id){
        for(int j = 0; j < responses[i].can_dlc; j++){
          readBufferInfo[j] = responses[i].data[j];
          readBufferInfoLength++;
        }
      }
    }

//    for(int i = 0; i < readBufferLength; i++){
//      Serial.print(readBuffer[i],HEX);
//      Serial.print(" ");
//    }
//    Serial.println();
//
//    for(int i = 0; i < readBufferInfoLength; i++){
//      Serial.print(readBufferInfo[i],HEX);
//      Serial.print(" ");
//    }
//    Serial.println();

    // TODO: Proper read validation with checksum, for now we just compare lengths, can improove if data is error prone.
    uint16_t supposedLength = ((uint16_t)readBufferInfo[2] << 8) + readBufferInfo[3];
    if(readBufferLength != supposedLength){
      readBufferLength = 0;
      readBufferInfoLength = 0;
    }
  }

};
