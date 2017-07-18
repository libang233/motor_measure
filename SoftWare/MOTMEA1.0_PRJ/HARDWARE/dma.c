/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : dma.c
 * Author    : hlb
 * Brief     : DMA驱动代码
 * Date      : 2017.07.18
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "dma.h"
#include "usart.h"

bool Usart1_Dma_Enable;

/**
* @ Function Name : drv_mcu_usart1_dma_init
* @ Author        : hlb
* @ Brief         : 初始化串口1的dma。
* @ Date          : 2017.07.18
* @ Modify        : ...
 **/
void usart1_dma_init(void)
{
	DMA_InitTypeDef dma_initstruct;  
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	
	
	/*------------------------------TX_DMA----------------------------------*/
	//DMA发送中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
	
	// 重置TX_DMA通道
	DMA_DeInit(USART1_TX_DMA_CHANNEL);
	// 使能时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
	dma_initstruct.DMA_BufferSize         = 1;                        		  // 设置dma的buffersize，此处为预留
	dma_initstruct.DMA_DIR                = DMA_DIR_PeripheralDST;    		  // 设置dma的方向由内存到外设
	dma_initstruct.DMA_M2M                = DMA_M2M_Disable;         		  // 禁止内存到内存的传输
	dma_initstruct.DMA_BufferSize         = USART_TX_LEN;					  // 设置DMA在传输时缓冲区的长度	
	dma_initstruct.DMA_MemoryBaseAddr     = null;              				  // 设置源地址
	dma_initstruct.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);       		  // 设置传输地址
	dma_initstruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;  		  // 每次传输单位为字节
	dma_initstruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;     		  // 允许内存自增地址
	dma_initstruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable; 	   	  // 禁止外设自增
	dma_initstruct.DMA_Mode               = DMA_Mode_Normal;          		  // 普通模式
	dma_initstruct.DMA_Priority           = USART1_TX_DMA_PRIORITY;	 		  // 设置DMA通道的优先级
	// 初始化DMA1的4通道(USART1, TX)
	DMA_Init(USART1_TX_DMA_CHANNEL, &dma_initstruct);
	
	/*------------------------------RX_DMA----------------------------------*/
	DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);                        	   	  // 关DMA通道
	DMA_DeInit(USART1_RX_DMA_CHANNEL);
	dma_initstruct.DMA_MemoryBaseAddr     = (u32)UsartRxBuffer.Buff;  		  // 设置源地址
	dma_initstruct.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);      		  // 设置传输地址
	dma_initstruct.DMA_DIR                = DMA_DIR_PeripheralSRC;    		  // 由外设到内存
	dma_initstruct.DMA_BufferSize         = USART_RX_LEN;  		  		 	  // 设置DMA在传输时缓冲区的长度	
	dma_initstruct.DMA_PeripheralInc	  = DMA_PeripheralInc_Disable;		  // 禁止外设自增
	dma_initstruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;     		  // 允许内存自增地址
	dma_initstruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	  // 外设数据字长
	dma_initstruct.DMA_MemoryDataSize	  = DMA_MemoryDataSize_Byte;		  // 内存数据字长
	dma_initstruct.DMA_Mode               = DMA_Mode_Normal;         		  // 循环接收
	dma_initstruct.DMA_Priority           = USART1_RX_DMA_PRIORITY;	  		  // 设置DMA通道的优先级
	dma_initstruct.DMA_M2M                = DMA_M2M_Disable;         		  // 禁止内存到内存的传输
	// 初始化DMA1的5通道(USART1, RX)
	DMA_Init(USART1_RX_DMA_CHANNEL, &dma_initstruct);
	
	//使能接收通道  
    DMA_Cmd(USART1_RX_DMA_CHANNEL,ENABLE);  
	
	// 允许串口DMA
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	// 置位dma状态
	Usart1_Dma_Enable = true;

	
}

/**
* @ Function Name : DMA1_Channel4_IRQHandler
* @ Author        : hlb
* @ Brief         : DMA发送中断
* @ Date          : 2017.07.18
* @ Modify        : ...
 **/
void DMA1_Channel4_IRQHandler(void)  
{  
    //清除标志位  
    DMA_ClearFlag(DMA1_FLAG_TC4);  
    //关闭DMA  
    DMA_Cmd(DMA1_Channel4,DISABLE);  
    //DMA1_Channel4->CCR &= ~(1<<0);  
  
    //允许再次发送  
    //Flag_Uart_Send = 0;  
}  

