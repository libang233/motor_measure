#include "Reserved.h"


 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : Ԥ��reserved���ų�ʼ��
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_Reserved_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
  /*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin= GPIO_Reserved3 |GPIO_Reserved4 | GPIO_Reserved5 |GPIO_Reserved6 | GPIO_Reserved7 ;  // ����reserved����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	/* ��ʼ��GPIO */
}
