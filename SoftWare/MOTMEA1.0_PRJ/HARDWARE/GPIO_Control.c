#include "GPIO_Control.h"



 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : GPIO引脚初始化
* @ Date          : 2017.09.14
* @ Modify        : ...
 **/
void GPIO_Control_Init()
{ 
	// 继电器引脚初始化
	GPIO_Relay_Init();
	// 测试状态引脚初始化
	GPIO_TestState_Init();
	// 电流测量引脚初始化
	GPIO_CurMeasure_Init();
	// PWM引脚初始化
	GPIO_PWM_Init();
	// FG引脚初始化
	GPIO_FG_Init();
	
	// 设置输出频率
	SetPwm(1000,50);
	// 继电器配置初始化
	Relay_Config_Init();
	// 输出状态配置初始化
	TestState_Config_Init();
	 
}

 

