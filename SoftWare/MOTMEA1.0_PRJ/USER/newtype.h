#ifndef __NEWTYPE_H_
#define __NEWTYPE_H_

#include "config.h"
#include "extern.h"

#define MAX_BYTE_NUM_SPECIF_EXPLAI				20					//规格项说明字节数
#define MAX_BYTE_NUM_PN_NUM						20					//PN号字节数
#define MAX_BYTE_NUM_PN_NUM_DESCRIP				20					//PN号补充描述字节数
#define MAX_BYTE_NUM_DISPLAY_SPEED 				20					//显示的转速字节数
#define MAX_BYTE_NUM_DISPLAY_CURRENT			20					//显示的电流值字节数
#define MAX_BYTE_NUM_DISPLAY_PWM				20					//显示的PWM字节数
#define MAX_BYTE_NUM_NOW_TEST_MES				20					//显示的当前测试信息的字节数
#define MAX_BYTE_NUM_DISPLAY_PWM_DUTY			20					//显示的PWM信号占空比字节数
#define MAX_BYTE_NUM_PASSWORD					20					//密码字节数
#define MAX_BYTE_NUM_PASSWORD_TIP				20					//密码提示字节数


//
//全局事件标志位
//
union whole_Evnt_TypeDef
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
		volatile u8 IsTestUsart					:1;		//校准程序标志位
		
		
	}Bit;
};

//
//配置测试段数据结构定义
//
typedef struct
{
	u8 explain[MAX_BYTE_NUM_SPECIF_EXPLAI];		//规格项说明
	u8 testPWM;									//测试pwm信号占空比值
	u8 testTime;								//测试时间，单位为秒
	u16 testSpeedUpLimit;						//测试转速范围的上限，单位RPM
	u16 testSpeedDownLimit;						//测试转速范围的下限，单位RPM
	u16 testCurrentUpLimit;						//第一段测试电流范围的上限，单位毫安
	u16 testCurrentDownLimit;					//第一段测试电流范围的下限，单位毫安
	
}part_Information_TypeDef;

//
//配置数据结构定义
//
typedef struct 
{
	//测试段数据
	part_Information_TypeDef part1Information;
	part_Information_TypeDef part2Information;
	part_Information_TypeDef part3Information;
	part_Information_TypeDef part4Information;
	part_Information_TypeDef part5Information;
	part_Information_TypeDef part6Information;

	u8 materialNum[MAX_BYTE_NUM_PN_NUM];						//也称为PN号，区别不同规格的唯一识别码
	u8 description[MAX_BYTE_NUM_PN_NUM_DESCRIP];				//PN号的补充描述
	u8 polesNum;												//风扇的极数
	u8 partNum;													//设置的测试段数
	u8 trigger;													//触发选择，1 = 触发，0 = 不触发
	u16 testCurrent;											//系统判定是否插入风扇的临界电流值
	u8 PWM;														//pwm调速信号频率
	
}config_Information_TypeDef;


//
//段测试完成时显示的数据结构定义
//
typedef struct
{
	u8  PWM[MAX_BYTE_NUM_DISPLAY_PWM];							//测试完成时显示的占空比值
	u16 speed[MAX_BYTE_NUM_DISPLAY_SPEED];						//测试完成时显示的转速值
	u16 current[MAX_BYTE_NUM_DISPLAY_CURRENT];					//测试完成时显示的电流值
	
}part_Finish_Value_TypeDef;

//
//测试数据结构体定义
//
typedef struct
{
	//段测试完成时显示的数据
	part_Finish_Value_TypeDef part1FinishValule;
	part_Finish_Value_TypeDef part2FinishValule;
	part_Finish_Value_TypeDef part3FinishValule;
	part_Finish_Value_TypeDef part4FinishValule;
	part_Finish_Value_TypeDef part5FinishValule;
	part_Finish_Value_TypeDef part6FinishValule;
	
	u8 PWMProgressbar;											//测试时显示当前pwm信号占空比值进度条
	u8 nowPWM[MAX_BYTE_NUM_DISPLAY_PWM];						//测试时pwm信号占空比即时显示
	u16 nowSpeed[MAX_BYTE_NUM_DISPLAY_SPEED];					//测试时显示当前测试风扇转速值，单位rpm
	u16 nowCrurrent[MAX_BYTE_NUM_DISPLAY_CURRENT];				//测试时显示当前测试风扇电流值，单位ma
	u8 nowTestMessage[MAX_BYTE_NUM_NOW_TEST_MES];				//显示当前测试信息 格式为“正在测试第N段，共M段
	u8 finishPWM[MAX_BYTE_NUM_DISPLAY_PWM];						//测试完成时占空比显示项表头
	u16 finishSpeed[MAX_BYTE_NUM_DISPLAY_SPEED];				//测试完成时转速显示项表头
	u16 finishCurrent[MAX_BYTE_NUM_DISPLAY_CURRENT];			//测试完成时电流显示项表头
	u8 resultOK;												//测试结束时，测试结果显示OK部分
	u8 reasultNG;												//测试结束时，测试结果显示NG部分
	
}testing_Information_TypeDef;

//
//校准界面数据结构定义
//
typedef struct
{
	u16 nowCurrent[MAX_BYTE_NUM_DISPLAY_CURRENT];				//校准是测量到的电流值
	u16 testCurrent[MAX_BYTE_NUM_DISPLAY_CURRENT];				//输入实际测量的标准电流值
	u8  autoZeroButton;											//电流自动校零按键
	
}adjust_Information_TypeDef;


//
//密码界面数据结构定义
//
typedef struct 
{
	u8 inputPassword[MAX_BYTE_NUM_PASSWORD];			//进入配置界面时输入的密码
	u8 oldPassword[MAX_BYTE_NUM_PASSWORD];				//密码更新时，输入原始密码
	u8 newPassword[MAX_BYTE_NUM_PASSWORD];				//密码更新时，输入新密码
	u8 oldPasswordTip[MAX_BYTE_NUM_PASSWORD_TIP];		//旧密码输入提示字符
	u8 newPasswrodTip[MAX_BYTE_NUM_PASSWORD_TIP];		//新密码输入提示字符
	
}password_Information_TypeDef;

//
//调试界面数据结构定义
//
typedef struct
{
	u8  button;						//删除料号按键
	u16 speed[20];					//调试界面转速显示
	u8  current[20];				//调试界面电流显示		
	u32 inputPoles;					//调试界面风扇极数输入
	u32 inputPWMFrequen;			//调试界面pwm信号频率输入
	u8  inputPWM;					//调试界面pwm信号占空比输入
	
}debug_Information_TypeDef;

//
//开始界面数据结构定义
//
typedef struct 
{
	u8 time;						//时间显示
	u8 date;						//日期显示
	u8 goDebugPageButton;			//进入调试界面按键
	u8 goTestPageButton;			//进入测试界面按键
	u8 goConfigPageButton;			//进入配置界面按键
	u8 goAdjustPageButton;			//进入校准界面按键
	u8 goBrowsePageButton;			//进入浏览界面按键
	
}begin_Information_TypeDef;

#endif
