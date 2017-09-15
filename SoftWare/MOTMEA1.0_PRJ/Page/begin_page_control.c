/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : begin_page_control.c
 * Author    : hlb
 * Brief     : 开始界面程序
 * Date      : 2017.07.29
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "begin_page_control.h"
#include "sys_manager.h"
 
 /**
* @ Function Name : begin_page_handle
* @ Author        : hlb
* @ Brief         : 开始界面线程
* @ Date          : 2017.07.29
* @ Modify        : ...
 **/ 
void begin_page_handle(void)
{
	if(globalInformation.isPageInit == false)
	{
		configInformation.isConfig = false;
		globalInformation.isPageInit = true;
	}
}
 

