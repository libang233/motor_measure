/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : debug_page_control.c
 * Author    : hlb
 * Brief     : ���Խ������
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
* @ Brief         : ���Խ��������ϴ�
* @ Date          : 2017.09.21
* @ Modify        : ...
 **/   
void debug_page_push(void)
{
	//Ƶ�ʲ����˲�����
	Freq_Sample();
	
	//�����ٶ�
	debugInformation.speed = Freq_value * (60 / debugInformation.inputPoles) * 2;

	if(tt++>100)
	{
		tt=0;
	}
		
	//�������
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
	
	//ת���ַ���
	calculate_current_string(debugInformation.current, debugInformation.currentString, &debugInformation.currentStringIdx);
	calculate_current_string(debugInformation.speed, debugInformation.speedString, &debugInformation.speedStringIdx);
	
	write_control_state(ID_DEBUG_SPEED, ENABLE);
	write_control_state(ID_DEBUG_CURRENT, ENABLE);
	
	//�ϴ�����
	write_char(ID_DEBUG_SPEED, debugInformation.currentString, debugInformation.currentStringIdx);
	write_char(ID_DEBUG_CURRENT, debugInformation.speedString, debugInformation.speedStringIdx);
	
	write_control_state(ID_DEBUG_SPEED, CONTROL_DISENABLE);
	write_control_state(ID_DEBUG_CURRENT, CONTROL_DISENABLE);	
}
 
 /**
* @ Function Name : debug_page_init
* @ Author        : hlb
* @ Brief         : ���Խ����ʼ��
* @ Date          : 2017.09.21
* @ Modify        : ...
 **/  
void debug_page_init(void)
{
	//�����ٶȵ����ؼ�
	write_control_state(ID_DEBUG_SPEED, CONTROL_DISENABLE);
	write_control_state(ID_DEBUG_CURRENT, CONTROL_DISENABLE);
}
 
 
 /**
* @ Function Name : debug_page_handle
* @ Author        : hlb
* @ Brief         : ���Խ����߳�
* @ Date          : 2017.09.21
* @ Modify        : ...
 **/ 
void debug_page_handle(void)
{
	if(globalInformation.isPageInit == false)
	{
		//��ʼ������
		debug_page_init();
		
		globalInformation.isPageInit = true;
	}
	
	if(debugInformation.timer++ > DEBUG_PUSH_CYCLE)
	{
		//ˢ�µ���ֵ
		debug_page_push();
		
		adjustInformation.timer = 0;
		
	}
	
	if(debugInformation.isSetPWM == true)
	{
		SetPwm(debugInformation.inputPWMFrequen,debugInformation.inputPWM);
		
		debugInformation.isSetPWM = false;
	}
	
}	
