#include "BSP_usart.h"
#include "usart.h"
#include "cmsis_os.h"
#include "string.h"
#include "stm32f4xx_hal.h"


extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

extern uint8_t UART1_BUFFER[UART1_BUFFER_LEN];




void usart1_init()
{

    //使能 DMA 串口接收
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);

    
    //使能空闲中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	
  	//enable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart1_rx);
	
   while(hdma_usart1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_rx);
    }
		
	__HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,DMA_FLAG_TCIF1_5 
		| DMA_FLAG_HTIF1_5);
		
	hdma_usart1_rx.Instance->PAR = (uint32_t) & (USART1->DR);
	hdma_usart1_rx.Instance->M0AR = (uint32_t)(UART1_BUFFER_LEN);
	
	
	__HAL_DMA_SET_COUNTER(huart1.hdmarx, UART1_BUFFER_LEN);
	__HAL_DMA_ENABLE(&hdma_usart1_rx);

}




 //重定向c库函数printf到串口USARTx，重定向后可使用printf函数
 int fputc(int ch, FILE *f)
 {
     /* 发送一个字节数据到串口USARTx */
     HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
 }

 //重定向c库函数scanf到串口USARTx，重写向后可使用scanf、getchar等函数
 int fgetc(FILE *f)
 {
     int ch;
     /* 等待串口输入数据 */
     while (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE) == RESET);
     HAL_UART_Receive(&huart6, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
 }