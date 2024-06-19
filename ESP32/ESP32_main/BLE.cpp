#include "BLE.h"
#include "uart.h"
#include <Arduino.h> 
#include <HardwareSerial.h>
#include <BleKeyboard.h>
#include <BleMouse.h>
#include <WiFi.h>


BleKeyboard bleKeyboard("Movexy","Yyh",100);
//BleMouse bleMouse;


extern Rev_data_t receivedData;
uint8_t BLE_Connected;

void BLE::setup()
{
  bleKeyboard.begin();
  //bleMouse.begin();
}

bool BLE::sendKeyData()
{

  
// 检查键盘是否连接
  if(bleKeyboard.isConnected()) {

    //指示灯    
    digitalWrite(2,HIGH);
    BLE_Connected=1;
    
    if(receivedData.F_button[0]){
      if(receivedData.end_positionX<50)
      {
        bleKeyboard.press('d');
      }
      if(receivedData.end_positionX>100)
      {
        bleKeyboard.press('a');
      }
      if(receivedData.end_positionY>80)
      {
        bleKeyboard.press('s');
      
      }
      if(receivedData.end_positionY<40)
      {
        bleKeyboard.press('w');
      }
      
    }
    else if(receivedData.F_button[1])
    {
       bleKeyboard.press('e');
    }    
    else if(receivedData.F_button[2])
    {
       bleKeyboard.press('q');
    }    
    
    else
    {
      bleKeyboard.releaseAll();
    }


  }
  else
  {
    digitalWrite(2,LOW);
    BLE_Connected=0;
  }

}

//
//bool BLE::sendMouseData()
//{
//
//// 检查键盘是否连接
//  if(bleKeyboard.isConnected()) {
//   
//    if(receivedData.F_button[0]){
//      if(receivedData.yaw>10)
//      {
//        bleMouse.move(0,-(int)receivedData.yaw);
//      }
//      if(receivedData.yaw<-10)
//      {
//        bleMouse.move(0,-(int)receivedData.yaw);
//      }
//      if(receivedData.pitch>20)
//      {
//        bleMouse.move(-(int)receivedData.pitch,0);
//      
//      }
//      if(receivedData.pitch<-20)
//      {
//        bleMouse.move(-(int)receivedData.pitch,0);
//      }
//      
//    } 
//
//
//  }
//
//  
//  }


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
