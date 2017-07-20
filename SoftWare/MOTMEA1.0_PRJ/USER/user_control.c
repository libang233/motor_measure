/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : user_control.c
 * Author    : hlb
 * Brief     : 用户控制器代码
 * Date      : 2017.07.18
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
 #include "user_control.h"
 #include "sys_manager.h"
 #include "message_analysis.h"
 
  
 /**
* @ Function Name : config_page_handle
* @ Author        : hlb
* @ Brief         : 配置界面函数
* @ Date          : 2017.05.29
* @ Modify        : ...
 **/
 void config_page_handle(void)
 {
	 
 }
 
 /**
* @ Function Name : sys_manager_handle
* @ Author        : hlb
* @ Brief         : 用户控制器线程
* @ Date          : 2017.05.29
* @ Modify        : ...
 **/
void user_control_handle(void)
{
	switch(globalInformation.nowPage)
	{
		case PAGE_NUM_BEGIN:
			break;
		
		case PAGE_NUM_TEST:
			break;
		
		case PAGE_NUM_CONFIG:
			break;
		
		case PAGE_NUM_ADJUST:
			break;
		
		case PAGE_NUM_PASSWORD_INPUT:
			break;
		
		case PAGE_NUM_PASSWORD_WRONG:
			break;
		
		case PAGE_NUM_DEBUG:
			break;
		
		default:
			break;
	}
}
 

