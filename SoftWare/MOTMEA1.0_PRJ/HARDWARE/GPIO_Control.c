#include "GPIO_Control.h"



 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : GPIO���ų�ʼ��
* @ Date          : 2017.09.14
* @ Modify        : ...
 **/
void GPIO_Control_Init()
{ 
	// �̵������ų�ʼ��
	GPIO_Relay_Init();
	// ����״̬���ų�ʼ��
	GPIO_TestState_Init();
	// �����������ų�ʼ��
	GPIO_CurMeasure_Init();
	// PWM���ų�ʼ��
	GPIO_PWM_Init();
	// FG���ų�ʼ��
	GPIO_FG_Init();
	
	// �������Ƶ��
	SetPwm(1000,50);
	// �̵������ó�ʼ��
	Relay_Config_Init();
	// ���״̬���ó�ʼ��
	TestState_Config_Init();
	 
}

 

