#include "uart.h"
#include <Arduino.h> 
#include <HardwareSerial.h>

#define FRAME_HEADER 0xA5

Rev_data_t receivedData;
uint8_t    STM_Connected;


Rev_data_t *get_movexy_data(void)
{
    return &receivedData;
  }

HardwareSerial Serial2Stm32(1);
uint8_t check_sum;

void UART::setup()
{
    Serial.begin(115200);
    Serial2Stm32.begin(115200,SERIAL_8N1,22,23);//R T
  }

//接收数据
bool UART::receiveData(Rev_data_t &data) {
    const int dataSize = 78;
    static uint8_t buffer[dataSize];
    static int index = 0;

    while (Serial2Stm32.available() > dataSize) {
        uint8_t byte = Serial2Stm32.read();

        if (index == 0 && byte == FRAME_HEADER) 
        {
            buffer[index++] = byte; // 帧头匹配
        } 
        else if (index > 0) 
        {
           buffer[index++] = byte;
     
           if (index == dataSize) 
           {
               memcpy(&data, buffer, dataSize);
               index = 0;
               STM_Connected=1;
               return true;

            }
        } 
        else
        {
            index = 0; // 重新开始匹配
        }
    }
    STM_Connected=0;
    return false;
}

//bool UART::receiveData(Rev_data_t &data) {
//    const int dataSize = 79;
//    static uint8_t buffer[dataSize];
//    static int index = 0;
//
//    while (Serial2Stm32.available() > 0) {
//        uint8_t byte = Serial2Stm32.read();
//        Serial.print(byte, HEX); // 打印接收到的每个字节
//        Serial.print(" ");
//
//        if (index == 0 && byte == FRAME_HEADER) {
//            buffer[index++] = byte; // 帧头匹配
//        } else if (index > 0) {
//            buffer[index++] = byte;
//            if (index == dataSize) {
//                // 打印接收到的数据
//                for (int i = 0; i < dataSize; i++) {
//                    Serial.print(buffer[i], HEX);
//                    Serial.print(" ");
//                }
//                Serial.println();
//
//                // 检查校验和
//                uint8_t receivedChecksum = buffer[dataSize - 1];
//                uint8_t calculatedChecksum = calculateChecksum(buffer, 78);
//                
//                Serial.print("Received Checksum: ");
//                Serial.println(receivedChecksum, HEX);
//                Serial.print("Calculated Checksum: ");
//                Serial.println(calculatedChecksum, HEX);
//
//                if (receivedChecksum == calculatedChecksum) {
//                    memcpy(&data, buffer, dataSize);
//                    index = 0;
//                    return true;
//                } else {
//                    Serial.println("Checksum error");
//                    index = 0; // 校验和错误，重新开始匹配
//                }
//            }
//        } else {
//            index = 0; // 重新开始匹配
//        }
//    }
//    return false;
//}
void UART::printData()
{
  if (receiveData(receivedData)) {

        Serial.println("Data received:");
        Serial.print("Head: "); Serial.println(receivedData.head);
        Serial.print("CmdID: "); Serial.println(receivedData.cmdID);
        Serial.print("End Position X: "); Serial.println(receivedData.end_positionX);
        Serial.print("End Position Y: "); Serial.println(receivedData.end_positionY);
        Serial.print("End Speed X: "); Serial.println(receivedData.end_speedX);
        Serial.print("End Speed Y: "); Serial.println(receivedData.end_speedY);
        Serial.print("Yaw: "); Serial.println(receivedData.yaw);
        Serial.print("Pitch: "); Serial.println(receivedData.pitch);
        Serial.print("Roll: "); Serial.println(receivedData.roll);
        Serial.print("Accel X: "); Serial.println(receivedData.accel[0]);
        Serial.print("Accel Y: "); Serial.println(receivedData.accel[1]);
        Serial.print("Accel Z: "); Serial.println(receivedData.accel[2]);
        Serial.print("Gyro X: "); Serial.println(receivedData.gyro[0]);
        Serial.print("Gyro Y: "); Serial.println(receivedData.gyro[1]);
        Serial.print("Gyro Z: "); Serial.println(receivedData.gyro[2]);
        Serial.print("Quat W: "); Serial.println(receivedData.quat[0]);
        Serial.print("Quat X: "); Serial.println(receivedData.quat[1]);
        Serial.print("Quat Y: "); Serial.println(receivedData.quat[2]);
        Serial.print("Quat Z: "); Serial.println(receivedData.quat[3]);
        Serial.print("F Buttons: ");
        Serial.print(receivedData.F_button[0]); Serial.print(" ");
        Serial.print(receivedData.F_button[1]); Serial.print(" ");
        Serial.println(receivedData.F_button[2]);
        Serial.print("Two Buttons: ");
        Serial.print(receivedData.Two_button[0]); Serial.print(" ");
        Serial.println(receivedData.Two_button[1]);
        Serial.print("Encoder: "); Serial.println(receivedData.encoder);

  }
  }

uint16_t calculateChecksum(const uint8_t* data, uint16_t length) {
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}
