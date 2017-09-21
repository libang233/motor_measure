#include "Reserved.h"


 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : 预留reserved引脚初始化
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_Reserved_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
  /*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin= GPIO_Reserved3 |GPIO_Reserved4 | GPIO_Reserved5 |GPIO_Reserved6 | GPIO_Reserved7 ;  // 配置reserved引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	/* 初始化GPIO */
}
