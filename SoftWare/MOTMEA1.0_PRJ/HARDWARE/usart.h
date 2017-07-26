#ifndef __USART_H_
#define __USART_H_

#include "stdio.h"	
#include "sys_manager.h"

//定义最大接收字节数 
#define USART_RX_LEN  			(u32)1024   

//最大发送缓冲
#define USART_TX_LEN			(u32)2048	

//发送缓冲个数
#define TX_BUFFER_NUM_DEFAULT   (u32)2

//缓冲区头部索引
#define BUFFER_HEAD_DEFAULT          (u32)0

//使能接收
#define EN_USART1_RX 			1	     


// 串口初始化模式定义
// 不使用DMA进行初始化
#define USART_INIT_WITHOUT_DMA     (u32)0

// 使用DMA进行初始化
#define USART_INIT_WITH_DMA        (u32)1


#pragma pack(push, 1) 
//串口数据缓冲定义
typedef struct
{
	volatile u8  Buff[USART_RX_LEN];			 	 //缓冲区
	volatile u16 AddIdx;							 //添加索引
	volatile u16 GetIdx;							 //取出索引
	
}Usart_Rx_Buff_TypeDef;
#pragma pack(pop)

#pragma pack(push, 1) 
//串口数据缓冲定义
typedef struct
{
	u8  Buff[TX_BUFFER_NUM_DEFAULT][USART_TX_LEN];			 	 //缓冲区
	u16 Idx[TX_BUFFER_NUM_DEFAULT];								 //添加索引
	u8  PartAvailAction;										 //激活缓冲区位置
	bool DmaBusy;												 //发送DMA是否为忙
	
}Usart_Tx_Buff_TypeDef;
#pragma pack(pop)

extern Usart_Rx_Buff_TypeDef UsartRxBuffer;		
extern Usart_Tx_Buff_TypeDef UsartTxBuffer;	


void usart_init(u32 mode);

#endif
