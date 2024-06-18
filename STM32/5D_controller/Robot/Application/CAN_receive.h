#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "struct_typedef.h"

#define CHASSIS_CAN hcan1
#define GIMBAL_CAN hcan2

#ifdef __cplusplus
extern "C"{
#endif
/* CAN send and receive ID */
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x1ff,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,

		CAN_MF9025_ALL_ID = 0x140,
		CAN_MF9025_1_ID = 0x141,
		CAN_MF9025_2_ID = 0x142,
	
    CAN_YAW_MOTOR_ID = 0x205,
    CAN_PIT_MOTOR_ID = 0x206,
    CAN_TRIGGER_MOTOR_ID = 0x207,
    CAN_GIMBAL_ALL_ID = 0x200,

    SEND_TO_GIMBAL_ID = 0x726,

		CAN_LOGO =	0x713,
} can_msg_id_e;

//rm motor data
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    uint16_t last_ecd;
		int16_t num;
} motor_measure_t;

typedef struct
{
    int16_t anglePidKp;			//位置环 P 参数
    int16_t anglePidKi;			//位置环 I 参数
    int16_t speedPidKp;			//速度环 P 参数 
    int16_t speedPidKi;			//速度环 I 参数 
    int16_t iqPidKp;				//转矩环 P 参数 
		int16_t iqPidKi;				//转矩环 I 参数 
} mf3508_PI_feedback;

typedef struct
{
		int8_t temperature;
		uint16_t voltage;
		uint8_t error_flag;
}	mf9025_error_read;




typedef struct
{
	fp32 vx_set;
  fp32 wz_set;
  bool_t move_enable;
	bool_t reset_enable;
	bool_t x_y_displacement_judgment;
	bool_t Motor_start_mark;
	float Instantaneous_power;

	
	int chassis_heat_MAX;
	int residual_joule_energy;
	
	
	bool_t save_logo;
	bool_t spin_logo;
	bool_t low_dip_mark;
	
	char chassis_outof_control;
	char gimbal_update_flag;
	
	
} Gimbal_order_t;

#ifdef __cplusplus
}     
#endif
/**
  * @brief          send control current of motor (0x205, 0x206, 0x207, 0x208)
  * @param[in]      yaw: (0x205) 6020 motor control current, range [-30000,30000] 
  * @param[in]      pitch: (0x206) 6020 motor control current, range [-30000,30000]
  * @param[in]      shoot: (0x207) 2006 motor control current, range [-10000,10000]
  * @param[in]      rev: (0x208) reserve motor control current
  * @retval         none
  */
/**
  * @brief          发送电机控制电流(0x205,0x206,0x207,0x208)
  * @param[in]      yaw: (0x205) 6020电机控制电流, 范围 [-30000,30000]
  * @param[in]      pitch: (0x206) 6020电机控制电流, 范围 [-30000,30000]
  * @param[in]      shoot: (0x207) 2006电机控制电流, 范围 [-10000,10000]
  * @param[in]      rev: (0x208) 保留，电机控制电流
  * @retval         none
  */
extern void CAN_cmd_gimbal(int16_t yaw, int16_t pitch, int16_t shoot, int16_t rev);

/**
  * @brief          send CAN packet of ID 0x700, it will set chassis motor 3508 to quick ID setting
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          发送ID为0x700的CAN包,它会设置3508电机进入快速设置ID
  * @param[in]      none
  * @retval         none
  */
extern void CAN_cmd_chassis_reset_ID(void);

/**
  * @brief          send control current of motor (0x201, 0x202, 0x203, 0x204)
  * @param[in]      motor1: (0x201) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor2: (0x202) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor3: (0x203) 3508 motor control current, range [-16384,16384] 
  * @param[in]      motor4: (0x204) 3508 motor control current, range [-16384,16384] 
  * @retval         none
  */
/**
  * @brief          发送电机控制电流(0x201,0x202,0x203,0x204)
  * @param[in]      motor1: (0x201) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor2: (0x202) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor3: (0x203) 3508电机控制电流, 范围 [-16384,16384]
  * @param[in]      motor4: (0x204) 3508电机控制电流, 范围 [-16384,16384]
  * @retval         none
  */

extern void CAN_cmd_set_current_loop(void);

/**
  * @brief          返回底盘电机 电机数据指针
  * @param[in]     
  * @retval         电机数据指针
  */
extern void CAN_mf9025_read_motor_error(void);
extern void CAN_mf9025_read_motor_error1(void);

//extern const motor_measure_t *get_chassis_motor_measure_point(int i);
//extern const motor_measure_t *get_mf9025_chassis_motor_measure_point(int i);
extern void CAN_mf9025_cmd_chassis(int16_t motor1, int16_t motor2);
//extern void CAN_mf9025_cmd_chassis_Torque_control_2(int16_t motor1);
//extern void CAN_mf9025_cmd_chassis_Torque_control_1(int16_t motor1);
//extern void CAN_mf9025_cmd_chassis_Torque_control_R(int32_t motor1);
//extern void CAN_mf9025_cmd_chassis_Torque_control_L(int32_t motor1);
extern void CAN_mf9025_cmd_chassis_L(void);
extern void CAN_mf9025_cmd_chassis_R(void);
extern void CAN_mf9025_read_motor_status1(void);
extern void CAN_mf9025_read_motor_status2(void);
extern void CAN_mf9025_cmd_chassis_PID_readin_R(void);
extern void CAN_mf9025_cmd_chassis_PID_readin_L(void);
extern const Gimbal_order_t *get_gimbal_order_point(void);
extern void chassis_protect(void);
extern void CAN_cmd_gimbal_command_0x714(float chassis_dip_Angle);


//extern void CAN_SEND_TO_GIMBAL(int16_t pitch_angle_set);

#endif
