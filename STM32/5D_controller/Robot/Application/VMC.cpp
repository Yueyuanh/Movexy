#include "arm_math.h"
#include "VMC.h"



VMC_t VMC_LEG_R,VMC_LEG_L;



void VMC_init()
{

		VMC_LEG_R.L1 = LEG_1;
    VMC_LEG_R.L2 = LEG_2;
    VMC_LEG_R.L3 = LEG_2;
    VMC_LEG_R.L4 = LEG_1;
    VMC_LEG_R.L5 = LEG_0;

    VMC_LEG_L.L1 = LEG_1;
    VMC_LEG_L.L2 = LEG_2;
    VMC_LEG_L.L3 = LEG_2;
    VMC_LEG_L.L4 = LEG_1;
    VMC_LEG_L.L5 = LEG_0;


/*********************************************************************************************************/
		//stand
    const fp32 F_PD_R[3]= {2,0.001,100};
    const fp32 Tp_PD_R[3]={20000,10,5000};

    
		VMC_LEG_R.F_PD.init(PID_POSITION, F_PD_R, 120, 50);
    VMC_LEG_R.Tp_PD.init(PID_POSITION,Tp_PD_R,6500, 4000);


/*********************************************************************************************************/
		//jump
		const fp32 JUMP_F_PD_R[3]={400,0,100};
    const fp32 JUMP_Tp_PD_R[3]={30,0,500};



    VMC_LEG_R.JUMP_F_PD.init(PID_POSITION, JUMP_F_PD_R, 20, 10);
    VMC_LEG_R.JUMP_Tp_PD.init(PID_POSITION,JUMP_Tp_PD_R,20, 0);
}

void VMC_calc(float Ld_R, float Ad_R, float Ld_L, float Ad_L)
{

		//期望位置
		VMC_LEG_R.F=  VMC_LEG_R.F_PD.calc(VMC_LEG_R.L0-90,Ld_R);
		VMC_LEG_R.Tp= VMC_LEG_R.Tp_PD.calc(VMC_LEG_R.Phi0,Ad_R);
//		VMC_LEG_R.F=  Ld_R;
//		VMC_LEG_R.Tp= Ad_R;


    //雅可比矩阵求解
    VMC_LEG_R.J[0][0] = LEG_1 * arm_sin_f32(VMC_LEG_R.Phi0 - VMC_LEG_R.Phi3) * arm_sin_f32(VMC_LEG_R.Phi1-VMC_LEG_R.Phi2) / arm_sin_f32(VMC_LEG_R.Phi3 - VMC_LEG_R.Phi2);
    VMC_LEG_R.J[0][1] = LEG_1 * arm_cos_f32(VMC_LEG_R.Phi0 - VMC_LEG_R.Phi3) * arm_sin_f32(VMC_LEG_R.Phi1-VMC_LEG_R.Phi2) / (LEG_0*arm_sin_f32(VMC_LEG_R.Phi3 - VMC_LEG_R.Phi2));
    VMC_LEG_R.J[1][0] = LEG_1 * arm_sin_f32(VMC_LEG_R.Phi0 - VMC_LEG_R.Phi2) * arm_sin_f32(VMC_LEG_R.Phi3-VMC_LEG_R.Phi4) / arm_sin_f32(VMC_LEG_R.Phi3 - VMC_LEG_R.Phi2);
    VMC_LEG_R.J[1][1] = LEG_1 * arm_cos_f32(VMC_LEG_R.Phi0 - VMC_LEG_R.Phi2) * arm_sin_f32(VMC_LEG_R.Phi3-VMC_LEG_R.Phi4) / (LEG_0*arm_sin_f32(VMC_LEG_R.Phi3 - VMC_LEG_R.Phi2));

    VMC_LEG_L.J[0][0] = LEG_1 * arm_sin_f32(VMC_LEG_L.Phi0 - VMC_LEG_L.Phi3) * arm_sin_f32(VMC_LEG_L.Phi1-VMC_LEG_L.Phi2) / arm_sin_f32(VMC_LEG_L.Phi3 - VMC_LEG_L.Phi2);
    VMC_LEG_L.J[0][1] = LEG_1 * arm_cos_f32(VMC_LEG_L.Phi0 - VMC_LEG_L.Phi3) * arm_sin_f32(VMC_LEG_L.Phi1-VMC_LEG_L.Phi2) / (LEG_0*arm_sin_f32(VMC_LEG_L.Phi3 - VMC_LEG_L.Phi2));
    VMC_LEG_L.J[1][0] = LEG_1 * arm_sin_f32(VMC_LEG_L.Phi0 - VMC_LEG_L.Phi2) * arm_sin_f32(VMC_LEG_L.Phi3-VMC_LEG_L.Phi4) / arm_sin_f32(VMC_LEG_L.Phi3 - VMC_LEG_L.Phi2);
    VMC_LEG_L.J[1][1] = LEG_1 * arm_cos_f32(VMC_LEG_L.Phi0 - VMC_LEG_L.Phi2) * arm_sin_f32(VMC_LEG_L.Phi3-VMC_LEG_L.Phi4) / (LEG_0*arm_sin_f32(VMC_LEG_L.Phi3 - VMC_LEG_L.Phi2));

    //计算关节电机输出扭矩
    VMC_LEG_R.T[0]=VMC_LEG_R.J[0][0]*VMC_LEG_R.F+VMC_LEG_R.J[0][1]*VMC_LEG_R.Tp;
    VMC_LEG_R.T[1]=VMC_LEG_R.J[1][0]*VMC_LEG_R.F+VMC_LEG_R.J[1][1]*VMC_LEG_R.Tp;

}



// 正运动学解算
//Q是角度，S是速度，A是加速度
void Forward_kinematics_R(fp32 Q1, fp32 Q4, fp32 S1, fp32 S4, fp32 A1, fp32 A4)
{

    fp32 L0 = 0, Q0 = 0;
    fp32 xb, xd, yb, yd, Lbd, xc, yc;
    fp32 A0, B0, C0, Q2, Q3, S2;
    fp32 vxb, vxd, vyb, vyd, vxc, vyc;
    fp32 cos_Q1, cos_Q4, sin_Q1, sin_Q4;
    fp32 S0;
    // fp32 sin_Q2, S3,cos_Q2, sin_Q3, cos_Q3;
    // fp32 axb, ayb, axd, ayd, a2, axc;
    /******************************/
    //Q1 = pi + Q1;  //这里是对真实电机的反馈角度处理
    cos_Q1 = arm_cos_f32(Q1);
    sin_Q1 = arm_sin_f32(Q1);
    cos_Q4 = arm_cos_f32(Q4);
    sin_Q4 = arm_sin_f32(Q4);
    xb = -LEG_0 / 2 + LEG_1 * cos_Q1;
    xd =  LEG_0 / 2 + LEG_1 * cos_Q4;
    yb =  LEG_1 * sin_Q1;
    yd =  LEG_1 * sin_Q4;

    // arm_sqrt_f32((xd-xb)*(xd-xb)+(yd-yb)*(yd-yb),&Lbd);
    Lbd = sqrt((xd - xb) * (xd - xb) + (yd - yb) * (yd - yb));
    A0 = 2 * LEG_2 * (xd - xb);
    B0 = 2 * LEG_2 * (yd - yb);
    C0 = LEG_2 * LEG_2 + Lbd * Lbd - LEG_2 * LEG_2;
    Q2 = 2 * atan((B0 + sqrt(A0 * A0 + B0 * B0 - C0 * C0)) / (A0 + C0));

    xc = xb + arm_cos_f32(Q2) * LEG_2;
    yc = yb + arm_sin_f32(Q2) * LEG_2;
    //	arm_sqrt_f32(xc*xc +yc*yc,&L0);
    L0 = sqrt(xc * xc + yc * yc);
    Q0 = atan(yc / xc);
    
    //以下为速度求解
    vxb = -S1 * LEG_1 * sin_Q1;
    vyb =  S1 * LEG_1 * cos_Q1;
    vxd = -S4 * LEG_1 * sin_Q4;
    vyd =  S4 * LEG_1 * cos_Q4;
    Q3 = atan((yc - yd) / (xc - xd));
    S2 = ((vxd - vxb) * arm_cos_f32(Q3) + (vyd - vyb) * arm_sin_f32(Q3)) / (LEG_2 * arm_sin_f32(Q3 - Q2));
    //S3 = ((vxd - vxb) * cos(Q2) + (vyd - vyb) * sin(Q2)) / (LEG_2 * sin(Q3 - Q2));
    vxc = vxb - S2 * LEG_2 * arm_sin_f32(Q2);
    vyc = vyb + S2 * LEG_2 * arm_cos_f32(Q2);
    S0 = 3 * (-arm_sin_f32(abs(Q0)) * vxc - arm_cos_f32(Q0) * vyc);

    if(Q0<0) Q0+=pi;

    VMC_LEG_R.L0 = L0;
    VMC_LEG_R.Phi0 = Q0;
    VMC_LEG_R.Phi0_gyro=S0;

    VMC_LEG_R.Phi2 = Q2;
    VMC_LEG_R.Phi3 = Q3;

		VMC_LEG_R.end_y=L0*sin(Q0);
		VMC_LEG_R.end_x=L0*cos(Q0);


}


void Forward_kinematics_L(fp32 Q1, fp32 Q4, fp32 S1, fp32 S4, fp32 A1, fp32 A4)
{

    fp32 L0 = 0, Q0 = 0;
    fp32 xb, xd, yb, yd, Lbd, xc, yc;
    fp32 A0, B0, C0, Q2, Q3, S2;
    fp32 vxb, vxd, vyb, vyd, vxc, vyc;
    fp32 cos_Q1, cos_Q4, sin_Q1, sin_Q4;
    fp32 S0;
    // fp32 dL0,S0, S3
    // fp32 sin_Q2, cos_Q2, sin_Q3, cos_Q3;
    // fp32 axb, ayb, axd, ayd, a2, axc;
    /******************************/
    //Q1 = pi + Q1;  //这里是对真实电机的反馈角度处理
    cos_Q1 = arm_cos_f32(Q1);
    sin_Q1 = arm_sin_f32(Q1);
    cos_Q4 = arm_cos_f32(Q4);
    sin_Q4 = arm_sin_f32(Q4);
    xb = -LEG_0 / 2 + LEG_1 * cos_Q1;
    xd =  LEG_0 / 2 + LEG_1 * cos_Q4;
    yb =  LEG_1 * sin_Q1;
    yd =  LEG_1 * sin_Q4;

    // arm_sqrt_f32((xd-xb)*(xd-xb)+(yd-yb)*(yd-yb),&Lbd);
    Lbd = sqrt((xd - xb) * (xd - xb) + (yd - yb) * (yd - yb));
    A0 = 2 * LEG_2 * (xd - xb);
    B0 = 2 * LEG_2 * (yd - yb);
    C0 = LEG_2 * LEG_2 + Lbd * Lbd - LEG_2 * LEG_2;
    Q2 = 2 * atan((B0 + sqrt(A0 * A0 + B0 * B0 - C0 * C0)) / (A0 + C0));

    xc = xb + arm_cos_f32(Q2) * LEG_2;
    yc = yb + arm_sin_f32(Q2) * LEG_2;
    //	arm_sqrt_f32(xc*xc +yc*yc,&L0);
    L0 = sqrt(xc * xc + yc * yc);
    Q0 = atan(yc / xc);
    
    //以下为速度求解
    vxb = -S1 * LEG_1 * sin_Q1;
    vyb =  S1 * LEG_1 * cos_Q1;
    vxd = -S4 * LEG_1 * sin_Q4;
    vyd =  S4 * LEG_1 * cos_Q4;
    Q3 = atan((yc - yd) / (xc - xd));
    S2 = ((vxd - vxb) * arm_cos_f32(Q3) + (vyd - vyb) * arm_sin_f32(Q3)) / (LEG_2 * arm_sin_f32(Q3 - Q2));
    //S3 = ((vxd - vxb) * cos(Q2) + (vyd - vyb) * sin(Q2)) / (LEG_2 * sin(Q3 - Q2));
    vxc = vxb - S2 * LEG_2 * arm_sin_f32(Q2);
    vyc = vyb + S2 * LEG_2 * arm_cos_f32(Q2);
    S0 = 3 * (-arm_sin_f32(abs(Q0)) * vxc - arm_cos_f32(Q0) * vyc);

    if(Q0<0) Q0+=pi;

    VMC_LEG_L.L0       = L0;
    VMC_LEG_L.Phi0     = Q0;
    VMC_LEG_L.Phi0_gyro = S0;

    VMC_LEG_L.Phi2 = Q2;
    VMC_LEG_L.Phi3 = Q3;

}



// 逆运动学解算
void inverse_kinematics(fp32 high, fp32 angle)
{
    fp32 yc, xc;
    fp32 A, B, C, T[2];
    fp32 ya = 0, xa = -LEG_0 / 2;
    fp32 q1, q4;
    fp32 temp;

    for (uint8_t i = 0; i < 2; i++)
    {
        // 两侧a坐标设置，由此看出坐标中心位于中心
        if (i == 0)
        {
            ya = 0, xa = -LEG_0 / 2;
        }
        else
        {
            ya = 0, xa = LEG_0 / 2;
        }

        yc = high;
        // xc = tan(angle-pi/2)*high;                        //xc为oc和机体的角度
        xc = (sin(angle-pi/2)/cos(angle-pi/2))*high;      //同上
        //xc = angle; // 直接设置为末端的横坐标

        // 代入公式计算
        A = (xc - xa) * (xc - xa) + (yc - ya) * (yc - ya) + LEG_1 * LEG_1 - LEG_2 * LEG_2;
        B = -2 * (xc - xa) * LEG_1;
        C = -2 * (yc - ya) * LEG_1;
        temp = sqrt(C * C + B * B - A * A);
        // arm_sqrt_f32(C*C+B*B-A*A,&temp);

        if (i == 0)
        {
            T[0] = (-C + temp) / (A - B);
            q1 = 2 * atan(T[0]);
        }

        else
        {
            T[1] = (-C - temp) / (A - B);
            q4 = 2 * atan(T[1]);
        }
    }

    if (q1 < -pi / 2)
        q1 = q1 + 2 * pi; // 防止角度过大导致奇异点

    VMC_LEG_R.Phi1_set = q1 - pi / 2; // 角度设定值赋值，并根据实际情况调节输出角度
    VMC_LEG_R.Phi4_set = q4 - pi / 2;

		//printf("q1_set:%f  q4_set:%f \n", VMC_LEG_R.Phi1_set, VMC_LEG_R.Phi4_set);
}
