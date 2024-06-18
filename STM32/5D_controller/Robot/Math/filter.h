#ifndef FILTER_H
#define FILTER_H

#include "struct_typedef.h"


#ifdef __cplusplus
extern "C"{
#endif

class LPF
{
		private:
		fp32 LPF_Ka;
		fp32 LPF_input[2];

		public:
		LPF();
		fp32 LPF_FILTER(fp32 input);

};

































#ifdef __cplusplus
}
#endif
#endif