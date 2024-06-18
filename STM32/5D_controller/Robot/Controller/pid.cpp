#include "pid.h"
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

void PID_t::init(uint8_t mode,const fp32 PID[3],fp32 max_out,fp32 max_iout)
{
	own_mode = mode;
	own_Kp = PID[0];
	own_Ki = PID[1];
	own_Kd = PID[2];
	own_max_out = max_out;
	own_max_iout = max_iout;
	Dbuf[0] = Dbuf[1] = Dbuf[2] =0.0f;
	error[0] = error[1] =error[2] = Pout = Dout = Iout = out =0.0f;
}

fp32 PID_t::calc(fp32 ref,fp32 set)
{
	error[2] = error[1];
	error[1] = error[0];
	own_set = set;
	own_fdb = ref;
	error[0] = set - ref;
	if(own_mode == PID_POSITION)
	{
		Pout = own_Kp * error[0];
		Iout += own_Ki * error[0];
		Dbuf[2] = Dbuf[1];
		Dbuf[1] = Dbuf[0];
		Dbuf[0] = error[0] - error[1];
		Dout = own_Kd *Dbuf[0];
		LimitMax(Iout,own_max_iout);
    if(isnan(Iout) || isinf(Iout))
			Iout=0;
		out = Pout + Iout + Dout;
    LimitMax(out,own_max_out);
	}
	else if(own_mode == PID_DELTA)
	{
		Pout = own_Kp * (error[0] - error[1]);
		Iout += own_Ki * error[0];
		Dbuf[2] = Dbuf[1];
		Dbuf[1] = Dbuf[0];
		Dbuf[0] = (error[0] - 2.0f *error[1] + error[2]);
		Dout = own_Kd * Dbuf[0];
		out = Pout + Iout + Dout;
		LimitMax(out,own_max_out);
	}
	return out;
}

void PID_t::clear(void)
{
	error[0] = error[1] = error[2] = 0.0f;
	Dbuf[0] = Dbuf[1] = Dbuf[2] = 0.0f;
	out = Pout = Iout = Dout =0.0f;
	own_fdb = own_set = 0.0f;
}


	
