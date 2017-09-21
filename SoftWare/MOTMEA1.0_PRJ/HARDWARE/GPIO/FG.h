#ifndef _FG_H
#define _FG_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"


#define FG1 GPIO_Pin_7 // FG�ź�1���� 												PA7
#define FG2 GPIO_Pin_12 // FG�ź�2���� 												PA12

#define arr 0XFFFF
#define psc 72-1
#define CAP_TIM               TIM3  // ����ʱ��
#define CAP_TIM_Channel       TIM_Channel_2   // ����ͨ��
#define CAP_TIM_IT_Channel    TIM_IT_CC2   // ����Ĵ���
#define CAP_TIM_NVIC          TIM3_IRQn  //�����ж�
#define CAP_TIM_RCC           RCC_APB1Periph_TIM3
#define CAP_Get_Channel       TIM_GetCapture2
#define CAP_TIM_Handler TIM3_IRQHandler
#define PI 3.1415926

extern u8  TIMXCHX_CAPTURE_STA;		//���벶��״̬		    				
extern u32 TIMXCHX_CAPTURE_VAL;	//���벶��ֵ	
extern u8 Show_flag;
extern float Freq_value;
extern u16 ReadValue1,ReadValue2; 

// FG���ų�ʼ��
extern void  GPIO_FG_Init(void);

/**
// ��ʾƵ��
#include "printf.h"    Ҫ�õĻ�Ҫ�������ڴ�ӡ
extern void show_frequency(void);

**/
// �˲�
extern void Freq_Sample(void);

#endif

