#ifndef _CUR_H
#define _CUR_H
#include "stm32f10x.h"
#include "stm32f10x_adc.h"

#define CUR_SDA 	GPIO_Pin_0 // 数字电流测量SDA 							PB0
#define CUR_SCL 	GPIO_Pin_1 //  数字电流测量SCL 							PB1
#define CUR_SIM 	GPIO_Pin_6 // 电流采集模拟电压 							PF6

//定义全局变量
extern u16 cur_data;
void GPIO_CurMeasure_Init(void);

#endif
