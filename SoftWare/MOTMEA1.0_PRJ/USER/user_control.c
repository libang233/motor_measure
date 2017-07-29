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
#include "config_page_control.h"
#include "password_page_control.h"
#include "begin_page_control.h"
#include "message_send.h"
 
  

 
 /**
* @ Function Name : sys_manager_handle
* @ Author        : hlb
* @ Brief         : 用户控制器线程
* @ Date          : 2017.07.20
* @ Modify        : ...
 **/
void user_control_handle(void)
{

	switch(globalInformation.nowPage)
	{
		case PAGE_NUM_BEGIN:
			begin_page_handle();
			break;
		
		case PAGE_NUM_TEST:
			break;
		
		case PAGE_NUM_CONFIG:
			config_page_handle();
			break;
		
		case PAGE_NUM_ADJUST:
			break;
		
		case PAGE_NUM_PASSWORD_INPUT:
			password_page_handle();
			break;
		
		case PAGE_NUM_PASSWORD_WRONG:
			break;
		
		case PAGE_NUM_DEBUG:
			break;
		
		default:
			break;
	}
}
 

