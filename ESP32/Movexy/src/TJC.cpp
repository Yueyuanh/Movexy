#include "TJC.h"
#include "uart.h"
#include "BLE.h"
#include <Arduino.h> 
#include <HardwareSerial.h>
#include "string.h"


HardwareSerial Serial2TJC(2);
Rev_data_t TJC_movexy_data;
extern Rev_data_t receivedData;
extern uint8_t BLE_Connected;
extern uint8_t STM_Connected;

int blue=1048;
int red=63488;
int white=48631;

void TJC::setup()
{
    Serial2TJC.begin(115200,SERIAL_8N1,16,17);//R T
    TJC_movexy_data=*get_movexy_data();
  }

bool TJC::sendData()
{
    char str[100];

    /*************************************************数字显示***************************************************/
    //用sprintf来格式化字符串，给n0的val属性赋值
    sprintf(str, "movestate.t1.txt=\"x:%d\"\xff\xff\xff",(int)receivedData.end_positionX);
    //把字符串发送出去
    Serial2TJC.print(str);
    
    sprintf(str, "movestate.t2.txt=\"y:%d\"\xff\xff\xff",(int)receivedData.end_positionY);
    //把字符串发送出去
    Serial2TJC.print(str);

    sprintf(str, "movestate.t3.txt=\"yaw:%.2f\"\xff\xff\xff",receivedData.yaw);
    //把字符串发送出去
    Serial2TJC.print(str);
    
    sprintf(str, "movestate.t4.txt=\"pit:%.2f\"\xff\xff\xff",receivedData.pitch);
    //把字符串发送出去
    Serial2TJC.print(str);
    
    sprintf(str, "movestate.t5.txt=\"rol:%.2f\"\xff\xff\xff",receivedData.roll);
    //把字符串发送出去
    Serial2TJC.print(str);
    
    /*************************************************坐标可视化显示***************************************************/

    sprintf(str, "movestate.j1.val=%d\xff\xff\xff",(int)((1-(receivedData.end_positionX/170))*100));
    //把字符串发送出去
    Serial2TJC.print(str);
    
    sprintf(str, "movestate.j0.val=%d\xff\xff\xff",(int)((1-(receivedData.end_positionY/130))*100));
    //把字符串发送出去
    Serial2TJC.print(str);
    /*************************************************陀螺仪可视化显示***************************************************/
    //yaw -179 180
    sprintf(str, "movestate.j4.val=%d\xff\xff\xff",(int)((receivedData.yaw+180)/360*100));
    Serial2TJC.print(str);

    //pitch -57  145
    sprintf(str, "movestate.j5.val=%d\xff\xff\xff",(int)((receivedData.pitch+150)/300*100));
    Serial2TJC.print(str);

    //roll 39 -42
    sprintf(str, "movestate.j6.val=%d\xff\xff\xff",(int)(receivedData.roll+50));
    Serial2TJC.print(str);
    



    /*************************************************按键可视化显示***************************************************/
    sprintf(str, "movestate.bt0.val=%d\xff\xff\xff",(int)receivedData.F_button[0]);
    //把字符串发送出去
    Serial2TJC.print(str);
    
    sprintf(str, "movestate.bt1.val=%d\xff\xff\xff",(int)receivedData.F_button[1]);
    //把字符串发送出去
    Serial2TJC.print(str);

    sprintf(str, "movestate.bt2.val=%d\xff\xff\xff",(int)receivedData.F_button[2]);
    //把字符串发送出去
    Serial2TJC.print(str);

    //
    if(receivedData.Two_button[0])
    {
       sprintf(str, "movestate.j2.bco=%d\xff\xff\xff",blue);
       Serial2TJC.print(str);
       sprintf(str, "movestate.j2.pco=%d\xff\xff\xff",white);
       Serial2TJC.print(str);
    }
    else
    {
       sprintf(str, "movestate.j2.bco=%d\xff\xff\xff",white);
       Serial2TJC.print(str);
       sprintf(str, "movestate.j2.pco=%d\xff\xff\xff",blue);
       Serial2TJC.print(str);
    }

    if(receivedData.Two_button[1])
    {
       sprintf(str, "movestate.j3.bco=%d\xff\xff\xff",red);
       Serial2TJC.print(str);
       sprintf(str, "movestate.j3.pco=%d\xff\xff\xff",white);
       Serial2TJC.print(str);
    }
    else
    {
       sprintf(str, "movestate.j3.bco=%d\xff\xff\xff",white);
       Serial2TJC.print(str);
       sprintf(str, "movestate.j3.pco=%d\xff\xff\xff",red);
       Serial2TJC.print(str);
    }
/*************************************************蓝牙在线显示***************************************************/
    if(BLE_Connected)
    {
       sprintf(str, "main.bt2.val=%d\xff\xff\xff",1);
       Serial2TJC.print(str);
    }
    else
    {
       sprintf(str, "main.bt2.val=%d\xff\xff\xff",0);
       Serial2TJC.print(str);
    }
/*************************************************STM32在线显示***************************************************/
    if(STM_Connected)
    {
       sprintf(str, "main.bt0.val=%d\xff\xff\xff",1);
       Serial2TJC.print(str);
       sprintf(str, "main.bt1.val=%d\xff\xff\xff",1);
       Serial2TJC.print(str);
    }
    else
    {
       sprintf(str, "main.bt0.val=%d\xff\xff\xff",0);
       Serial2TJC.print(str);
       sprintf(str, "main.bt1.val=%d\xff\xff\xff",0);
       Serial2TJC.print(str);
    }
  
}
bool TJC::receiveData()
{
//串口数据格式：
   //串口数据帧长度：7字节
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     1字节   1字节    1字节     0xffffff
   //当参数是01时
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     01     led编号  led状态    0xffffff
   //例子1：上位机代码  printh 55 01 01 00 ff ff ff  含义：1号led关闭
   //例子2：上位机代码  printh 55 01 04 01 ff ff ff  含义：4号led打开
   //例子3：上位机代码  printh 55 01 00 01 ff ff ff  含义：0号led打开
   //例子4：上位机代码  printh 55 01 04 00 ff ff ff  含义：4号led关闭

   //当参数是02或03时
   //帧头     参数1    参数2   参数3       帧尾
   //0x55     02/03   滑动值    00    0xffffff
   //例子1：上位机代码  printh 55 02 64 00 ff ff ff  含义：h0.val=100
   //例子2：上位机代码  printh 55 02 00 00 ff ff ff  含义：h0.val=0
   //例子3：上位机代码  printh 55 03 64 00 ff ff ff  含义：h1.val=100
   //例子4：上位机代码  printh 55 03 00 00 ff ff ff  含义：h1.val=0


   //当串口缓冲区大于等于6时
   while (Serial2TJC.available() >= FRAME_LENGTH) {
     unsigned char ubuffer[FRAME_LENGTH];
     //从串口缓冲读取1个字节但不删除
     unsigned char frame_header = Serial2TJC.peek();
     //当获取的数据是包头(0x55)时
     if (frame_header == 0x55) {
       //从串口缓冲区读取7字节
       Serial2TJC.readBytes(ubuffer, FRAME_LENGTH);
       if (ubuffer[4] == 0xff && ubuffer[5] == 0xff && ubuffer[6] == 0xff) //帧尾校验
       {
//         if(ubuffer[1] == 0x01)
//         {
//
//         }
        }
        else {
        Serial2TJC.read();  //从串口缓冲读取1个字节并删除
      }
   }

  
   }
  }
