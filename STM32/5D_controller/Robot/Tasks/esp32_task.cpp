#include "esp32_task.h"
#include "cmsis_os.h"
#include "usart.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "moveit_task.h"
#include "hipnuc_task.h"



Send_data_t Send_data;
Rev_deta_t  Rev_data;

void esp32_task(void const * argument)
{
	vTaskDelay(10);
	
	while(1)
	{

		SEND_ESP(1);

		vTaskDelay(50);
	}


}

/**
	* @brief   STM32发送给ESP32
	* @note    
*/

void SEND_ESP(uint8_t cmdID)
{	
	uint8_t stm32_send_pack[100]={0};

	Send_data.head=SEND_HEAD;
	Send_data.cmdID=cmdID;

	Send_data.end_positionX=get_moveit_point()->end_positionX;
	Send_data.end_positionY=get_moveit_point()->end_positionY;
	Send_data.end_speedX=get_moveit_point()->end_speedX;
	Send_data.end_speedY=get_moveit_point()->end_speedY;

	Send_data.yaw=get_imu_point()->hi91.yaw;
	Send_data.pitch=get_imu_point()->hi91.roll;
	Send_data.roll=get_imu_point()->hi91.pitch;

	Send_data.gyro[0]=get_imu_point()->hi91.gyr[0];
	Send_data.gyro[1]=get_imu_point()->hi91.gyr[1];
	Send_data.gyro[2]=get_imu_point()->hi91.gyr[2];

	Send_data.accel[0]=get_imu_point()->hi91.acc[0];
	Send_data.accel[1]=get_imu_point()->hi91.acc[1];
	Send_data.accel[2]=get_imu_point()->hi91.acc[2];

	Send_data.quat[0]=get_imu_point()->hi91.quat[0];
	Send_data.quat[1]=get_imu_point()->hi91.quat[1];
	Send_data.quat[2]=get_imu_point()->hi91.quat[2];
	Send_data.quat[3]=get_imu_point()->hi91.quat[3];

	Send_data.F_button[0]=get_moveit_point()->buttons.F1;
	Send_data.F_button[1]=get_moveit_point()->buttons.F2;
	Send_data.F_button[2]=get_moveit_point()->buttons.F3;

	if(get_moveit_point()->buttons.Thr_down) Send_data.Two_button[0]=0;
	else if(get_moveit_point()->buttons.Thr_mid) Send_data.Two_button[0]=1;

	if(get_moveit_point()->buttons.Two_down) Send_data.Two_button[1]=0;
	else if(get_moveit_point()->buttons.Two_up) Send_data.Two_button[1]=1;

	Send_data.encoder=0;

	//Send_data.checksum = calculateChecksum((uint8_t*)&Send_data, 78);

	memcpy(stm32_send_pack,&Send_data,78);
	
	HAL_UART_Transmit(&huart6,(uint8_t *)stm32_send_pack,78,200);

	memset(stm32_send_pack,0,sizeof(Send_data_t));


}

extern "C"
{
    /**
     * @brief   串口6接收中断
     * @note    先后读取SR和DR寄存器后，中断标志位会自动清除
     */
void USART6_IRQHandler(void)
{
	if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE) == SET)
  {
			//读取单个字节
//		ch = USART1->DR;
			//解析函数
//    decode_succ = hipnuc_input(&hipnuc_raw, ch);    
	}
}
}

uint8_t calculateChecksum(const uint8_t* data, uint16_t length) 
{
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}


