#include "ReservedTest.h"

 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : 预留test引脚初始化
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_Reserved_Test_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
  /*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=  GPIO_Test1 | GPIO_Test2  |  GPIO_Test3 | GPIO_Test4 | GPIO_Test5 ;  // 配置Ok NG SPEEK 引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	/* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Test5;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	/* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Test6 | GPIO_Test7;
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	/* 初始化GPIO */
	
}
