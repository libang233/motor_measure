#ifndef __NEWTYPE_H_
#define __NEWTYPE_H_

#include "config.h"
#include "extern.h"

//
//全局事件标志位
//
union whole_Evnt
{
	u32 ALL;
	struct
	{
		volatile u8 IsSerialMessageAnaly		:1;		//串口数据分析标志位
		volatile u8 IsTest                      :1;		//测试程序标志位
		volatile u8 IsConfig                    :1;		//配置程序标志位
		volatile u8 IsResearch					:1;		//查阅程序标志位
		volatile u8 IsDebug                     :1;		//调试程序标志位
		volatile u8 IsAdjust					:1;		//校准程序标志位
		
		
	}Bit;
};

//
//配置测试段数据结构定义
//
typedef struct
{
	u8 explain[20];					//规格项说明
	u8 testPWM;						//测试pwm信号占空比值
	u8 testTime;					//测试时间，单位为秒
	u16 testSpeedUpLimit;			//测试转速范围的上限，单位RPM
	u16 testSpeedDownLimit;			//测试转速范围的下限，单位RPM
	u16 testCurrentUpLimit;			//第一段测试电流范围的上限，单位毫安
	u16 testCurrentDownLimit;		//第一段测试电流范围的下限，单位毫安
	u8 finishPWM;					//测试完成时显示的占空比值
	u16 finishSpeed;				//测试完成时显示的转速值
	u16 finishCurrent;				//测试完成时显示的电流值
	
}Part_Information;

//
//测试数据结构体定义
//
typedef struct
{
	u8 PWMProgressbar;				//测试时显示当前pwm信号占空比值
	u8 nowPWM;						//测试时pwm信号占空比即时显示
	u16 nowSpeed;					//测试时显示当前测试风扇转速值，单位rpm
	u16 nowCrurrent;				//测试时显示当前测试风扇电流值，单位ma
	u8 nowTestMessage[20];			//显示当前测试信息 格式为“正在测试第N段，共M段”
	u8 finishPWM;					//测试完成时占空比显示项表头
	u16 finishSpeed;				//测试完成时转速显示项表头
	u16 finishCurrent;				//测试完成时电流显示项表头
	u8 resultOK;					//测试结束时，测试结果显示OK部分
	u8 reasultNG;					//测试结束时，测试结果显示NG部分
	
}testing_Information;

//
//校准界面数据结构定义
//
typedef struct
{
	u16 nowCurrent;					//校准是测量到的电流值
	u16 testCurrent;				//输入实际测量的标准电流值
	
}adjust_Information;

//
//配置数据结构定义
//
typedef struct 
{
	u8 materialNum[20];				//也称为PN号，区别不同规格的唯一识别码
	u8 description[20];				//PN号的补充描述
	u8 polesNum;					//风扇的极数
	u8 partNum;						//设置的测试段数
	u8 trigger;						//触发选择，1 = 触发，0 = 不触发
	u16 testCurrent;				//系统判定是否插入风扇的临界电流值
	u8 PWM;							//pwm调速信号频率
	
}config_Information;

//
//密码界面数据结构定义
//
typedef struct 
{
	u8 inputPassword[20];			//进入配置界面时输入的密码
	u8 oldPassword[20];				//密码更新时，输入原始密码
	u8 newPassword[20];				//密码更新时，输入新密码
	
	
	
}pass_Information;


void data_init();

#endif
