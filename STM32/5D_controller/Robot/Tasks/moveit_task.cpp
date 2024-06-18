#include "moveit_task.h"
#include "cmsis_os.h"
#include "GPIO.h"
#include "VMC.h"


moveit_t movexy;
extern VMC_t VMC_LEG_R;



/**
	* @brief          �˶��๹�캯��
  * @param[in]      NULL
  * @retval         NULL
  */
moveit_t::moveit_t()
{
	//���ָ���ʼ��
	move_motor_measure[0]=get_chassis_motor_measure_point(0);
	move_motor_measure[1]=get_chassis_motor_measure_point(1);

	//��ʼ��
	init_flag=0;
	init_count=0;
}


/**
	* @brief          �˶���ʼ��
  * @param[in]      NULL
  * @retval         NULL
  */
void moveit_t::moveit_init()
{
	init_flag=0;

	//�����λ��ʼ��	

	if(init_count++<1000&&!init_flag) motor_send_current[0]=6000;
	else 
	{
		init_count=motor_send_current[0]=0;
		init_flag=1;
	}

	motor_data[0].motor_ecd_offset=move_motor_measure[0]->ecd;
	motor_data[1].motor_ecd_offset=move_motor_measure[1]->ecd;


}


/**
	* @brief          �˶�������
  * @param[in]      NULL
  * @retval         NULL
  */
void moveit_task(void const * argument)
{
	vTaskDelay(MOVEIT_TASK_INIT_TIME);
	movexy.moveit_init();

	while(1)
	{
		if(!movexy.init_flag) movexy.moveit_init();
		
		//���ݸ���
		movexy.moveit_update();

		CAN_cmd_chassis(movexy.motor_send_current[0],movexy.motor_send_current[1],0,0);
		vTaskDelay(2);
	}
}

void moveit_t::moveit_update()
{

	//��ȡ�������
	motor_data[0].motor_ecd=move_motor_measure[0]->ecd;
	motor_data[1].motor_ecd=move_motor_measure[1]->ecd;

	motor_data[0].motor_angle=(move_motor_measure[0]->ecd-motor_data[0].motor_ecd_offset)/8191.0f*360.0f;
	motor_data[1].motor_angle=(move_motor_measure[1]->ecd-motor_data[1].motor_ecd_offset)/8191.0f*360.0f;

	motor_data[0].motor_angle+=191;
	motor_data[1].motor_angle+=85;


	motor_data[0].motor_angle_rad=motor_data[0].motor_angle*angle2rad;
	motor_data[1].motor_angle_rad=motor_data[1].motor_angle*angle2rad;

	motor_data[0].motor_speed=move_motor_measure[0]->speed_rpm;
  motor_data[1].motor_speed=move_motor_measure[1]->speed_rpm;

	//�˶�ѧ����
	Forward_kinematics_R(motor_data[0].motor_angle_rad,motor_data[1].motor_angle_rad,
                       motor_data[0].motor_speed,    motor_data[1].motor_speed,0,0);

	end_positionX=VMC_LEG_R.end_x+84;
	end_positionY=VMC_LEG_R.end_y-88;

	//������Ϣ��ȡ
	//�ֱ�����
	movexy.buttons.F1=!HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13);
	movexy.buttons.F2=!HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11);
	movexy.buttons.F3=!HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_14);

	//��������
	movexy.buttons.Thr_down=!HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_6);
//	movexy.buttons.Thr_up=!(movexy.buttons.Thr_down||movexy.buttons.Thr_mid);
	if(movexy.buttons.Thr_down) movexy.buttons.Thr_mid=0;
	else movexy.buttons.Thr_mid=1;

	//��������
	movexy.buttons.Two_up=!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6);
	movexy.buttons.Two_down=!movexy.buttons.Two_up;


}


/**
  * @brief          ��ȡ������Ϣ
  * @param[in]      NULL
  * @retval         NULL
  */
const moveit_t *get_moveit_point(void)
{
    return &movexy;
}
