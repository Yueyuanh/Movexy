#ifndef ESP32_TASK
#define ESP32_TASK

#include "struct_typedef.h"

#define SEND_LENGTH   100
#define SEND_LEN_HEAD 3
#define SEND_LEN_DATA 80

#define SEND_HEAD     (0xA5)




#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	uint8_t head;
	uint8_t cmdID;
	
	
}Rev_deta_t;

typedef struct
{
	uint8_t head;
	uint8_t cmdID;

	//xy信息
	float end_positionX;
	float end_positionY;
	float end_speedX;
	float end_speedY;//16

	//陀螺仪信息
	float yaw,pitch,roll;
	float accel[3];	//xyz
	float gyro[3];	//xyz//36
	float quat[4];	//wxyz//16

	//按键信息
	uint8_t F_button[3];//F1 F2 F3
	uint8_t Two_button[2];//0↓ 1↑
	uint8_t encoder;//6
//78
	//uint8_t checksum; // 校验和
}Send_data_t;



#ifdef __cplusplus




#endif




void esp32_task(void const * argument);
void SEND_ESP(uint8_t cmdID);
uint8_t calculateChecksum(const uint8_t* data, uint16_t length);


#ifdef __cplusplus
}
#endif

#endif
