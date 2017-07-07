#ifndef __SYS_MANAGER_H_
#define __SYS_MANAGER_H_

#include "stm32f10x_conf.h"
#include "config.h"
#include "newtype.h"


//测试界面数据外部声明
extern testing_Information_TypeDef testingInformation;

//校准界面数据外部声明
extern adjust_Information_TypeDef adjustInformation;

//配置界面数据外部声明
extern config_Information_TypeDef configInformation;

//密码界面数据外部声明
extern password_Information_TypeDef passwordInformation;

//调试界面数据外部声明
extern debug_Information_TypeDef debugInformation;

//开始界面数据外部声明
extern begin_Information_TypeDef beginInformation;

// 系统初始化
void sys_manager_init(void);
// 系统主循环
void sys_manager_handle(void);


#endif
