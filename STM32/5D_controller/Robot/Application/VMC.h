#ifndef VMC_H
#define VMC_H

#include "struct_typedef.h"
#include "pid.h"

#define pi 3.1415926f
#define rad2angle 57.3f
#define angle2rad 1/57.3f

#define LEG_1 120.0f
#define LEG_2 130.0f
#define LEG_0 66.0f




#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus

class VMC_t
{
		public:

    PID_t F_PD,Tp_PD;
		float STAND_FEED;
		PID_t JUMP_F_PD,JUMP_Tp_PD;
		float JUMP_FEED;
		

		//定义实际模型参数
    float L1,L2,L3,L4,L5;
  
    //定义中间变量
    float Phi1,Phi2,Phi3,Phi4;
    float A0,B0,C0,L_BD;
    float xB,xD,yB,yD;
    
    float xC,yC;
    

    //逆运动学变量
    float xA,yA;
    float xC_set,yC_set;
    float a0,b0,c0;
    float Phi1_set,Phi4_set;
    float Phi2_set,Phi3_set;
    float motor_speed_1,motor_speed_4;

		//定义输入量
    float F,Tp;

    //观测器结构体


    // 定义输出变量
    float L0,Phi0;
		float end_x,end_y;
    float L0_dot, Phi0_gyro;

    //定义雅可比矩阵
    double J[2][2];
   
    //定义输出量
    float T[2];

    //逆解算求出足端返回扭矩
    double inv_J[2][2];
    double F_foot;
};






#endif

void VMC_init();
void VMC_calc(float Ld_R, float Ad_R, float Ld_L, float Ad_L);
void Forward_kinematics_R(fp32 Q1, fp32 Q4, fp32 S1, fp32 S4, fp32 A1, fp32 A4);
void Forward_kinematics_L(fp32 Q1, fp32 Q4, fp32 S1, fp32 S4, fp32 A1, fp32 A4);
void inverse_kinematics(fp32 angle, fp32 high); 





#ifdef __cplusplus
}
#endif



#endif
