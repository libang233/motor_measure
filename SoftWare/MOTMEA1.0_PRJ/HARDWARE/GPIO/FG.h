#ifndef _FG_H
#define _FG_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"


#define FG1 GPIO_Pin_7 // FG信号1输入 												PA7
#define FG2 GPIO_Pin_12 // FG信号2输入 												PA12

#define arr 0XFFFF
#define psc 72-1
#define CAP_TIM               TIM3  // 捕获定时器
#define CAP_TIM_Channel       TIM_Channel_2   // 捕获通道
#define CAP_TIM_IT_Channel    TIM_IT_CC2   // 捕获寄存器
#define CAP_TIM_NVIC          TIM3_IRQn  //配置中断
#define CAP_TIM_RCC           RCC_APB1Periph_TIM3
#define CAP_Get_Channel       TIM_GetCapture2
#define CAP_TIM_Handler TIM3_IRQHandler
#define PI 3.1415926

extern u8  TIMXCHX_CAPTURE_STA;		//输入捕获状态		    				
extern u32 TIMXCHX_CAPTURE_VAL;	//输入捕获值	
extern u8 Show_flag;
extern float Freq_value;
extern u16 ReadValue1,ReadValue2; 

// FG引脚初始化
extern void  GPIO_FG_Init(void);

/**
// 显示频率
#include "printf.h"    要用的话要保函串口打印
extern void show_frequency(void);

**/
// 滤波
extern void Freq_Sample(void);

#endif

