/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : usart.c
 * Author    : hlb
 * Brief     : 串口驱动代码
 * Date      : 2017.05.29
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "usart.h"
#include "newtype.h"
#include "dma.h"
 
//如果使能接收
#if EN_USART1_RX   

//接收缓冲
Usart_Rx_Buff_TypeDef UsartRxBuffer;	

//发送缓冲
Usart_Tx_Buff_TypeDef UsartTxBuffer;


/**
* @ Function Name : usart_data_init
* @ Author        : hlb
* @ Brief         : 串口数据初始化
* @ Date          : 2017.07.19
* @ Modify        : ...
 **/
void usart_data_init(void)
{
	//初始化接收缓冲
	memset(&UsartRxBuffer, 0, sizeof(UsartRxBuffer));
	//初始化发送缓冲
	memset(&UsartTxBuffer, 0, sizeof(UsartTxBuffer));
}
	 
/**
* @ Function Name : usart_init
* @ Author        : hlb
* @ Brief         : 串口初始化
* @ Date          : 2017.07.18
* @ Modify        : ...
 **/
void usart_init(u32 mode)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	//串口数据初始化
	usart_data_init();
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);		//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;            	//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);  				//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			 	//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化NVIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = BOUNDRATE;			//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	USART_Init(USART1, &USART_InitStructure); 				//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启串口接收终端
	USART_Cmd(USART1, ENABLE);                    			//使能串口1 
	
	memset(&UsartRxBuffer, 0, sizeof(UsartRxBuffer));		//串口缓冲初始化
	
		
	/*--------------------------------DMA-----------------------------------*/
	if (mode == USART_INIT_WITHOUT_DMA)
	{	
		return;
	}
	else if (mode == USART_INIT_WITH_DMA)
	{
		usart1_dma_init(); // 初始化dma
	}
	else
	{
		return;
	}
}


/**
* @ Function Name : USART1_IRQHandler
* @ Author        : hlb
* @ Brief         : 串口1中断服务程序
* @ Date          : 2017.05.29
* @ Modify        : ...
 **/
void USART1_IRQHandler(void)                	
{
	u16 i;
	u32 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  		
	{
		Res = USART_ReceiveData(USART1);						//读取接收到的数据

		if(UsartRxBuffer.AddIdx < USART_RX_LEN)
		{
			UsartRxBuffer.Buff[UsartRxBuffer.AddIdx++] = Res;	//填充至缓存
		}
		else
		{
			UsartRxBuffer.AddIdx = 0;
			UsartRxBuffer.Buff[UsartRxBuffer.AddIdx++] = Res;
		}
    } 
} 
#endif	

