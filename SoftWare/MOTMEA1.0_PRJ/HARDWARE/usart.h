#ifndef __USART_H_
#define __USART_H_

#include "stdio.h"	
#include "sys_manager.h"

#define USART_REC_LEN  			200     //定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能接收

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲，最大为USART_REC_LEN字节
extern u16 USART_RX_STA;         		//接收状态标记

//串口初始化
void usart_init(void);

#endif
