#ifndef _PWM_H
#define _PWM_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"

#define PWM_OUT GPIO_Pin_0  // PWM���  											PA0
//��ʼ��PWM
void GPIO_PWM_Init(void); 
// ���������Ƶ�ʺ�ռ�ձ�
void SetPwm(u16 fre,u16 cycle);
#endif
