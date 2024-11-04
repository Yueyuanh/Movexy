#ifndef UPPER_H
#define UPPER_H

#include "struct_typedef.h"
#include "uart.h"

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
    
}Send_upper_data_t;

class UPPER
{
  public:
    uint8_t is_connected;
    
    void setup();
    void upper_data_update();//更新数据，方便以后添加数据
    bool send_upper_data(Rev_data_t &data);//发送数据
    void printData();
};


#endif
