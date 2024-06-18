#ifndef LQR_H
#define LQR_H

#include "struct_typedef.h"

#define UPDATE_TIME 9.25



#ifdef __cplusplus
extern "C"{
#endif


#ifdef __cplusplus

class LQR
{
		public:

		fp32 own_K1;
		fp32 own_K2;


		fp32 own_max_out;

		fp32 out;
		
		void init(const fp32 K[2],fp32 max_out);
		fp32 LQR_calc(fp32 ins_angle,fp32 ins_gyro ,fp32 set_angle);
};




#endif	

#ifdef __cplusplus
}
#endif
#endif
