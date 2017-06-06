#ifndef __USART_H_
#define __USART_H_

#include "stdio.h"	
#include "sys_manager.h"

#define USART_REC_LEN  			256     //定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能接收

//串口数据缓冲定义
typedef struct
{
	u8  Buff[USART_REC_LEN];			//缓冲区
	u16 AddIdx;							//添加索引
	u16 GetIdx;							//取出索引
}Usart_Rx_Buff_TypeDef;

//接收缓冲
extern Usart_Rx_Buff_TypeDef UsartRxBuffer;		

//串口初始化
void usart_init(void);

#endif
