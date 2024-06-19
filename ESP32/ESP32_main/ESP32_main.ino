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

//多线程
TaskHandle_t Task1;
TaskHandle_t Task2;
void Task1code(void * pvParameters);
void Task2code(void * pvParameters);


extern Rev_data_t receivedData;

void setup() {

  //GPIO init
  pinMode(2,OUTPUT);

  
  //串口初始化
  STM_UART.setup();
  TJC_UART.setup();
  BLE_UART.setup();

  
 //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}



void Task1code( void * pvParameters)
{

  
  while(1)
  {

    
    //STM32通讯
    STM_UART.receiveData(receivedData);
    //STM_UART.printData();

    //串口屏通讯
    TJC_UART.sendData();
    
    //任务1延时
    delay(20);
  }
}


void Task2code(void * pvParameters)
{

  
  while(1)
  {
    //键鼠操控
    BLE_UART.sendKeyData();
    //BLE_UART.sendMouseData();

    delay(10);
  }
}












void loop() {}
