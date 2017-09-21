#ifndef _testState_H
#define _testState_H
#include "stm32f10x.h"

// 测试状态
#define GPIO_OK   		GPIO_Pin_12  // 测试通过输出						PB12
#define GPIO_NG 			GPIO_Pin_13  //  测试不通过输出 				PB13
#define GPIO_SPEEK 		GPIO_Pin_14  // 测试声音输出 					PB14
#define GPIO_TRIGLE 	GPIO_Pin_7 // ′触发开始输入							PF7


// OK输出状态
#define GPIO_OK_OFF GPIO_ResetBits(GPIOB,GPIO_OK)
#define GPIO_OK_ON  GPIO_SetBits(GPIOB,GPIO_OK)
// NG输出状态
#define GPIO_NG_OFF GPIO_ResetBits(GPIOB,GPIO_NG)
#define GPIO_NG_ON  GPIO_SetBits(GPIOB,GPIO_NG)
// SPEEK输出状态
#define GPIO_SPEEK_OFF GPIO_ResetBits(GPIOB,GPIO_SPEEK)
#define GPIO_SPEEK_ON  GPIO_SetBits(GPIOB,GPIO_SPEEK)


// 测试状态输出引脚初始化
void GPIO_TestState_Init(void);
// 测试状态输出配置初始化
void TestState_Config_Init(void);

#endif