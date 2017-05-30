#ifndef __SYS_MANAGER_H_
#define __SYS_MANAGER_H_

#include "stm32f10x_conf.h"
#include "config.h"


// 系统初始化
void sys_manager_init(void);
// 系统主循环
void sys_manager_handle(void);


#endif
