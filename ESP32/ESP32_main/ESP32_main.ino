/**
  * @brief          ESP32端通讯
  * @param[in]      NULL
  * @retval         NULL
  * 
  * x 0-152
  * y o-118
  * 
  */


#include "uart.h"
#include "TJC.h"
#include "BLE.h"

UART STM_UART;
TJC  TJC_UART;
BLE  BLE_UART;

extern Rev_data_t receivedData;

void setup() {

  //GPIO init
  pinMode(2,OUTPUT);
  
  //串口初始化
  STM_UART.setup();
  TJC_UART.setup();
  BLE_UART.setup();

}

void loop() {
  
   digitalWrite(2,HIGH);


  //STM32通讯
   STM_UART.receiveData(receivedData);
  //STM_UART.printData();

  //串口屏通讯
  TJC_UART.sendData();

  //键鼠操控
  BLE_UART.sendData();
 
  
  //udelay(10);
  delay(20);
}
