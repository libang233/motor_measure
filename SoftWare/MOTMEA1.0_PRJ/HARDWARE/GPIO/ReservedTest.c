#include "ReservedTest.h"

 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : Ԥ��test���ų�ʼ��
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_Reserved_Test_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
  /*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=  GPIO_Test1 | GPIO_Test2  |  GPIO_Test3 | GPIO_Test4 | GPIO_Test5 ;  // ����Ok NG SPEEK ����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	/* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Test5;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	/* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Test6 | GPIO_Test7;
	GPIO_Init(GPIOC,&GPIO_InitStructure); 	/* ��ʼ��GPIO */
	
}
