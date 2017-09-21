#include "Relay.h"

 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : �̵����������ų�ʼ��
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_Relay_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin= Relay_5V | Relay_12V |  Relay_EXPOWER | Relay_All;  //ѡ���ʼ�� 5V 12V EXpower �ܿ��صļ̵�������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	   /* ��ʼ��GPIO */
	
}

 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : �̵������ó�ʼ��
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void Relay_Config_Init()
{
	Relay_5V_OFF;
	Relay_12V_OFF;
  Relay_EXPOWER_OFF;
	Relay_All_OFF;
}

