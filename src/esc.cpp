#include <CAN.h>

#define ESC_CAN_ID 2
#define BALANCE_BUDDY_CAN_ID 3

#define RX_PIN 4
#define TX_PIN 5

typedef enum
{
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

class ESC
{
private:
    struct can_frame
    {
        unsigned long can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
        unsigned char can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
        unsigned char data[8] __attribute__((aligned(8)));
    };

    struct can_frame responses[10];
    int responsesLength = 0;

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

    void setup()
    {
        CAN.setPins(RX_PIN, TX_PIN);
        // start the CAN bus at 500 kbps
        CAN.begin(500E3);

        // For testing
        getBalance();
    }

    void loop()
    {
        getRealtimeData();
        getBalance();
    }

    void getRealtimeData()
    {
        struct can_frame canMsg1;
        canMsg1.can_id = (uint16_t(CAN_PACKET_PROCESS_SHORT_BUFFER) << 8) + ESC_CAN_ID;
        canMsg1.can_dlc = 0x07;
        canMsg1.data[0] = BALANCE_BUDDY_CAN_ID;
        canMsg1.data[1] = 0x00;
        canMsg1.data[2] = 0x32;
        canMsg1.data[3] = 0x00;
        canMsg1.data[4] = 0x00;
        canMsg1.data[5] = B10000001;
        canMsg1.data[6] = B11000011;

        CAN.beginExtendedPacket(canMsg1.can_id, canMsg1.can_dlc);
        CAN.write(canMsg1.data, canMsg1.can_dlc);
        CAN.endPacket();

        batchRead();
        parseRealtimeData();
    }

    void parseRealtimeData()
    {
        if (readBufferLength != 0x12 || readBuffer[0] != 0x32)
        {
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

    void getBalance()
    {
        struct can_frame canMsg1;
        canMsg1.can_id = (uint16_t(CAN_PACKET_PROCESS_SHORT_BUFFER) << 8) + ESC_CAN_ID;
        canMsg1.can_dlc = 0x03;
        canMsg1.data[0] = BALANCE_BUDDY_CAN_ID;
        canMsg1.data[1] = 0x00;
        canMsg1.data[2] = 0x4F;

        CAN.beginExtendedPacket(canMsg1.can_id, canMsg1.can_dlc);
        CAN.write(canMsg1.data, canMsg1.can_dlc);
        CAN.endPacket();

        batchRead();
        parseBalance();
    }

    void parseBalance()
    {
        if (readBufferLength != 0x25 || readBuffer[0] != 0x4F)
        {
            return;
        }
        pidOutput = (((int32_t)readBuffer[1] << 24) + ((int32_t)readBuffer[2] << 16) + ((int32_t)readBuffer[3] << 8) + ((int32_t)readBuffer[4])) / 1000000.0;
        pitch = (((int32_t)readBuffer[5] << 24) + ((int32_t)readBuffer[6] << 16) + ((int32_t)readBuffer[7] << 8) + ((int32_t)readBuffer[8])) / 1000000.0;
        roll = (((int32_t)readBuffer[9] << 24) + ((int32_t)readBuffer[10] << 16) + ((int32_t)readBuffer[11] << 8) + ((int32_t)readBuffer[12])) / 1000000.0;
        loopTime = ((uint32_t)readBuffer[13] << 24) + ((uint32_t)readBuffer[14] << 16) + ((uint32_t)readBuffer[15] << 8) + ((uint32_t)readBuffer[16]);
        motorCurrent = (((int32_t)readBuffer[17] << 24) + ((int32_t)readBuffer[18] << 16) + ((int32_t)readBuffer[19] << 8) + ((int32_t)readBuffer[20])) / 1000000.0;
        motorPosition = (((int32_t)readBuffer[21] << 24) + ((int32_t)readBuffer[22] << 16) + ((int32_t)readBuffer[23] << 8) + ((int32_t)readBuffer[24])) / 1000000.0;
        balanceState = ((uint16_t)readBuffer[25] << 8) + ((uint16_t)readBuffer[26]);
        switchState = ((uint16_t)readBuffer[27] << 8) + ((uint16_t)readBuffer[28]);
        adc1 = (((int32_t)readBuffer[29] << 24) + ((int32_t)readBuffer[30] << 16) + ((int32_t)readBuffer[31] << 8) + ((int32_t)readBuffer[32])) / 1000000.0;
        adc2 = (((int32_t)readBuffer[33] << 24) + ((int32_t)readBuffer[34] << 16) + ((int32_t)readBuffer[35] << 8) + ((int32_t)readBuffer[36])) / 1000000.0;
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

    void printFrame(struct can_frame *frame)
    {
        Serial.print(frame->can_id, HEX); // print ID
        Serial.print(" ");
        Serial.print(frame->can_dlc, HEX); // print DLC
        Serial.print(" ");
        for (int i = 0; i < frame->can_dlc; i++)
        { // print the data
            Serial.print(frame->data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    void batchRead()
    {

        responsesLength = 0;
        for (int j = 0; j < 1000 && responsesLength < 10; j++)
        {
            int packetSize = CAN.parsePacket();
            if (packetSize)
            {
                responses[responsesLength].can_id = CAN.packetId();
                responses[responsesLength].can_dlc = packetSize;
                for (int i = 0; i < packetSize; i++)
                {
                    responses[responsesLength].data[i] = CAN.read();
                }
                responsesLength++;
            }
            delayMicroseconds(10);
        }

        //    for(int i = 0; i < responsesLength; i++){
        //      printFrame(&responses[i]);
        //    }

        // Clear buffer
        readBufferLength = 0;
        for (int i = 0; i < 50; i++)
        {
            readBuffer[i] = 0; //(Not really necessary ?)
        }
        readBufferInfoLength = 0;
        for (int i = 0; i < 8; i++)
        {
            readBufferInfo[i] = 0; //(Not really necessary ?)
        }
        // Convert can frames to full buffer
        for (int i = 0; i < responsesLength; i++)
        {
            if (responses[i].can_id == ((uint16_t)CAN_PACKET_FILL_RX_BUFFER << 8) + BALANCE_BUDDY_CAN_ID)
            {
                for (int j = 1; j < responses[i].can_dlc; j++)
                {
                    readBuffer[responses[i].data[0] + j - 1] = responses[i].data[j];
                    readBufferLength++;
                }
            }
            else if (responses[i].can_id == ((uint16_t)CAN_PACKET_PROCESS_RX_BUFFER << 8) + BALANCE_BUDDY_CAN_ID)
            {
                for (int j = 0; j < responses[i].can_dlc; j++)
                {
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
        if (readBufferLength != supposedLength)
        {
            readBufferLength = 0;
            readBufferInfoLength = 0;
        }
    }
};
