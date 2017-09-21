#ifndef _PWM_H
#define _PWM_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"

#define PWM_OUT GPIO_Pin_0  // PWM输出  											PA0
//初始化PWM
void GPIO_PWM_Init(void); 
// 设置输出的频率和占空比
void SetPwm(u16 fre,u16 cycle);
#endif
