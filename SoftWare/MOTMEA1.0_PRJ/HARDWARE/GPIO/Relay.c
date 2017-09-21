#include "Relay.h"

 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : 继电器控制引脚初始化
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_Relay_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin= Relay_5V | Relay_12V |  Relay_EXPOWER | Relay_All;  //选择初始化 5V 12V EXpower 总开关的继电器引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
}

 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : 继电器配置初始化
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void Relay_Config_Init()
{
	Relay_5V_OFF;
	Relay_12V_OFF;
  Relay_EXPOWER_OFF;
	Relay_All_OFF;
}

