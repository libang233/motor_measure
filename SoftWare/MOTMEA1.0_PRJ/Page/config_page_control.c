/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : config_page_control.c
 * Author    : hlb
 * Brief     : 配置界面程序
 * Date      : 2017.07.24
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "config_page_control.h"
#include "sys_manager.h"
#include "message_analysis.h"
#include "message_send.h"
#include "sdcard_control.h"
#include "diskio.h"
#include "usart.h"

/**
* @ Function Name : config_page_data_push
* @ Author        : hlb
* @ Brief         : 上传配置界面数据
* @ Date          : 2017.07.25
* @ Modify        : ...
**/
void config_page_data_push(void)
{
	u8 i;
	
	
	//上传料号列表
	for(i = 0; i < configInformation.PNNumQuantity; i++)
	{

		write_select_val(ID_PN_NUMBER_SELECT, i, configInformation.PNNumStringIdx[i], configInformation.PNNum[i]);
		
	}
	
	write_select_position(ID_PN_NUMBER_SELECT, configInformation.PNNumSelect);
	
	write_select_position(ID_POWER_SELECT, configInformation.powerSelect);
	
	write_char(ID_PN_NUMBER, configInformation.PNNum[configInformation.PNNumSelect], configInformation.PNNumStringIdx[configInformation.PNNumSelect]);
	
	write_char(ID_PN_DESCRIP, configInformation.PNNumDescription, configInformation.descriptionStringIdx);
	
	write_int(ID_POLES_NUMBER, configInformation.polesNum);
	
	write_int(ID_PART_NUMBER, configInformation.partNum);
	
	write_bitmap(ID_TRIGGER, configInformation.trigger);
	
	write_int(ID_TEST_CURRENT, configInformation.testCurrent);
	
	write_int(ID_CONFIG_PWM, configInformation.PWMSignFrequen);

	//通过ID号偏移进行遍历
	for(i = 0; i < configInformation.partNum; i++)
	{
		write_char((ID_PART1_EXPLAIN + i),configInformation.partInformation[i].explain, configInformation.partInformation[i].explainStringIdx);
	
		write_int((ID_PART1_PWM + i * 6), configInformation.partInformation[i].testPWM);
		
		write_int((ID_PART1_TIME + i * 6), configInformation.partInformation[i].testTime);
		
		write_int((ID_PART1_UP_SPEED + i * 6), configInformation.partInformation[i].testSpeedUpLimit);
		
		write_int((ID_PART1_DOWN_SPEED + i * 6), configInformation.partInformation[i].testSpeedDownLimit);
		
		write_int((ID_PART1_UP_CURRENT + i * 6), configInformation.partInformation[i].testCurrentUpLimit);
		
		write_int((ID_PART1_DOWN_CURRENT + i * 6), configInformation.partInformation[i].testCurrentDownLimit);
			
	}
	
	
}

/**
* @ Function Name : config_page_data_pull
* @ Author        : hlb
* @ Brief         : 拉下配置界面数据
* @ Date          : 2017.07.25
* @ Modify        : ...
**/
void config_page_data_pull(void)
{
	u8 i;
	
	pull_control_data(ID_PN_NUMBER_SELECT, FRAME_REC_DATA_SELECT_DEF);
	
	pull_control_data(ID_PN_NUMBER, FRAME_REC_DATA_STRING_DEF);
	
	pull_control_data(ID_PN_DESCRIP, FRAME_REC_DATA_STRING_DEF);
	
	pull_control_data(ID_POLES_NUMBER, FRAME_REC_DATA_INT_DEF);
	
	pull_control_data(ID_PART_NUMBER, FRAME_REC_DATA_INT_DEF);
	
	pull_control_data(ID_TRIGGER, FRAME_REC_DATA_BITMAP_DEF);
	
	pull_control_data(ID_TEST_CURRENT, FRAME_REC_DATA_INT_DEF);
	
	pull_control_data(ID_CONFIG_PWM, FRAME_REC_DATA_INT_DEF);
	
	//遍历段
	for(i = 0; i < configInformation.partNum; i++)
	{
		pull_control_data(ID_PART1_EXPLAIN + i, FRAME_REC_DATA_STRING_DEF);
		
		pull_control_data(ID_PART1_PWM + 6 * i, FRAME_REC_DATA_INT_DEF);
		
		pull_control_data(ID_PART1_TIME + 6 * i, FRAME_REC_DATA_INT_DEF);
		
		pull_control_data(ID_PART1_UP_SPEED + 6 * i, FRAME_REC_DATA_INT_DEF);
		
		pull_control_data(ID_PART1_DOWN_SPEED + 6 * i, FRAME_REC_DATA_INT_DEF);
		
		pull_control_data(ID_PART1_UP_CURRENT + 6 * i, FRAME_REC_DATA_INT_DEF);
		
		pull_control_data(ID_PART1_DOWN_CURRENT + 6 * i, FRAME_REC_DATA_INT_DEF);
	}
	
	//将电源数据放置最后 做为结束标志 
	//当接收到电源信息时 则认为数据接收完毕
	pull_control_data(ID_POWER_SELECT, FRAME_REC_DATA_SELECT_DEF);
}

/**
* @ Function Name : config_page_enable
* @ Author        : hlb
* @ Brief         : 配置界面 使能控件
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_enable(void)
{
	u8 i;

	//使能其他控件
	write_control_state(ID_POWER_SELECT, CONTROL_ENABLE);

	write_control_state(ID_PN_NUMBER, CONTROL_ENABLE);

	write_control_state(ID_PN_DESCRIP, CONTROL_ENABLE);

	write_control_state(ID_POLES_NUMBER, CONTROL_ENABLE);

	write_control_state(ID_PART_NUMBER, CONTROL_ENABLE);

	write_control_state(ID_TRIGGER, CONTROL_ENABLE);

	write_control_state(ID_TEST_CURRENT, CONTROL_ENABLE);

	write_control_state(ID_CONFIG_PWM, CONTROL_ENABLE);
	
		//使能有效段
	for(i = 0; i < configInformation.partNum; i++)
	{
		write_control_state(ID_PART1_EXPLAIN + i, CONTROL_ENABLE);
		
		write_control_state(ID_PART1_PWM + 6 * i, CONTROL_ENABLE);
		
		write_control_state(ID_PART1_TIME + 6 * i, CONTROL_ENABLE);
		
		write_control_state(ID_PART1_UP_SPEED + 6 * i, CONTROL_ENABLE);
		
		write_control_state(ID_PART1_DOWN_SPEED + 6 * i, CONTROL_ENABLE);
		
		write_control_state(ID_PART1_UP_CURRENT + 6 * i, CONTROL_ENABLE);
		
		write_control_state(ID_PART1_DOWN_CURRENT + 6 * i, CONTROL_ENABLE);
		
	}	

	
}

/**
* @ Function Name : config_page_disenable
* @ Author        : hlb
* @ Brief         : 配置界面 查阅状态下禁能控件
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_disenable(void)
{

	if(configInformation.isConfig == false)
	{
		//禁能其他控件
		write_control_state(ID_POWER_SELECT, CONTROL_DISENABLE);

		write_control_state(ID_PN_NUMBER, CONTROL_DISENABLE);

		write_control_state(ID_PN_DESCRIP, CONTROL_DISENABLE);

		write_control_state(ID_POLES_NUMBER, CONTROL_DISENABLE);

		write_control_state(ID_PART_NUMBER, CONTROL_DISENABLE);

		write_control_state(ID_TRIGGER, CONTROL_DISENABLE);

		write_control_state(ID_TEST_CURRENT, CONTROL_DISENABLE);

		write_control_state(ID_CONFIG_PWM, CONTROL_DISENABLE);
		
	}
}
/**
* @ Function Name : config_page_disappear_part
* @ Author        : hlb
* @ Brief         : 配置界面 隐藏相应段
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_disappear_part(void)
{
	u8 i;
	
	
	if(configInformation.partNum <= MAX_PART_NUM)
	{
		//如果为查阅状态
		if(configInformation.isConfig == false)
		{
			//禁能有效段
			for(i = 0; i < configInformation.partNum; i++)
			{
				write_control_state(ID_PART1_EXPLAIN + i, CONTROL_DISENABLE);
				
				write_control_state(ID_PART1_PWM + 6 * i, CONTROL_DISENABLE);
				
				write_control_state(ID_PART1_TIME + 6 * i, CONTROL_DISENABLE);
				
				write_control_state(ID_PART1_UP_SPEED + 6 * i, CONTROL_DISENABLE);
				
				write_control_state(ID_PART1_DOWN_SPEED + 6 * i, CONTROL_DISENABLE);
				
				write_control_state(ID_PART1_UP_CURRENT + 6 * i, CONTROL_DISENABLE);
				
				write_control_state(ID_PART1_DOWN_CURRENT + 6 * i, CONTROL_DISENABLE);
				
			}			
		}
		else
		{
			//使能有效段
			for(i = 0; i < configInformation.partNum; i++)
			{
				write_control_state(ID_PART1_EXPLAIN + i, CONTROL_ENABLE);
				
				write_control_state(ID_PART1_PWM + 6 * i, CONTROL_ENABLE);
				
				write_control_state(ID_PART1_TIME + 6 * i, CONTROL_ENABLE);
				
				write_control_state(ID_PART1_UP_SPEED + 6 * i, CONTROL_ENABLE);
				
				write_control_state(ID_PART1_DOWN_SPEED + 6 * i, CONTROL_ENABLE);
				
				write_control_state(ID_PART1_UP_CURRENT + 6 * i, CONTROL_ENABLE);
				
				write_control_state(ID_PART1_DOWN_CURRENT + 6 * i, CONTROL_ENABLE);
				
			}	
		}

		//隐藏多余段
		for(i = configInformation.partNum; i < MAX_PART_NUM; i++)
		{
			write_control_state(ID_PART1_EXPLAIN + i, CONTROL_DISENABLE_DISAPPER);
			
			write_control_state(ID_PART1_PWM + 6 * i, CONTROL_DISENABLE_DISAPPER);
			
			write_control_state(ID_PART1_TIME + 6 * i, CONTROL_DISENABLE_DISAPPER);
			
			write_control_state(ID_PART1_UP_SPEED + 6 * i, CONTROL_DISENABLE_DISAPPER);
			
			write_control_state(ID_PART1_DOWN_SPEED + 6 * i, CONTROL_DISENABLE_DISAPPER);
			
			write_control_state(ID_PART1_UP_CURRENT + 6 * i, CONTROL_DISENABLE_DISAPPER);
			
			write_control_state(ID_PART1_DOWN_CURRENT + 6 * i, CONTROL_DISENABLE_DISAPPER);
		}
	}
}


/**
* @ Function Name : config_page_update
* @ Author        : hlb
* @ Brief         : 配置界面更新
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_update(void)
{

	//从sd卡下载数据
	sdcard_PN_file_load();
	
	//使能相应控件
	config_page_enable();
	
	//上传数据
	config_page_data_push();
	
	//禁能相应控件
	config_page_disenable();
}

 /**
* @ Function Name : config_page_PN_save
* @ Author        : hlb
* @ Brief         : 配置界面料号存储事件
* @ Date          : 2017.07.29
* @ Output        : bool			是否处理完毕
* @ Modify        : ...
 **/ 
bool config_page_PN_save(void)
{
	if(configInformation.isDataPull == true)
	{
		//发送下拉数据指令
		config_page_data_pull();
		
		configInformation.isDataPull = false;
	}
	//数据下拉接收完毕
	if(configInformation.isDataReced == true)
	{
		//修改PN文件
		sdcard_PN_file_edit();

		configInformation.isDataReced = false;
	
		configInformation.isDataUpdate = false;

		return true;
	}
	
	return false;
}

/**
* @ Function Name : config_page_init
* @ Author        : hlb
* @ Brief         : 配置界面初始化
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/
void config_page_init(void)
{
	
	//是否为配置状态
	if(configInformation.isConfig == false)
	{
		//禁能并消除显示存储和删除按键
		write_control_state(ID_CONFIG_SAVE_BUTTON, CONTROL_DISENABLE_DISAPPER);

		write_control_state(ID_DELETE_PN_BUTTON, CONTROL_DISENABLE_DISAPPER);	
		
		if(configInformation.PNNumQuantity == 1)
		{
			//只有模板料号存在
			configInformation.PNNumSelect = MODLE_PN_POSITION;
		}
		else
		{
			//选取最后一个料号
			configInformation.PNNumSelect = configInformation.PNNumQuantity - 1;
		}
	}
	else
	{
				
		//显示存储和删除按键
		write_control_state(ID_CONFIG_SAVE_BUTTON, CONTROL_ENABLE);

		write_control_state(ID_DELETE_PN_BUTTON, CONTROL_ENABLE);	
		
		if(configInformation.PNNumQuantity >= MAX_PN_NUM_MAX_QUANTITY)
		{
			//料号已满 读取第二个料号
			configInformation.PNNumSelect = MODLE_PN_POSITION + 1;
		}
		else
		{
			//料号未满 读取模板料号
			configInformation.PNNumSelect = MODLE_PN_POSITION;
		}
	}
}

/**
* @ Function Name : config_page_handle
* @ Author        : hlb
* @ Brief         : 配置界面线程
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_handle(void)
{
	
	if(globalInformation.isPageInit == false)
	{
		//初始化界面
		config_page_init();
		
		configInformation.isDataUpdate = false;
		globalInformation.isPageInit = true;
	}
	
	if(configInformation.isDataUpdate == false)
	{
		//更新界面数据
		config_page_update();

		configInformation.isPartUpdate = false;
		configInformation.isDataUpdate = true;
	}

	if(configInformation.isPartUpdate == false)
	{
		//更新段的状态
		config_page_disappear_part();
		
		configInformation.isPartUpdate = true;
	}
	
	if(configInformation.isSaveButtonDown == true)
	{	
		//进入料号存储事件
		if(config_page_PN_save() == true)
		{
			configInformation.isSaveButtonDown = false;
		}
	}
	
	if(configInformation.isDeleteButtonDown == true)
	{
		//删除存储料号
		sdcard_PN_file_delete();
		
		configInformation.isDataUpdate = false;
		
		configInformation.isDeleteButtonDown = false;
	}
}

