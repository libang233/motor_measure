#ifndef _GPIO_Control_H
#define _GPIO_Control_H
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"

// 控制GPIO
#include "CUR.h"
#include "PWM.h"
#include "FG.h"
#include "Relay.h"
#include "Reserved.h"
#include "ReservedTest.h"
#include "testState.h"

void GPIO_Control_Init(void);

#endif


