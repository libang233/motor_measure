#include "FG.h"


u8  TIMXCHX_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIMXCHX_CAPTURE_VAL;	//���벶��ֵ
u16 ReadValue1,ReadValue2; 

 /**
* @ Function Name :   GPIO_FG_Init
* @ Author        : ygl
* @ Brief         : FG���ų�ʼ��
* @ Date          : 2017.09.19
* @ Modify        : 
 **/
 
void GPIO_FG_Init()
{	 
	// ����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB1PeriphClockCmd(CAP_TIM_RCC, ENABLE);	//ʹ��TIMʱ��                                                                          // ** ������Ķ�ʱ�� TIMX
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = FG1;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,FG1);						 //PA0 ����
	
	//��ʼ������ʱ�� 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(CAP_TIM, &TIM_TimeBaseStructure);                                                                                     // ** ������Ķ�ʱ�� TIMX
  
	//��ʼ��TIM���벶�����
	TIM_ICInitStructure.TIM_Channel =  CAP_TIM_Channel;                                              // ����ѡ��ͨ��  ��Ϊ TIM_Channel_X
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ���������ѡ���ͨ��ӳ�䵽�ܽ�
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(CAP_TIM, &TIM_ICInitStructure);                                                                                           // ** ������Ķ�ʱ�� TIMX
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = CAP_TIM_NVIC;  //TIM�ж�                                                                        // ** ������Ķ�ʱ�� TIMX
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  
	
	TIM_ITConfig(CAP_TIM,TIM_IT_Update|CAP_TIM_IT_Channel,ENABLE);//��������ж� ,����CC1IE�����ж�	        // ����ѡ��ͨ�� ��Ϊ  TIM_IT_CCX     // ** ������Ķ�ʱ�� TIMX
	
  TIM_Cmd(CAP_TIM,ENABLE ); 	//ʹ�ܶ�ʱ��                                                                                                 // ** ������Ķ�ʱ�� TIMX
   
}



// ������ƽ��ֵ�����飬��10�βɼ���ƽ��ֵ
#define TempLen 10
float Freq_value=0;  //Ƶ�ʸ���ֵ
u32 Freq[TempLen];        //Ƶ��ֵ��������
u32 Freq_Sum=0;      //
u32 Overflow_ptr=0;  //�������ֵ
u8 Freq_ptr1=0;      //�˲�����ֵ1
u8 Freq_ptr2=0;      //�˲�����ֵ2
u8 Show_flag=0;      //Ƶ��ֵ��ʾ��־λ



 /**
* @ Function Name : Freq_Sample
* @ Author        : ygl
* @ Brief         : Ƶ�ʲ����˲�����
* @ Date          :  2017.09.19
* @ Modify        : һ��Ҫִ��������������м��������״̬
 **/
void Freq_Sample(void)
{
 u32 Dtemp=0; 
 u8 i;
 //���������θߵ�ƽ
 if(TIMXCHX_CAPTURE_STA&0X80)
 {
  //�˲�����
  Freq_ptr1++;
  Freq_ptr2++;
  if(Freq_ptr1>=TempLen)
    Freq_ptr1=0;    
	// ���������ȡ����
  Dtemp = TIMXCHX_CAPTURE_STA&0X3F;
  Dtemp *= 65536;//���ʱ���ܺ�  
	// û����������
  if(Dtemp<=65536)  //�������С��2
    Dtemp =TIMXCHX_CAPTURE_VAL;
  else 
    Dtemp = Dtemp-65536+TIMXCHX_CAPTURE_VAL;
     
  Freq[Freq_ptr1] = Dtemp;
  Dtemp=0;
  
  if(Freq_ptr2>=TempLen)
  {
    for(i=0;i<TempLen;i++)
        Dtemp += Freq[i];     
    Freq_value = 10000000.0/Dtemp;
    Freq_ptr2=TempLen;
  }
	//�˲���Ѳ���״̬�򿪣�������ܼ����ɼ�
  TIMXCHX_CAPTURE_STA=0;
  Overflow_ptr=0;
 }
 else //δ����ȴ�����
 {
 	Overflow_ptr++;
 	if(Overflow_ptr>720000)
 	{
 	 Freq_value=Freq_value/10;
 	 Show_flag=1;
 	 Overflow_ptr=0;
 	}		
 }
 
}



 /**
* @ Function Name : show_frequency
* @ Author        : ygl
* @ Brief         : ��ʾƵ��
* @ Date          :  2017.09.19
* @ Modify        : 
 **/
/**
void show_frequency(void)
{
 float Ftemp;
 Ftemp=Freq_value;
	// Ftemp=100;
 if(Ftemp>10000.0)
 printf("%5.2f KHz\r\n",Ftemp/1000.0);   
 else if((Ftemp>1000.0)&&(Ftemp<10000.0))
 	printf("%5.3f KHz\r\n",Ftemp/1000.0);
 else if((Ftemp<1000.0)&&(Ftemp>100.0))
	printf("%5.1f Hz\r\n ",Ftemp);
 else if((Ftemp<100.0)&&(Ftemp>10.0))
  printf("%5.2f Hz \r\n ",Ftemp);
 else 
 printf("%5.3f Hz \r\n",Ftemp);
 Show_flag=0;
}

**/


  

 /**
* @ Function Name : CAP_TIM_Handler
* @ Author        : ygl
* @ Brief         : ��ʱ�������ж�
* @ Date          :  2017.09.19
* @ Modify        : 
     �ж˴�����������
     1�� ����Ĵ������񵽸ߵ�ƽ
     2�� ����Ĵ������
 **/
void CAP_TIM_Handler(void)
{ 
	//��δ�ɹ��������
 	if((TIMXCHX_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(CAP_TIM, TIM_IT_Update) != RESET)                                                                         // ** ������Ķ�ʱ�� TIMX
		{
			//�Ѿ����񵽸ߵ�ƽ��
			if(TIMXCHX_CAPTURE_STA&0X40)
			{ 
				// �ܹ����������ֻ��5λ ���ȫ���������0x3F  0110 1111
				if((TIMXCHX_CAPTURE_STA&0X3F)==0X3f)
				{
					//��������ٴμ��� ֻ��ǿ�ƽ�������һֱͣ�������ֵ
					TIMXCHX_CAPTURE_STA=0X80;
					TIMXCHX_CAPTURE_VAL=0;
				}
				else 
					// ������״̬�� ֱ�����
					TIMXCHX_CAPTURE_STA++;
			}
		}
		//����1���������¼� 
	 if (TIM_GetITStatus(CAP_TIM, CAP_TIM_IT_Channel) != RESET)                          // ����ѡ���ͨ��   TIM_IT_CCX         // ** ������Ķ�ʱ�� TIMX
		{	
			//���ڶ��λ������أ���Ϊ��һ�β����ʱ���TIMXCHX_CAPTURE_STA��0 ����Ϊ0X40  
			// ���Եڶ�����������жϵ�ʱ��״̬����0x40
			if(TIMXCHX_CAPTURE_STA&0X40)		
			{ 
				// ��ȡ����Ĵ�����ֵ
				ReadValue2 = CAP_Get_Channel(CAP_TIM);                                           // ����ѡ���ͨ��   TIM_GetCaptureX         // ** ������Ķ�ʱ�� TIMX
				// ��Ϊ��֪���ڶ��β���ʲôʱ���� �����ܻ���������
				if ((ReadValue2 > ReadValue1)&&(TIMXCHX_CAPTURE_STA==0x40))  //�����
        {
					// ������ľ���2�β����ʱ�� 
          TIMXCHX_CAPTURE_VAL = (ReadValue2 - ReadValue1); 
        }
        else  //�����
        {
					// ����Ӧ����vl2-vl1 ,��������� �����������ʱ��val2���� 0xffff
					// �������ʱ��Ͱ�val2����Ϊ0��ֱ������
					// ע������ļĴ�����5λ������5λ�Ͳ���val������� 
          TIMXCHX_CAPTURE_VAL =  ((0xFFFF - ReadValue1) +ReadValue2);
        }
				// �ڶ��β�������Ϊ���������ǿ�ƽ�������
				TIMXCHX_CAPTURE_STA|=0X80;		//��ǳɹ�����һ��������
			}
			//��һ�β��������أ���Ϊ��һ�ε�ʱ�� TIMXCHX_CAPTURE_STA��Ϊ0 
			else  								
			{ 
				// �ѵ�һ�β���Ĵ�����ֵȡ����
				ReadValue1=CAP_Get_Channel(CAP_TIM);                                               // ����ѡ�����ͨ�� TIM_GetCaptureX          // ** ������Ķ�ʱ�� TIMX
				//TIM5CH1_CAPTURE_STA=0;			
				TIMXCHX_CAPTURE_VAL=0;  // ���
				TIMXCHX_CAPTURE_STA = 0X40;		//��ǲ����������أ��Ƚ���ڶ��β���
			}
		}
 	} 
  TIM_ClearITPendingBit(CAP_TIM, CAP_TIM_IT_Channel|TIM_IT_Update); //����жϱ�־λ        //����ѡ�����ͨ��   TIM_IT_CCX              // ** ������Ķ�ʱ�� TIMX
 
}
