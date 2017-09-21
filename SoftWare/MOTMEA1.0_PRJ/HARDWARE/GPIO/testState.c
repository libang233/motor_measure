 #include "testState.h"


 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : ����������ų�ʼ��
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void GPIO_TestState_Init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ��GPIO

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin= GPIO_OK |GPIO_NG | GPIO_SPEEK ;  // ����Ok NG SPEEK ����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 	/* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_TRIGLE ; //  ���� Trigle ����
	GPIO_Init(GPIOF,&GPIO_InitStructure); 	  /* ��ʼ��GPIO */
	
}


 /**
* @ Function Name : GPIO_Control_Init
* @ Author        : ygl
* @ Brief         : ����״̬�����ʼ��
* @ Date          :  2017.09.14
* @ Modify        : ...
 **/
void TestState_Config_Init()
{
	GPIO_OK_OFF;
	GPIO_NG_OFF;
	GPIO_SPEEK_OFF;
	
}
