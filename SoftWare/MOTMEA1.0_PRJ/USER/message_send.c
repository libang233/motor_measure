/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : message_send.c
 * Author    : hlb
 * Brief     : 数据发送代码
 * Date      : 2017.07.18
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "message_send.h"
#include "usart.h"
#include "dma.h"


/**
* @ Function Name : usart_send_handle
* @ Author        : hlb
* @ Brief         : 串口数据发送线程
* @ Date          : 2017.07.18
* @ Modify        : ...
**/
void usart_send_handle(void)
{


}

/**
* @ Function Name : dma_usart_send
* @ Author        : hlb
* @ Brief         : DMA串口数据发送
* @ Date          : 2017.07.18
* @ Input         : u8 *res	      	  发送的数据  
					u8 len            传输数据的长度
* @ Modify        : ...
**/
void dma_usart_send(u8 *res, u8 len)
{
	// 禁用DMA
	DMA_Cmd(USART1_TX_DMA_CHANNEL, DISABLE);
	
	// 设置DMA的传输值
	USART1_TX_DMA_CHANNEL -> CNDTR = len;
	
	// 设置传输地址
	USART1_TX_DMA_CHANNEL -> CMAR  = (u32)res;
	
	// 启动DMA
	DMA_Cmd(USART1_TX_DMA_CHANNEL, ENABLE);
	
}

