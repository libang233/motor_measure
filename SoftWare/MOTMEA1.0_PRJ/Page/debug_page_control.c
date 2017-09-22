/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : debug_page_control.c
 * Author    : hlb
 * Brief     : 调试界面程序
 * Date      : 2017.07.25
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "debug_page_control.h"
#include "adjust_page_control.h"
#include "sys_manager.h"
#include "message_analysis.h"
#include "message_send.h"
#include "sdcard_control.h"
#include "usart.h"
#include "CUR.h"
#include "FG.h"
#include "PWM.h"
 
#define DEBUG_PUSH_CYCLE 50000
 
int tt=0; 
 
 /**
* @ Function Name : debug_page_push
* @ Author        : hlb
* @ Brief         : 调试界面数据上传
* @ Date          : 2017.09.21
* @ Modify        : ...
 **/   
void debug_page_push(void)
{
	//频率采样滤波处理
	Freq_Sample();
	
	//计算速度
	debugInformation.speed = Freq_value * (60 / debugInformation.inputPoles) * 2;

	if(tt++>100)
	{
		tt=0;
	}
		
	//计算电流
	if(adjustInformation.signOfadjustParameter == 0)
	{
		adjustInformation.measureCurrent = cur_data + adjustInformation.adjustParameter;
	}
	else
	{
		if(cur_data > adjustInformation.adjustParameter)
		{
			adjustInformation.measureCurrent = cur_data - adjustInformation.adjustParameter;
		}
		else
		{
			adjustInformation.measureCurrent = 0;
		}
	}
	
	debugInformation.current = tt;
	
	//转换字符串
	calculate_current_string(debugInformation.current, debugInformation.currentString, &debugInformation.currentStringIdx);
	calculate_current_string(debugInformation.speed, debugInformation.speedString, &debugInformation.speedStringIdx);
	
	write_control_state(ID_DEBUG_SPEED, ENABLE);
	write_control_state(ID_DEBUG_CURRENT, ENABLE);
	
	//上传数据
	write_char(ID_DEBUG_SPEED, debugInformation.currentString, debugInformation.currentStringIdx);
	write_char(ID_DEBUG_CURRENT, debugInformation.speedString, debugInformation.speedStringIdx);
	
	write_control_state(ID_DEBUG_SPEED, CONTROL_DISENABLE);
	write_control_state(ID_DEBUG_CURRENT, CONTROL_DISENABLE);	
}
 
 /**
* @ Function Name : debug_page_init
* @ Author        : hlb
* @ Brief         : 调试界面初始化
* @ Date          : 2017.09.21
* @ Modify        : ...
 **/  
void debug_page_init(void)
{
	//禁能速度电流控件
	write_control_state(ID_DEBUG_SPEED, CONTROL_DISENABLE);
	write_control_state(ID_DEBUG_CURRENT, CONTROL_DISENABLE);
}
 
 
 /**
* @ Function Name : debug_page_handle
* @ Author        : hlb
* @ Brief         : 调试界面线程
* @ Date          : 2017.09.21
* @ Modify        : ...
 **/ 
void debug_page_handle(void)
{
	if(globalInformation.isPageInit == false)
	{
		//初始化界面
		debug_page_init();
		
		globalInformation.isPageInit = true;
	}
	
	if(debugInformation.timer++ > DEBUG_PUSH_CYCLE)
	{
		//刷新电流值
		debug_page_push();
		
		adjustInformation.timer = 0;
		
	}
	
	if(debugInformation.isSetPWM == true)
	{
		SetPwm(debugInformation.inputPWMFrequen,debugInformation.inputPWM);
		
		debugInformation.isSetPWM = false;
	}
	
}	
