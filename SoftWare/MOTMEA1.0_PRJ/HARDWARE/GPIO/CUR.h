#ifndef _CUR_H
#define _CUR_H
#include "stm32f10x.h"
#include "stm32f10x_adc.h"

#define CUR_SDA 	GPIO_Pin_0 // ���ֵ�������SDA 							PB0
#define CUR_SCL 	GPIO_Pin_1 //  ���ֵ�������SCL 							PB1
#define CUR_SIM 	GPIO_Pin_6 // �����ɼ�ģ���ѹ 							PF6

//����ȫ�ֱ���
extern u16 cur_data;
void GPIO_CurMeasure_Init(void);

#endif
