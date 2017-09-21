#ifndef _ReservedTest_H
#define _ReservedTest_H
#include "stm32f10x.h"

// 系统保留
#define GPIO_Test1 GPIO_Pin_4  // 														PA4
#define GPIO_Test2 GPIO_Pin_5  // 														PA5
#define GPIO_Test3 GPIO_Pin_6  // 														PA6
#define GPIO_Test4 GPIO_Pin_8  // 														PA8
#define GPIO_Test5 GPIO_Pin_12 // 														PA12
#define GPIO_Test6 GPIO_Pin_5  // 														PB5
#define GPIO_Test7 GPIO_Pin_6  //  														PC6
#define GPIO_Test8 GPIO_Pin_7  //  														PC7

// 预留test引脚初始化
void GPIO_Reserved_Test_Init(void);

#endif
