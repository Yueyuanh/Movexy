#ifndef MOVEIT_TASK_H
#define MOVEIT_TASK_H

//include 
#include "struct_typedef.h"
#include "can_receive.h"
#include "pid.h"


//define 
#define MOVEIT_TASK_INIT_TIME 3000
#define PI 3.1415926f


#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }	


#ifdef __cplusplus
extern "C"{
#endif
#ifdef __cplusplus



//struct
typedef struct 
{
	int16_t motor_ecd_offset;
	int16_t motor_ecd;
	fp32    motor_angle;
	fp32    motor_angle_rad;
	fp32    motor_speed;

}motor_data;

typedef struct 
{
	uint8_t F1;
	uint8_t F2;
	uint8_t F3;

	uint8_t Thr_up;
	uint8_t Thr_mid;
	uint8_t Thr_down;

	uint8_t Two_up;
	uint8_t Two_down;

}button_data;



//class
class moveit_t
{
private:

	//电机数据结构体
  const motor_measure_t *move_motor_measure[2];


	
public:

	//初始化
	uint8_t init_flag;
	int init_count;
	int16_t motor_send_current[2];


	motor_data motor_data[2];


	fp32 end_positionX,end_positionY;
	fp32 end_speedX,end_speedY;
	button_data buttons;


	
	uint8_t control_mode;
	fp32 end_L_set;
	fp32 end_Tp_set;

	moveit_t();
	void moveit_init();
	void moveit_update();
	void moveit_mode_set();
	void moveit_control();

		
};

const moveit_t *get_moveit_point(void);

#endif





void moveit_task(void const * argument);

extern const motor_measure_t *get_chassis_motor_measure_point(int i);
extern void CAN_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);



#ifdef __cplusplus
}
#endif
#endif
