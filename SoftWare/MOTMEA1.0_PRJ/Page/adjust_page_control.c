/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : adjust_page_control.c
 * Author    : hlb
 * Brief     : 校准界面程序
 * Date      : 2017.07.25
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "adjust_page_control.h"
#include "sys_manager.h"
#include "message_analysis.h"
#include "message_send.h"
#include "sdcard_control.h"
#include "usart.h"
#include "CUR.h"
 
#define ADJUST_PUSH_CYCLE  100000

int ttt=0;

 /**
* @ Function Name : calculate_current_string
* @ Author        : hlb
* @ Brief         : 计算电流值的字符串
* @ Date          : 2017.09.18
* @ Input         : u16 cur      	电流
					u8  *val        电流字符串头
					u8  *valSize    字符串大小
* @ Modify        : ...
 **/
void calculate_current_string(u16 cur, u8 *val, u8 *valSize)
{
	u8 i;
	sprintf(val, "%d", cur);
	
	for(i = 1; i < MAX_CUR_DEC_BIT; i++)
	{
		if(cur / i*10 == 0)
		{
			break;
		}
	}
	
	*valSize = i;
}


 /**
* @ Function Name : calculate_current
* @ Author        : hlb
* @ Brief         : 计算电流值
* @ Date          : 2017.09.18
* @ Input         : u16 *cur      电流存储地址
					u8  *val      电流字符串头
					u8  valSize   字符串大小
* @ Output        : bool			是否正确处理
* @ Modify        : ...
 **/
bool calculate_curent(u16 *cur, u8 *val, u8 valSize)
{
	u8 i;
	u16 j;
	
	j = 0;
	
	for(i = valSize - 1; i >= 0; i--)
	{
		if(val[i] >= '0' && val[i] <= '9')
		{
			j *= 10;
			j += val[i] - '0';		
		}			
		else
		{
			return false;
		}
	}
	
	*cur = j;
	return true;
}

 
 /**
* @ Function Name : adjust_page_push_cur
* @ Author        : hlb
* @ Brief         : 发送电流
* @ Date          : 2017.09.20
* @ Modify        : ...
 **/ 
void adjust_page_push_cur(void)
{
	
	if(ttt++>100)
	{
		ttt=0;
	}
	
	cur_data = 0;
	
	if(adjustInformation.signOfadjustParameter == 0)
	{
		adjustInformation.measureCurrent = cur_data + adjustInformation.adjustParameter;
	}
	else
	{
		if(cur_data > adjustInformation.adjustParameter)
		{
			adjustInformation.measureCurrent = cur_data - adjustInformation.adjustParameter;
		}
		else
		{
			adjustInformation.measureCurrent = 0;
		}
	}
	
	adjustInformation.measureCurrent = ttt;
	
	//转换字符串
	calculate_current_string(adjustInformation.measureCurrent, adjustInformation.measureCurrentString, &adjustInformation.measureCurrentStringIdx);
	
	write_control_state(ID_ADJUST_MEA_CURRNT, ENABLE);
	
	//上传数据
	write_char(ID_ADJUST_MEA_CURRNT, adjustInformation.measureCurrentString, adjustInformation.measureCurrentStringIdx);
	
	write_control_state(ID_ADJUST_MEA_CURRNT, CONTROL_DISENABLE);
}
 

 /**
* @ Function Name : adjust_page_auto_zero
* @ Author        : hlb
* @ Brief         : 清零事件
* @ Date          : 2017.09.20
* @ Output        : bool			是否处理完毕
* @ Modify        : ...
 **/ 
void adjust_page_auto_zero(void)
{
	adjustInformation.inputCurrent = 0;
	
	adjustInformation.adjustParameter = adjustInformation.measureCurrent - adjustInformation.inputCurrent;
	
	if(adjustInformation.adjustParameter > 0)
	{
		adjustInformation.signOfadjustParameter = 1;
	}
	else
	{
		adjustInformation.signOfadjustParameter = 0;
	}
	
	//修改sd卡内数据
	sdcard_index_file_current_edit();		
}

 /**
* @ Function Name : adjust_page_save
* @ Author        : hlb
* @ Brief         : 存储事件
* @ Date          : 2017.09.20
* @ Output        : bool			是否处理完毕
* @ Modify        : ...
 **/ 
bool adjust_page_save(void)
{
	if(adjustInformation.isDataPull == true)
	{
		//下拉数据
		pull_control_data(ID_ADJUST_REA_CURRNT, FRAME_REC_DATA_STRING_DEF);
		
		adjustInformation.isDataPull = false;
	}
	if(adjustInformation.isDataReced == true)
	{
		//计算输入的电流
		if(calculate_curent(&adjustInformation.inputCurrent, adjustInformation.inputCurrentString, adjustInformation.inputCurrentStringIdx) == true)
		{
			//计算偏差
			if(adjustInformation.inputCurrent > adjustInformation.measureCurrent)
			{
				adjustInformation.adjustParameter = adjustInformation.inputCurrent - adjustInformation.measureCurrent;
				adjustInformation.signOfadjustParameter = 0;
			}
			else
			{
				adjustInformation.adjustParameter = adjustInformation.measureCurrent - adjustInformation.inputCurrent;
				adjustInformation.signOfadjustParameter = 1;
			}
		
			//修改sd卡内数据
			sdcard_index_file_current_edit();		
		}
		
		adjustInformation.isDataReced = false;
		
		return true;
	}
	
	return false;
}
 
 /**
* @ Function Name : adjust_page_init
* @ Author        : hlb
* @ Brief         : 配置界面线程
* @ Date          : 2017.09.20
* @ Modify        : ...
 **/
void adjust_page_init(void)
{
	//禁能控件
	write_control_state(ID_ADJUST_MEA_CURRNT, CONTROL_DISENABLE);
}	
	
 /**
* @ Function Name : adjust_page_handle
* @ Author        : hlb
* @ Brief         : 校准界面线程
* @ Date          : 2017.09.18
* @ Modify        : ...
 **/
void adjust_page_handle(void)
{
	
	if(globalInformation.isPageInit == false)
	{
		//初始化界面
		adjust_page_init();
		
		globalInformation.isPageInit = true;
	}
	
	if(adjustInformation.isAutoZeroButtonDown == true)
	{
		//清零事件
		adjust_page_auto_zero();
		
		adjustInformation.isAutoZeroButtonDown = false;
	}
	
	if(adjustInformation.isSaveArgButtonDown == true)
	{
		//存储事件
		if(adjust_page_save() == true)
		{
			adjustInformation.isSaveArgButtonDown = false;
		}
	}
	
	if(adjustInformation.timer++ > ADJUST_PUSH_CYCLE)
	{
		//刷新电流值
		adjust_page_push_cur();	
		
		adjustInformation.timer = 0;
		
	}
}
 

