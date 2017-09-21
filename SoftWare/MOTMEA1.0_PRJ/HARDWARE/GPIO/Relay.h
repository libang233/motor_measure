#ifndef _Relay_H
#define _Relay_H
#include "stm32f10x.h"


// 继电器
#define Relay_5V       GPIO_Pin_8   //  5V继电器开关 					PB8
#define Relay_12V      GPIO_Pin_9   //  12V继电器开关					PB9
#define Relay_EXPOWER  GPIO_Pin_11  // 继电器总开关		  PB11
#define Relay_All      GPIO_Pin_10  // 外部电源继电器开关					PB10


// 5V继电器开关状态
#define Relay_5V_OFF GPIO_ResetBits(GPIOB,Relay_5V)
#define Relay_5V_ON  GPIO_SetBits(GPIOB,Relay_5V)
// 12V继电器开关状态
#define Relay_12V_OFF GPIO_ResetBits(GPIOB,Relay_12V)
#define Relay_12V_ON  GPIO_SetBits(GPIOB,Relay_12V)
// 外部电源继电器开关状态
#define Relay_EXPOWER_OFF GPIO_ResetBits(GPIOB,Relay_EXPOWER)
#define Relay_EXPOWER_ON  GPIO_SetBits(GPIOB,Relay_EXPOWER)
// 总继电器开关状态
#define Relay_All_OFF GPIO_ResetBits(GPIOB,Relay_All)
#define Relay_All_ON  GPIO_SetBits(GPIOB,Relay_All)

// 继电器引脚初始化
void GPIO_Relay_Init(void);

// 继电器配置初始化
void Relay_Config_Init(void);


#endif
