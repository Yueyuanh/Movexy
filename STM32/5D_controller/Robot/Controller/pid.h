#ifndef __PID_H
#define __PID_H

#include "struct_typedef.h"
#ifdef __cplusplus
extern "C"{
#endif
	
#ifdef __cplusplus
enum PID_MODE
{
	PID_POSITION = 0,
	PID_DELTA,
};	
	
class PID_t
{
	public:
		uint8_t own_mode;
		fp32 own_Kp;
	  fp32 own_Ki;
	  fp32 own_Kd;
	  fp32 own_max_out;  //最大输出
	  fp32 own_max_iout; //最大积分输出
    fp32 own_set; //设定值
    fp32 own_fdb; //当前值
    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //微分项 0最新 1上一次 2上上次
    fp32 error[3]; //误差项 0最新 1上一次 2上上次
	  
		void init(uint8_t mode,const fp32 PID[3],fp32 max_out,fp32 max_iout);
	  fp32 calc(fp32 ref,fp32 set);
	  void clear(void);
};

#endif	
	
#ifdef __cplusplus
}
#endif

#endif
