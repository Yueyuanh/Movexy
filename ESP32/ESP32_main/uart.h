#ifndef UART_H
#define UART_H

#include "struct_typedef.h"

typedef struct {
  
    uint8_t head;
    uint8_t cmdID;
    
    float end_positionX;
    float end_positionY;
    float end_speedX;
    float end_speedY;
    
    float yaw, pitch, roll;
    float accel[3]; // xyz
    float gyro[3];  // xyz
    float quat[4];  // wxyz
    
    uint8_t F_button[3]; // F1, F2, F3
    uint8_t Two_button[2]; // 0↓, 1↑
    uint8_t encoder;
    
    uint8_t checksum; // 校验和
} Rev_data_t;

class UART
{
  public:
    void setup();
    bool receiveData(Rev_data_t &data);
    void printData();
};

Rev_data_t *get_movexy_data(void);
uint16_t calculateChecksum(const uint8_t* data, uint16_t length);
#endif
