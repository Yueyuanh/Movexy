#include "hipnuc_task.h"
#include "cmsis_os.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>



hipnuc_raw_t hipnuc_raw = {0};

static uint8_t decode_succ = 0;

static char log_buf[256];

uint8_t ch;


void hipnuc_task(void const * argument)
{
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&hipnuc_raw, 1); // Enable UART receive interrupt (optional if you want to keep ISR working)
    vTaskDelay(10);
    memset(&hipnuc_raw, 0, sizeof(hipnuc_raw_t));
    decode_succ = 0;

    while(1)
    {
        if (decode_succ)
        {
            decode_succ = 0;
            hipnuc_dump_packet(&hipnuc_raw, log_buf, sizeof(log_buf));
        }
    
        vTaskDelay(2);
    }
}


extern "C"
{
    /**
     * @brief   串口1接收中断
     * @note    先后读取SR和DR寄存器后，中断标志位会自动清除
     */
void USART1_IRQHandler(void)
{
	if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET)
  {
		ch = USART1->DR;						
    decode_succ = hipnuc_input(&hipnuc_raw, ch);    
	}
}
}


/**
  * @brief          获取数据信息
  * @param[in]      NULL
  * @retval         NULL
  */
const hipnuc_raw_t *get_imu_point(void)
{
    return &hipnuc_raw;
}
