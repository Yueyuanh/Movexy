#include "BLE.h"
#include "uart.h"
#include <Arduino.h> 
#include <HardwareSerial.h>
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <WiFi.h>


BleKeyboard bleKeyboard("Movexy","Yyh",100);
//BleMouse bleMouse("Movexy","Yyh",100);


extern Rev_data_t receivedData;


void BLE::setup()
{
  bleKeyboard.begin();
  //bleMouse.begin();
}

bool BLE::sendData()
{
// 检查键盘是否连接
  if(bleKeyboard.isConnected()) {
    if(receivedData.F_button[0]){
   
      if(receivedData.end_positionX<50)
      {
        Serial.println("D");
        bleKeyboard.print("d");
        //bleMouse.move(0,1000);
      
      }
      if(receivedData.end_positionX>100)
      {
         Serial.println("A");
        bleKeyboard.print("a");
     
      }
      if(receivedData.end_positionY>80)
      {
        Serial.println("S");
        bleKeyboard.print("s");
      
      }
      if(receivedData.end_positionY<40)
      {
        Serial.println("W");
        bleKeyboard.print("w");

      }
    }


   bleKeyboard.release(1);
  }

}





static __inline void delay_clock(int ts)
{
    uint32_t start, curr;
 
    __asm__ __volatile__("rsr %0, ccount" : "=r"(start));
    do
    {
        __asm__ __volatile__("rsr %0, ccount" : "=r"(curr));
        
    }while (curr - start <= ts);
}
 
void udelay(int us)
{
    while (us--)
    {
        delay_clock(160);//CPU_Freq=160MHz
    }
}
 
void mdelay(int ms)
{
    while (ms--)
    {
        delay_clock(160*1000);//CPU_Freq=160MHz
    }
}
