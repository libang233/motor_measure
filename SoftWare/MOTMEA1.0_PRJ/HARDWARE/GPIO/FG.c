#include "FG.h"


u8  TIMXCHX_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIMXCHX_CAPTURE_VAL;	//输入捕获值
u16 ReadValue1,ReadValue2; 

 /**
* @ Function Name :   GPIO_FG_Init
* @ Author        : ygl
* @ Brief         : FG引脚初始化
* @ Date          : 2017.09.19
* @ Modify        : 
 **/
 
void GPIO_FG_Init()
{	 
	// 定义结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;

	RCC_APB1PeriphClockCmd(CAP_TIM_RCC, ENABLE);	//使能TIM时钟                                                                          // ** 这里更改定时器 TIMX
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = FG1;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,FG1);						 //PA0 下拉
	
	//初始化捕获定时器 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(CAP_TIM, &TIM_TimeBaseStructure);                                                                                     // ** 这里更改定时器 TIMX
  
	//初始化TIM输入捕获参数
	TIM_ICInitStructure.TIM_Channel =  CAP_TIM_Channel;                                              // 这里选择通道  改为 TIM_Channel_X
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 这里把上面选择的通道映射到管脚
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(CAP_TIM, &TIM_ICInitStructure);                                                                                           // ** 这里更改定时器 TIMX
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = CAP_TIM_NVIC;  //TIM中断                                                                        // ** 这里更改定时器 TIMX
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  
	
	TIM_ITConfig(CAP_TIM,TIM_IT_Update|CAP_TIM_IT_Channel,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	        // 这里选择通道 改为  TIM_IT_CCX     // ** 这里更改定时器 TIMX
	
  TIM_Cmd(CAP_TIM,ENABLE ); 	//使能定时器                                                                                                 // ** 这里更改定时器 TIMX
   
}



// 用于求平均值的数组，对10次采集求平均值
#define TempLen 10
float Freq_value=0;  //频率浮点值
u32 Freq[TempLen];        //频率值缓存数组
u32 Freq_Sum=0;      //
u32 Overflow_ptr=0;  //溢出计数值
u8 Freq_ptr1=0;      //滤波计数值1
u8 Freq_ptr2=0;      //滤波计数值2
u8 Show_flag=0;      //频率值显示标志位



 /**
* @ Function Name : Freq_Sample
* @ Author        : ygl
* @ Brief         : 频率采样滤波处理
* @ Date          :  2017.09.19
* @ Modify        : 一定要执行这个程序，里面有继续捕获的状态
 **/
void Freq_Sample(void)
{
 u32 Dtemp=0; 
 u8 i;
 //捕获了两次高电平
 if(TIMXCHX_CAPTURE_STA&0X80)
 {
  //滤波处理
  Freq_ptr1++;
  Freq_ptr2++;
  if(Freq_ptr1>=TempLen)
    Freq_ptr1=0;    
	// 把溢出次数取出来
  Dtemp = TIMXCHX_CAPTURE_STA&0X3F;
  Dtemp *= 65536;//溢出时间总和  
	// 没有溢出的情况
  if(Dtemp<=65536)  //溢出次数小于2
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
	//滤波后把捕获状态打开，后面才能继续采集
  TIMXCHX_CAPTURE_STA=0;
  Overflow_ptr=0;
 }
 else //未捕获等待清零
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
* @ Brief         : 显示频率
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
* @ Brief         : 定时器捕获中断
* @ Date          :  2017.09.19
* @ Modify        : 
     中端触发的条件：
     1） 捕获寄存器捕获到高电平
     2） 捕获寄存器溢出
 **/
void CAP_TIM_Handler(void)
{ 
	//还未成功捕获完成
 	if((TIMXCHX_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(CAP_TIM, TIM_IT_Update) != RESET)                                                                         // ** 这里更改定时器 TIMX
		{
			//已经捕获到高电平了
			if(TIMXCHX_CAPTURE_STA&0X40)
			{ 
				// 能够溢出计数的只有5位 如果全部溢出就是0x3F  0110 1111
				if((TIMXCHX_CAPTURE_STA&0X3F)==0X3f)
				{
					//溢出后不能再次计数 只能强制结束捕获，一直停留在这个值
					TIMXCHX_CAPTURE_STA=0X80;
					TIMXCHX_CAPTURE_VAL=0;
				}
				else 
					// 溢出后把状态加 直到溢出
					TIMXCHX_CAPTURE_STA++;
			}
		}
		//捕获1发生捕获事件 
	 if (TIM_GetITStatus(CAP_TIM, CAP_TIM_IT_Channel) != RESET)                          // 这里选择改通道   TIM_IT_CCX         // ** 这里更改定时器 TIMX
		{	
			//捕第二次获到上升沿，因为第一次捕获的时候把TIMXCHX_CAPTURE_STA从0 更改为0X40  
			// 所以第二次如果进入中断的时候，状态就是0x40
			if(TIMXCHX_CAPTURE_STA&0X40)		
			{ 
				// 读取捕获寄存器的值
				ReadValue2 = CAP_Get_Channel(CAP_TIM);                                           // 这里选择改通道   TIM_GetCaptureX         // ** 这里更改定时器 TIMX
				// 因为不知道第二次捕获什么时候来 ，可能会溢出的情况
				if ((ReadValue2 > ReadValue1)&&(TIMXCHX_CAPTURE_STA==0x40))  //无溢出
        {
					// 计算出的就是2次捕获的时间 
          TIMXCHX_CAPTURE_VAL = (ReadValue2 - ReadValue1); 
        }
        else  //有溢出
        {
					// 本来应该是vl2-vl1 ,但是有溢出 所以在溢出的时候val2等于 0xffff
					// 在溢出的时候就把val2重新为0，直到捕获
					// 注意溢出的寄存器是5位，超过5位就不在val进行相加 
          TIMXCHX_CAPTURE_VAL =  ((0xFFFF - ReadValue1) +ReadValue2);
        }
				// 第二次捕获后就认为捕获结束，强制结束捕获
				TIMXCHX_CAPTURE_STA|=0X80;		//标记成功捕获到一次上升沿
			}
			//第一次捕获上升沿，因为第一次的时候 TIMXCHX_CAPTURE_STA是为0 
			else  								
			{ 
				// 把第一次捕获寄存器的值取出来
				ReadValue1=CAP_Get_Channel(CAP_TIM);                                               // 这里选择更改通道 TIM_GetCaptureX          // ** 这里更改定时器 TIMX
				//TIM5CH1_CAPTURE_STA=0;			
				TIMXCHX_CAPTURE_VAL=0;  // 清空
				TIMXCHX_CAPTURE_STA = 0X40;		//标记捕获到了上升沿，等进入第二次捕获
			}
		}
 	} 
  TIM_ClearITPendingBit(CAP_TIM, CAP_TIM_IT_Channel|TIM_IT_Update); //清除中断标志位        //这里选择更改通道   TIM_IT_CCX              // ** 这里更改定时器 TIMX
 
}
