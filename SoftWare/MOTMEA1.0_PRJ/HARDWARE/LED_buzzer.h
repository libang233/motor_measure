#ifndef __LED_BUZZER_H_
#define __LED_BUZZER_H_


#define LED0ON  GPIO_ResetBits(GPIOB,GPIO_Pin_5);					//PE.5 输出低 点亮
#define LED0OFF GPIO_SetBits(GPIOB,GPIO_Pin_5);						//PE.5 输出高 熄灭

#define LED1ON  GPIO_ResetBits(GPIOB,GPIO_Pin_6);					//PE.6 输出低 点亮
#define LED1OFF GPIO_SetBits(GPIOB,GPIO_Pin_6);						//PE.6 输出高 熄灭

#define buzzerOFF  GPIO_ResetBits(GPIOB,GPIO_Pin_5);				//PE.4 输出低 关闭
#define buzzerON   GPIO_SetBits(GPIOB,GPIO_Pin_5);					//PE.5 输出高 开启
 
//LED和蜂鸣器初始化
void LED_buzzer_init(void);
	
#endif
