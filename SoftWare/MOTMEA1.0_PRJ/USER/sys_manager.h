#ifndef __SYS_MANAGER_H_
#define __SYS_MANAGER_H_

#include "stm32f10x_conf.h"
#include "config.h"
#include "newtype.h"

extern testing_Information_TypeDef testingInformation;

//测试段数据外部声明
extern part_Information_TypeDef part1Information;

extern part_Information_TypeDef part2Information;

extern part_Information_TypeDef part3Information;

extern part_Information_TypeDef part4Information;

extern part_Information_TypeDef part5Information;

extern part_Information_TypeDef part6Information;

//测试界面数据外部声明
extern testing_Information_TypeDef testingInformation;

//校准界面数据外部声明
extern adjust_Information_TypeDef adjustInformation;

//配置界面数据外部声明
extern config_Information_TypeDef configInformation;

//密码界面数据外部声明
extern password_Information_TypeDef passwordInformation;

//调试界面数据实例化
extern debug_Information_TypeDef debugInformation;



// 系统初始化
void sys_manager_init(void);
// 系统主循环
void sys_manager_handle(void);


#endif
