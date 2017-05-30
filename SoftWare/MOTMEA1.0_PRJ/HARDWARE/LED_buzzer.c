/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : LED_buzzer.c
 * Author    : hlb
 * Brief     : LED和蜂鸣器驱动代码
 * Date      : 2017.05.30
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "sys_manager.h"

/**
* @ Function Name : LED_buzzer_init
* @ Author        : hlb
* @ Brief         : LED和蜂鸣器初始化
* @ Date          : 2017.05.30
* @ Modify        : ...
 **/
void LED_buzzer_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PE端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PE.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //初始化GPIOE.5
	GPIO_SetBits(GPIOB,GPIO_Pin_5);						 	 //PE.5 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		 //LED1-->PE.6 端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出，IO口速度为50MHz
	GPIO_SetBits(GPIOE,GPIO_Pin_6); 						 //PE.6 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //buzzer-->PE.4 端口配置
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出，IO口速度为50MHz
	GPIO_ResetBits(GPIOE,GPIO_Pin_4); 						 //PE.4 输出低
}

