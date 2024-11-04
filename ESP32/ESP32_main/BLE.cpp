#include "BLE.h"
#include "uart.h"
#include <Arduino.h> 
#include <HardwareSerial.h>
//#include "BleCombo.h"
#include <BleMouse.h>

BleMouse bleMouse;


extern Rev_data_t receivedData;
uint8_t BLE_Connected;


void BLE::setup()
{
//  Keyboard.begin();
  bleMouse.begin();
}


bool BLE::BleConntect()
{
    if(bleMouse.isConnected())
    {
      //指示灯    
      digitalWrite(2,HIGH);
      BLE_Connected=1;
    }
    else
    {
      digitalWrite(2,LOW);
      BLE_Connected=0;
    }
}

//bool BLE::sendKeyData()
//{
//
//  
//// 检查键盘是否连接
//  if(Keyboard.isConnected()) {
//
//    if(receivedData.F_button[0]){
//      if(receivedData.end_positionX<50)
//      {
//        Keyboard.press('d');
//      }
//      if(receivedData.end_positionX>100)
//      {
//        Keyboard.press('a');
//      }
//      if(receivedData.end_positionY>80)
//      {
//        Keyboard.press('s');
//      
//      }
//      if(receivedData.end_positionY<40)
//      {
//        Keyboard.press('w');
//      }
//      
//    }
//    else if(receivedData.F_button[1])
//    {
//       Keyboard.press('e');
//    }    
//    else if(receivedData.F_button[2])
//    {
//       Keyboard.press('q');
//    }    
//    
//    else
//    {
//      Keyboard.releaseAll();
//    }
//
//
//  }
//
//}

bool BLE::sendMouseData()
{

// 检查蓝牙是否连接
  if(bleMouse.isConnected()) 
  {
    if(receivedData.F_button[0])
    {
      if(receivedData.yaw>10)
      {
        bleMouse.move(-(int8_t)receivedData.yaw/10,0);
      }
      if(receivedData.yaw<-10)
      {
        bleMouse.move(-(int8_t)receivedData.yaw/10,0);
      }
      if(receivedData.pitch>20)
      {
        bleMouse.move(0,(int8_t)receivedData.pitch/10);
      
      }
      if(receivedData.pitch<-20)
      {
        bleMouse.move(0,(int8_t)receivedData.pitch/10,0);
      }
      delay(5);
    } 

    else if(receivedData.F_button[1])
    {
       bleMouse.click(MOUSE_LEFT);
       delay(50);
    }


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
