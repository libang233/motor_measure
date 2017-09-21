#ifndef _testState_H
#define _testState_H
#include "stm32f10x.h"

// ����״̬
#define GPIO_OK   		GPIO_Pin_12  // ����ͨ�����						PB12
#define GPIO_NG 			GPIO_Pin_13  //  ���Բ�ͨ����� 				PB13
#define GPIO_SPEEK 		GPIO_Pin_14  // ����������� 					PB14
#define GPIO_TRIGLE 	GPIO_Pin_7 // �䴥����ʼ����							PF7


// OK���״̬
#define GPIO_OK_OFF GPIO_ResetBits(GPIOB,GPIO_OK)
#define GPIO_OK_ON  GPIO_SetBits(GPIOB,GPIO_OK)
// NG���״̬
#define GPIO_NG_OFF GPIO_ResetBits(GPIOB,GPIO_NG)
#define GPIO_NG_ON  GPIO_SetBits(GPIOB,GPIO_NG)
// SPEEK���״̬
#define GPIO_SPEEK_OFF GPIO_ResetBits(GPIOB,GPIO_SPEEK)
#define GPIO_SPEEK_ON  GPIO_SetBits(GPIOB,GPIO_SPEEK)


// ����״̬������ų�ʼ��
void GPIO_TestState_Init(void);
// ����״̬������ó�ʼ��
void TestState_Config_Init(void);

#endif