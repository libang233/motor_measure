#ifndef __NEWTYPE_H_
#define __NEWTYPE_H_

#include "config.h"
#include "extern.h"

#define MAX_BYTE_NUM_SPECIF_EXPLAIN				20					//规格项说明字节数
#define MAX_PN_NUM_MAX_QUANTITY					50					//最大PN号个数
#define MAX_BYTE_NUM_PN_NUM						20					//PN号字节数
#define MAX_BYTE_NUM_PN_NUM_DESCRIP				20					//PN号补充描述字节数
#define MAX_BYTE_NUM_DISPLAY_SPEED 				20					//显示的转速字节数
#define MAX_BYTE_NUM_DISPLAY_CURRENT			20					//显示的电流值字节数
#define MAX_BYTE_NUM_DISPLAY_PWM				20					//显示的PWM字节数
#define MAX_BYTE_NUM_NOW_TEST_MES				20					//显示的当前测试信息的字节数
#define MAX_BYTE_NUM_DISPLAY_PWM_DUTY			20					//显示的PWM信号占空比字节数
#define MAX_BYTE_NUM_PASSWORD					20					//密码字节数
#define MAX_BYTE_NUM_PASSWORD_TIP				20					//密码提示字节数


#define MAX_PART_NUM							6

#define MAX_CUR_DEC_BIT							10					//电流最大十进制位数

#define TEST_PART1								0
#define TEST_PART2								1
#define TEST_PART3								2
#define TEST_PART4								3
#define TEST_PART5								4
#define	TEST_PART6								5

//
//配置测试段数据结构定义
//
typedef struct
{
	u8 explain[MAX_BYTE_NUM_SPECIF_EXPLAIN];	//规格项说明
	u8 explainStringIdx;						//说明数据字符数
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
	part_Information_TypeDef partInformation[MAX_PART_NUM];
	
	u8 PNNum[MAX_PN_NUM_MAX_QUANTITY][MAX_BYTE_NUM_PN_NUM];		//PN号列表
	u8 PNNumStringIdx[MAX_PN_NUM_MAX_QUANTITY];					//PN号字符串索引
	u8 PNNumDisplay[MAX_BYTE_NUM_PN_NUM];						//显示的PN号
	u8 PNNumDisplayIdx;											//前一项字符串索引	
	u8 PNNumQuantity;											//PN号的总数
	u8 PNNumDescription[MAX_BYTE_NUM_PN_NUM_DESCRIP];			//PN号的补充描述
	u8 descriptionStringIdx;									//描述字符串索引
	u8 polesNum;												//风扇的极数
	u8 partNum;													//设置的测试段数
	u8 PNNumSelect;												//料号选择
	u8 powerSelect;												//电源选择	
	u16 PWMSignFrequen;											//pwm调速信号频率
	u16 testCurrent;											//系统判定是否插入风扇的临界电流值
	bool trigger;												//触发选择，1 = 触发，0 = 不触发	

	bool isDeleteButtonDown;									//删除按键是否按下									
	bool isSaveButtonDown;										//存储按键是否按下
	bool isConfig;												//是否为配置状态
	bool isPartUpdate;											//段是否隐藏更新	false = 未更新 true = 已更新
	bool isDataUpdate;											//料号对应数据是否已更新 false = 未更新 true = 已更新
	bool isDataPull;											//屏幕数据是否开始下拉
	bool isDataReced;											//数据是否接收完毕
	
	bool testFlag;															
	
}config_Information_TypeDef;


//
//段测试完成时显示的数据结构定义
//
typedef struct
{
	u8  PWM[MAX_BYTE_NUM_DISPLAY_PWM];							//测试完成时显示的占空比值
	u8  PWMStringIdx;											//前一项字符串索引	
	u16 speed[MAX_BYTE_NUM_DISPLAY_SPEED];						//测试完成时显示的转速值
	u8  speedStringIdx;											//前一项字符串索引	
	u16 current[MAX_BYTE_NUM_DISPLAY_CURRENT];					//测试完成时显示的电流值
	u8  currentStringIdx;										//前一项字符串索引	
	
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
	
	u16 testProgressBar;										//测试时当前进度值
	u16 testProgressBarMax;										//最大进度值 
	u8  nowPWM[MAX_BYTE_NUM_DISPLAY_PWM];						//测试时pwm信号占空比即时显示
	u8  nowPWMStringIdx;										//前一项字符串索引	
	u16 nowSpeed[MAX_BYTE_NUM_DISPLAY_SPEED];					//测试时显示当前测试风扇转速值，单位rpm
	u8  nowSpeedStringIdx;										//前一项字符串索引
	u16 nowCurrent[MAX_BYTE_NUM_DISPLAY_CURRENT];				//测试时显示当前测试风扇电流值，单位ma
	u8  nowCurrentStringIdx;									//前一项字符串索引
	u8  nowTestMessage[MAX_BYTE_NUM_NOW_TEST_MES];				//显示当前测试信息 格式为“正在测试第N段，共M段
	u8  nowTestMessageStringIdx;								//前一项字符串索引
	u8  finishPWM[MAX_BYTE_NUM_DISPLAY_PWM];					//测试完成时占空比显示项表头
	u8  finishPWMStringIdx;										//前一项字符串索引
	u16 finishSpeed[MAX_BYTE_NUM_DISPLAY_SPEED];				//测试完成时转速显示项表头
	u8  finishSpeedStringIdx;									//前一项字符串索引
	u16 finishCurrent[MAX_BYTE_NUM_DISPLAY_CURRENT];			//测试完成时电流显示项表头
	u8  finishCurrentStringIdx;									//前一项字符串索引
	u8  resultBitmapOK;											//测试结束时，测试结果显示OK部分
	u8  resultBitmapNG;				 							//测试结束时，测试结果显示NG部分
	
}testing_Information_TypeDef;

//
//校准界面数据结构定义
//
typedef struct
{
	
	u16 adjustParameter;										//电流误差
	u8  signOfadjustParameter;									//电流误差的符号 0 = 正  1 = 负
	u16 measureCurrent;											//测量电流数值
	u16 inputCurrent;											//输入电流数值
	u8  measureCurrentString[MAX_BYTE_NUM_DISPLAY_CURRENT];		//校准是测量到的电流值
	u8  measureCurrentStringIdx;								//前一项字符串索引
	u8  inputCurrentString[MAX_BYTE_NUM_DISPLAY_CURRENT];		//输入实际测量的标准电流值
	u8  inputCurrentStringIdx;									//前一项字符串索引
	u32  timer;													//计数器
	bool isConnection;											//是否连接
	bool isAutoZeroButtonDown;									//电流自动校零按键
	bool isSaveArgButtonDown;									//存储校准参数按键
	bool isDataPull;											//屏幕数据是否开始下拉
	bool isDataReced;											//数据是否接收完毕	
	
}adjust_Information_TypeDef;


//
//密码界面数据结构定义
//
typedef struct 
{
	u8 inputPassword[MAX_BYTE_NUM_PASSWORD];			//进入配置界面时输入的密码
	u8 inputPasswordStringIdx;							//前一项字符串索引
	u8 oldPassword[MAX_BYTE_NUM_PASSWORD];				//密码更新时，输入原始密码
	u8 oldPasswordStringIdx;							//前一项字符串索引
	u8 newPassword[MAX_BYTE_NUM_PASSWORD];				//密码更新时，输入新密码
	u8 newPasswordStringIdx;							//前一项字符串索引
	u8 oldPasswordTip[MAX_BYTE_NUM_PASSWORD_TIP];		//旧密码输入提示字符
	u8 oldPasswordTipStringIdx;							//前一项字符串索引
	u8 newPasswordTip[MAX_BYTE_NUM_PASSWORD_TIP];		//新密码输入提示字符
	u8 newPasswordTipStringIdx;							//前一项字符串索引		
	u8 realPassword[MAX_BYTE_NUM_PASSWORD];				//正确的密码
	u8 realPasswordStringIdx;							//前一项字符串索引
	
	bool isOkButtonDown;								//是否按下密码确定键
	bool isChangeButtonDown;							//是否按下密码修改键
	bool isChangeOkButtonDown;							//是否按下密码修改OK键
	bool isGetOldPassword;								//是否以接收到旧密码
	bool isGetNewPassword;								//是否接收到新密码
	bool isDataPullInput;								//是否完成下拉输入的密码
	bool isDataPullNew;									//是否完成下拉新密码
	
	
}password_Information_TypeDef;

//
//调试界面数据结构定义
//
typedef struct
{
	u16 timer;												//刷新计时器
	u8  button;												//删除料号按键
	u16 speed;
	u8 speedString[MAX_BYTE_NUM_DISPLAY_SPEED];				//调试界面转速显示
	u8  speedStringIdx;										//前一项字符串索引
	u8  currentString[MAX_BYTE_NUM_DISPLAY_CURRENT];		//调试界面电流显示	
	u16 current;
	u8  currentStringIdx;									//前一项字符串索引
	u16 inputPoles;											//调试界面风扇极数输入
	u16 inputPWMFrequen;									//调试界面pwm信号频率输入
	u8  inputPWM;											//调试界面pwm信号占空比输入
	
	bool isSetPWM;											//是否需要设置PWM
	
}debug_Information_TypeDef;

//
//时间数据结构定义
//
typedef struct
{
	u8 hour;
	u8 minute;
	u8 second;
	
}begin_Page_Time_TypeDef;

//
//日期数据结构定义
//
typedef struct
{
	u16 year;
	u8  month;
	u8  day;
	u8  week;
	
}begin_Page_Date_TypeDef;

//
//开始界面数据结构定义
//
typedef struct 
{
	begin_Page_Time_TypeDef time;	//时间显示
	begin_Page_Date_TypeDef date;	//日期显示
	
	u8 goDebugPageButton;			//进入调试界面按键
	u8 goTestPageButton;			//进入测试界面按键
	u8 goConfigPageButton;			//进入配置界面按键
	u8 goAdjustPageButton;			//进入校准界面按键
	u8 goBrowsePageButton;			//进入浏览界面按键
	
}begin_Information_TypeDef;

//
//全局信息数据结构定义
//
typedef struct
{
	u8 nowPage;						//当前页面
	bool isPageInit;				//界面是否初始化	false = 未初始化 true = 已初始化
	
}global_Information_TypeDef;

#endif
