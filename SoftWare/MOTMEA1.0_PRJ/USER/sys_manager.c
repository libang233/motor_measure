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
#include "data_init.h"
#include "message_analysis.h"
#include "message_send.h"
#include "user_control.h"
#include "sdcard_control.h"
#include "test.h"



//全局信息数据实例化
global_Information_TypeDef globalInformation;

//测试界面数据实例化
testing_Information_TypeDef testingInformation;

//校准界面数据实例化
adjust_Information_TypeDef adjustInformation;

//配置界面数据实例化
config_Information_TypeDef configInformation;

//密码界面数据实例化
password_Information_TypeDef passwordInformation;

//调试界面数据实例化
debug_Information_TypeDef debugInformation;

//开始界面数据实例化
begin_Information_TypeDef beginInformation;



/**
* @ Function Name : sys_manager_init
* @ Author        : hlb
* @ Brief         : 系统初始化
* @ Date          : 2017.05.29
* @ Modify        : ...
 **/
void sys_manager_init(void)
{   
	//中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//数据初始化
	data_init();
	
	//串口初始化
	usart_init(USART_INIT_WITH_DMA);
	
	//SD卡初始化
	sdcard_init();
	
	//解析器初始化
	parser_init();
	
	//LED和蜂鸣器初始化
	LED_buzzer_init();
	
	
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
	
	//串口解析控制器线程
	usart_analysis_handle();								
	
	//串口发送控制器线程
	usart_send_handle();
	
	//SD卡控制器线程
	sdcard_control_handle();
	
	//用户控制器线程
	user_control_handle();
	

	test();

	
}
 

