#include "PWM.h"


// 分频 周期 占空比
u16 Prescaler = 0;
u16 Period = 0;
u16 Pulse = 0;

 /**
* @ Function Name :  GPIO_PWM_Init
* @ Author        : ygl
* @ Brief         : PWM引脚初始化
* @ Date          : 2017.09.19
* @ Modify        : 
 **/
void GPIO_PWM_Init()
{
	
GPIO_InitTypeDef GPIO_InitStructure; //声明一个结构体变量，用来初始化 GPIO
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//声明一个结构体变量，用来初始化定时器
TIM_OCInitTypeDef TIM_OCInitStructure;//根据 TIM_OCInitStruct 中指定的参数初始化外设 TIMx

/* 开启时钟 */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  // 开启GPIO组的时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  // 开启定时器5的时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); // AFIO复用功能模块时钟
	
/* 配置 GPIO 的模式和 IO 口 */
GPIO_InitStructure.GPIO_Pin=PWM_OUT;  //PWM引脚 PA0
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
GPIO_Init(GPIOA,&GPIO_InitStructure);  // 通过结构体赋值
	
//TIM3 定时器初始化
TIM_TimeBaseInitStructure.TIM_Period = Period; //设置自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;//设置用来作为 TIMx 时钟频率预分频值，100Khz 计数频率
TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM 向上计数模式
TIM_TimeBaseInit(TIM5, & TIM_TimeBaseInitStructure); // 赋值定时器5

// 设置占空比的填充数
TIM_OCInitStructure.TIM_Pulse = Pulse; //设置跳变值，当计数器计数到这个值时，电平发生跳变


//PWM 初始化 //根据 TIM_OCInitStruct 中指定的参数初始化外设 TIMx
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //配置为 PWM 模式 1 
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能

// 通道1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;   //当定时器计数值小于 CCR1_Val 时为低电平
TIM_OC1Init(TIM5,&TIM_OCInitStructure);//注意此处初始化时 TIM_OC2Init 而不是 TIM_OCInit，否则会出错。因为固件库的版本不一样。
TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);//使能或者失能TIMx 在 CCR2 上的预装载寄存器


TIM_Cmd(TIM5,ENABLE);//使能或者失能 TIMx 外设
	
}


 /**
* @ Function Name :  SetPwm
* @ Author        : ygl
* @ Brief         : 设置频率和占空比
* @ Date          : 2017.09.19
* @ Input          :
           u16 fre :  设置输出频率
           u16 cycle  设置输出占空比
* @ Modify        : 
 **/

void SetPwm(u16 fre,u16 cycle)
{
	// 72分频后，基础频率1MHZ
 Prescaler = 72 - 1;
	// 计算周期应该是多少 单位是HZ
 Period = Prescaler *1000000/ fre - 1;
	
	// 计算占空比
	Pulse = (Period+1) * cycle/100;
	
	}


