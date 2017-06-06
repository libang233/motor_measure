/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : sys_manager.c
 * Author    : hlb
 * Brief     : 系统管理模块，控制系统的初始化和运行流程
 * Date      : 2017.05.29
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "sys_manager.h"
#include "usart.h"
#include "LED_buzzer.h"
#include "screen.h"
#include "newtype.h"
#include "data_init.h"
#include "message_analysis.h"

union whole_Evnt_TypeDef myEvnt;				//全局事件标志位实例化

/**
* @ Function Name : sys_manager_init
* @ Author        : hlb
* @ Brief         : 系统初始化
* @ Date          : 2017.05.29
* @ Modify        : ...
 **/
void sys_manager_init(void)
{   
	//数据初始化
	data_init();
	
	//串口初始化
	usart_init();
	
	//LED和蜂鸣器初始化
	LED_buzzer_init();
	
	//屏幕初始化
	screen_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
}


/**
* @ Function Name : sys_manager_handle
* @ Author        : hlb
* @ Brief         : 系统主循环
* @ Date          : 2017.05.29
* @ Modify        : ...
 **/
void sys_manager_handle(void)
{

	Serial_analysis_handle();								//串口数据控制器线程

	
	if(myEvnt.Bit.IsTest == 1)								//进入检测
	{
		
	}
	
	if(myEvnt.Bit.IsConfig == 1)							//进入配置
	{
		
	}
	
	if(myEvnt.Bit.IsResearch == 1)							//进入查阅
	{
		
	}
	
	if(myEvnt.Bit.IsDebug == 1)								//进入调试
	{
		
	}
	
	if(myEvnt.Bit.IsAdjust == 1)							//进入校准
	{
		
	}
	
//	u16 t;
//	u16 len;
//	if(USART_RX_STA & 0x8000)
//	{					   
//		len = USART_RX_STA & 0x3fff;
//		for(t = 0; t < len; t++)
//		{
//			
//			USART_SendData(USART1, USART_RX_BUF[t]);				//
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);	//
//		}
//		USART_RX_STA=0;
//	}
}
 
