#ifndef _Reserved_H
#define _Reserved_H
#include "stm32f10x.h"


// 系统保留
#define GPIO_Reserved3 GPIO_Pin_1  //  												PC1
#define GPIO_Reserved4 GPIO_Pin_2  //  												PC2
#define GPIO_Reserved5 GPIO_Pin_3  //  												PC3
#define GPIO_Reserved6 GPIO_Pin_4  //  												PC4
#define GPIO_Reserved7 GPIO_Pin_5  //  												PC5

// 预留引脚初始化
void GPIO_Reserved_Init(void);

#endif
