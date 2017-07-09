#ifndef __USART_H_
#define __USART_H_

#include "stdio.h"	
#include "sys_manager.h"

#define USART_REC_LEN  			1024     //定义最大接收字节数 256
#define EN_USART1_RX 			1	     //使能接收

//串口数据缓冲定义
typedef struct
{
	u8  Buff[USART_REC_LEN];			 //缓冲区
	u16 AddIdx;							 //添加索引
	u16 GetIdx;							 //取出索引
}Usart_Rx_Buff_TypeDef;

//接收缓冲
extern Usart_Rx_Buff_TypeDef UsartRxBuffer;		

extern u8 USART_RX_BUF[USART_REC_LEN]; 
extern u16 buffIdx;

//串口初始化
void usart_init(void);

#endif
