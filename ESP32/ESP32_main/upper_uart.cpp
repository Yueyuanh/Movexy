/**
  * @brief          上位机发送/接收
  * @param[in]      NULL
  * @retval         NULL
  * 
  * x 0-152
  * y o-118
  * 
**/


#include "upper_uart.h"

#include <Arduino.h> 
#include <HardwareSerial.h>

#define FRAME_HEADER 0xC5

Send_upper_data_t send_upper_Data;
extern Rev_data_t receivedData;


void UPPER::setup()
{

}

//发送数据更新
void UPPER::upper_data_update()
{
  
}


//接收数据
bool UPPER::send_upper_data(Rev_data_t &data) 
{
  Serial.write((uint8_t*)&data, sizeof(Rev_data_t));
  
}
