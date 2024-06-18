#include "LQR.h"
#include "cmath"

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


void LQR::init(const fp32 K[2],fp32 max_out)
{
		own_K1 = K[0];
		own_K2 = K[1];
		
		own_max_out=max_out;

}

fp32 LQR::LQR_calc(fp32 ins_angle,fp32 ins_gyro ,fp32 set_angle)
{
		//��Ϊƽ�ⲽ��ֱ����LQR�㷨

		out=-own_K1*(ins_angle-set_angle)-own_K2*(ins_gyro-0);
		LimitMax(out,own_max_out);

		return out;
}

