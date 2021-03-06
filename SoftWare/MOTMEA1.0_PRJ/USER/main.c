/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : main.c
 * Author    : hlb
 * Brief     : 该文件的编码格式为UTF-8
 * Date      : 2017.05.29
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
 

#include "sys_manager.h"

 
 /**
* @ Function Name : main
* @ Author        : hlb
* @ Brief         : 主程序运行接口。
* @ Date          : 2017.05.29
* @ Output        : int 
* @ Modify        : ...
 **/
int main(void)
{
			
 	// 系统初始化函数
	sys_manager_init();
	
	for(;;)
	{
		// 系统主循环 
		sys_manager_handle();
	}
}

