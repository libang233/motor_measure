#include "CUR.h"

u16 cur_data;

 /**
* @ Function Name :  GPIO_CurMeasure_Init
* @ Author        : ygl
* @ Brief         : �����ɼ����ų�ʼ��
* @ Date          : 2017.09.19
* @ Modify        : 
 **/
void GPIO_CurMeasure_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_AFIO|RCC_APB2Periph_ADC3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12M  ���14M ����ADCʱ�ӣ�ADCCLK��
	ADC_DeInit(ADC3);

	GPIO_InitStructure.GPIO_Pin=CUR_SIM;//ADC  //1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStructure);	  //A


	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_239Cycles5); //1
	
	//�ڲ��¶ȴ���������ADC1ͨ��16�ġ�
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_239Cycles5);
//	ADC_TempSensorVrefintCmd(ENABLE);//���ڲ��¶ȴ�����ʹ��

	ADC_DMACmd(ADC3,ENABLE);//��ADC��DMA������һ��
	ADC_Cmd(ADC3,ENABLE);	

	ADC_ResetCalibration(ADC3);//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC3));//��ȡADC����У׼�Ĵ�����״̬	
	ADC_StartCalibration(ADC3);//��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC3));//��ȡָ��ADC��У׼����
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);//ʹ�ܻ���ʧ��ָ����ADC�����ת����������

	DMA_DeInit(DMA2_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC3->DR;//DMA�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&cur_data;//DMA�ڴ��ַ
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//������Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize=1;//ָ��DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc=DMA_PeripheralInc_Enable;//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//�������ݿ��16
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//�洢���ݿ��16
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//DMAͨ��xӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA2_Channel5,&DMA_InitStructure);	//ADC1��DMA1ͨ��1��
	DMA_Cmd(DMA2_Channel5,ENABLE);//ʹ��DMA1
	
}
