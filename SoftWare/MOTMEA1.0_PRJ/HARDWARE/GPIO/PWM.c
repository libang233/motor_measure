#include "PWM.h"


// ��Ƶ ���� ռ�ձ�
u16 Prescaler = 0;
u16 Period = 0;
u16 Pulse = 0;

 /**
* @ Function Name :  GPIO_PWM_Init
* @ Author        : ygl
* @ Brief         : PWM���ų�ʼ��
* @ Date          : 2017.09.19
* @ Modify        : 
 **/
void GPIO_PWM_Init()
{
	
GPIO_InitTypeDef GPIO_InitStructure; //����һ���ṹ�������������ʼ�� GPIO
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//����һ���ṹ�������������ʼ����ʱ��
TIM_OCInitTypeDef TIM_OCInitStructure;//���� TIM_OCInitStruct ��ָ���Ĳ�����ʼ������ TIMx

/* ����ʱ�� */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  // ����GPIO���ʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  // ������ʱ��5��ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); // AFIO���ù���ģ��ʱ��
	
/* ���� GPIO ��ģʽ�� IO �� */
GPIO_InitStructure.GPIO_Pin=PWM_OUT;  //PWM���� PA0
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
GPIO_Init(GPIOA,&GPIO_InitStructure);  // ͨ���ṹ�帳ֵ
	
//TIM3 ��ʱ����ʼ��
TIM_TimeBaseInitStructure.TIM_Period = Period; //�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;//����������Ϊ TIMx ʱ��Ƶ��Ԥ��Ƶֵ��100Khz ����Ƶ��
TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM ���ϼ���ģʽ
TIM_TimeBaseInit(TIM5, & TIM_TimeBaseInitStructure); // ��ֵ��ʱ��5

// ����ռ�ձȵ������
TIM_OCInitStructure.TIM_Pulse = Pulse; //��������ֵ�������������������ֵʱ����ƽ��������


//PWM ��ʼ�� //���� TIM_OCInitStruct ��ָ���Ĳ�����ʼ������ TIMx
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //����Ϊ PWM ģʽ 1 
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM���ʹ��

// ͨ��1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;   //����ʱ������ֵС�� CCR1_Val ʱΪ�͵�ƽ
TIM_OC1Init(TIM5,&TIM_OCInitStructure);//ע��˴���ʼ��ʱ TIM_OC2Init ������ TIM_OCInit������������Ϊ�̼���İ汾��һ����
TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ��TIMx �� CCR2 �ϵ�Ԥװ�ؼĴ���


TIM_Cmd(TIM5,ENABLE);//ʹ�ܻ���ʧ�� TIMx ����
	
}


 /**
* @ Function Name :  SetPwm
* @ Author        : ygl
* @ Brief         : ����Ƶ�ʺ�ռ�ձ�
* @ Date          : 2017.09.19
* @ Input          :
           u16 fre :  �������Ƶ��
           u16 cycle  �������ռ�ձ�
* @ Modify        : 
 **/

void SetPwm(u16 fre,u16 cycle)
{
	// 72��Ƶ�󣬻���Ƶ��1MHZ
 Prescaler = 72 - 1;
	// ��������Ӧ���Ƕ��� ��λ��HZ
 Period = Prescaler *1000000/ fre - 1;
	
	// ����ռ�ձ�
	Pulse = (Period+1) * cycle/100;
	
	}


