/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : newtype.c
 * Author    : hlb
 * Brief     : 数据结构初始化
 * Date      : 2017.05.30
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
 
#include "sys_manager.h"
#include "newtype.h"
#include "data_init.h"
#include "message_analysis.h"



/**
* @ Function Name : data_init
* @ Author        : hlb
* @ Brief         : 数据初始化
* @ Date          : 2017.05.30
* @ Modify        : ...
 **/
void data_init(void)
{
	//初始化全局标志位
	myEvnt.ALL = 0;
	
	//解析器初始化
	parser_init();
	
	//测试数据初始化；
	memset(&testingInformation, 0, sizeof(testingInformation));
	
	//校准界面数据初始化
	memset(&adjustInformation, 0, sizeof(adjustInformation));
	
	//配置界面数据初始化
	memset(&configInformation, 0, sizeof(configInformation));
	
	//密码界面数据初始化
	memset(&passwordInformation, 0, sizeof(passwordInformation));
	
	//调试界面密码初始化
	memset(&debugInformation, 0, sizeof(debugInformation));
	
	memset(&beginInformation, 0, sizeof(beginInformation));
	
}

