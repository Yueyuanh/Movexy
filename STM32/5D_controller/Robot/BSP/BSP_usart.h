#ifndef __BSP_USART_H
#define __BSP_USART_H

#define UART1_BUFFER_LEN 256
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stdio.h"

#include "struct_typedef.h"
void usart1_init(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);





#ifdef __cplusplus
}
#endif


#endif 
