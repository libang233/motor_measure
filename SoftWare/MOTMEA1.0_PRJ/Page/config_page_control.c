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
	
	write_control_state(ID_TRIGGER, configInformation.trigger);
	
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
	
	pull_control_data(ID_TRIGGER, FRAME_REC_DATA_STATUES_DEF);
	
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
* @ Function Name : config_page_disenable
* @ Author        : hlb
* @ Brief         : 配置界面 查阅状态下禁能控件
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_disenable(void)
{
	u8 i;

	
	//是否为配置状态
	if(configInformation.isConfig == false)
	{
		//禁能并消除显示存储和删除按键
		write_control_state(ID_CONFIG_SAVE_BUTTON, CONTROL_DISENABLE_DISAPPER);
	
		write_control_state(ID_DELETE_PN_BUTTON, CONTROL_DISENABLE_DISAPPER);	


		//禁能其他控件
		write_control_state(ID_POWER_SELECT, CONTROL_DISENABLE);

		write_control_state(ID_PN_NUMBER, CONTROL_DISENABLE);

		write_control_state(ID_PN_DESCRIP, CONTROL_DISENABLE);

		write_control_state(ID_POLES_NUMBER, CONTROL_DISENABLE);

		write_control_state(ID_PART_NUMBER, CONTROL_DISENABLE);

		write_control_state(ID_TRIGGER, CONTROL_DISENABLE);

		write_control_state(ID_TEST_CURRENT, CONTROL_DISENABLE);

		write_control_state(ID_CONFIG_PWM, CONTROL_DISENABLE);


		//遍历段 禁能
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

	//禁能或隐藏相应控件
	//config_page_disenable();
	
	//上传数据
	config_page_data_push();
	
	//隐藏相应段
	config_page_disappear_part();
	
}

/**
* @ Function Name : config_page_handle
* @ Author        : hlb
* @ Brief         : 配置界面函数
* @ Date          : 2017.07.25
* @ Modify        : ...
 **/
void config_page_handle(void)
{
	u8 i;

	if(configInformation.isDataUpdate == false)
	{
		config_page_update();

		configInformation.isDataUpdate = true;
	}

	if(configInformation.isPartUpdate == false)
	{
		config_page_disappear_part();
		
		configInformation.isPartUpdate = true;
	}
	
	if(configInformation.isSaveButtonDown == true)
	{	
		if(configInformation.isDataPull == false)
		{
			//发送下拉数据指令
			config_page_data_pull();
			
			configInformation.isDataPull = true;
		}
		
		//数据下拉接收完毕
		if(configInformation.isDataReced == true)
		{

			sdcard_PN_file_edit();

			configInformation.isDataReced = false;
		
			configInformation.isDataUpdate = false;
	
			configInformation.isSaveButtonDown = false;
		}
	}
	
	if(configInformation.isDeleteButtonDown == true)
	{
		sdcard_PN_file_delete();
		
		configInformation.isDataUpdate = false;
		
		configInformation.isDeleteButtonDown = false;
	}
}

