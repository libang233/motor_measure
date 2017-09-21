 #include "testState.h"


 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : 测试输出引脚初始化
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_TestState_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化GPIO

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin= GPIO_OK |GPIO_NG | GPIO_SPEEK ;  // 配置Ok NG SPEEK 引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	/* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_TRIGLE ; //  配置 Trigle 引脚
	GPIO_Init(GPIOF,&GPIO_InitStructure); 	  /* 初始化GPIO */
	
}


 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : 测试状态输出初始化
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void TestState_Config_Init()
{
	GPIO_OK_OFF;
	GPIO_NG_OFF;
	GPIO_SPEEK_OFF;
	
}
