#include "TJC.h"
#include "uart.h"
#include <Arduino.h> 
#include <HardwareSerial.h>
#include "string.h"


HardwareSerial Serial2TJC(2);
Rev_data_t TJC_movexy_data;
extern Rev_data_t receivedData;

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
    Serial.println(receivedData.end_positionX);
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
        
    
  
}
