#ifndef _Relay_H
#define _Relay_H
#include "stm32f10x.h"


// �̵���
#define Relay_5V       GPIO_Pin_8   //  5V�̵������� 					PB8
#define Relay_12V      GPIO_Pin_9   //  12V�̵�������					PB9
#define Relay_EXPOWER  GPIO_Pin_11  // �̵����ܿ���		  PB11
#define Relay_All      GPIO_Pin_10  // �ⲿ��Դ�̵�������					PB10


// 5V�̵�������״̬
#define Relay_5V_OFF GPIO_ResetBits(GPIOB,Relay_5V)
#define Relay_5V_ON  GPIO_SetBits(GPIOB,Relay_5V)
// 12V�̵�������״̬
#define Relay_12V_OFF GPIO_ResetBits(GPIOB,Relay_12V)
#define Relay_12V_ON  GPIO_SetBits(GPIOB,Relay_12V)
// �ⲿ��Դ�̵�������״̬
#define Relay_EXPOWER_OFF GPIO_ResetBits(GPIOB,Relay_EXPOWER)
#define Relay_EXPOWER_ON  GPIO_SetBits(GPIOB,Relay_EXPOWER)
// �̵ܼ�������״̬
#define Relay_All_OFF GPIO_ResetBits(GPIOB,Relay_All)
#define Relay_All_ON  GPIO_SetBits(GPIOB,Relay_All)

// �̵������ų�ʼ��
void GPIO_Relay_Init(void);

// �̵������ó�ʼ��
void Relay_Config_Init(void);


#endif
