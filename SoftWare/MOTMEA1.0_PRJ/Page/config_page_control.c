/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : sdcard_control.c
 * Author    : hlb
 * Brief     : ���ý������
 * Date      : 2017.07.22
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "config_page_control.h"
#include "sys_manager.h"
#include "message_analysis.h"
#include "message_send.h"

/**
* @ Function Name : pull_config_page_data
* @ Author        : hlb
* @ Brief         : 
* @ Date          : 2017.07.23
* @ Modify        : ...
**/
void pull_config_page_data(void)
{
	u8 i;
	
	pull_control_data(ID_PN_NUMBER_SELECT, FRAME_REC_DATA_SELECT_DEF);
	
	pull_control_data(ID_POWER_SELECT, FRAME_REC_DATA_SELECT_DEF);
	
	pull_control_data(ID_PN_NUMBER, FRAME_REC_DATA_STRING_DEF);
	
	pull_control_data(ID_PN_DESCRIP, FRAME_REC_DATA_STRING_DEF);
	
	pull_control_data(ID_POLES_NUMBER, FRAME_REC_DATA_INT_DEF);
	
	pull_control_data(ID_PART_NUMBER, FRAME_REC_DATA_INT_DEF);
	
	pull_control_data(ID_TRIGGER, FRAME_REC_DATA_STATUES_DEF);
	
	pull_control_data(ID_TEST_CURRENT, FRAME_REC_DATA_INT_DEF);
	
	pull_control_data(ID_CONFIG_PWM, FRAME_REC_DATA_INT_DEF);
	
	//??ID??? ??
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
}

/**
* @ Function Name : config_page_update
* @ Author        : hlb
* @ Brief         : ���ý������ݸ���
* @ Date          : 2017.07.24
* @ Modify        : ...
 **/
void config_page_update(void)
{
	int i;
	
	//�Ƿ�Ϊ����״̬
	if(configInformation.isConfig == false)
	{
		//���ܲ�������ʾ�洢��ɾ������
		write_control_state(ID_CONFIG_SAVE_BUTTON, CONTROL_DISENABLE_DISAPPER);
		write_control_state(ID_DELETE_PN_BUTTON, CONTROL_DISENABLE_DISAPPER);
	}
	
	//�ϴ��Ϻ��б�
	for(i = 0; i < configInformation.PNNumQuantity; i++)
	{
		write_select_val(ID_PN_NUMBER_SELECT, i, configInformation.PNNumStringIdx[i], configInformation.PNNum[i]);
	}
	
	//Ĭ�϶�ȡϵͳ�Ϻ�
	write_select_position(ID_PN_NUMBER_SELECT, configInformation.PNNumQuantity - 1);
}

/**
* @ Function Name : config_page_init
* @ Author        : hlb
* @ Brief         : ���ý����ʼ������
* @ Date          : 2017.07.22
* @ Modify        : ...
 **/
void config_page_init(void)
{
	int i;
	
	//�Ƿ�Ϊ����״̬
	if(configInformation.isConfig == false)
	{
		//���ܲ�������ʾ�洢��ɾ������
		write_control_state(ID_CONFIG_SAVE_BUTTON, CONTROL_DISENABLE_DISAPPER);
		write_control_state(ID_DELETE_PN_BUTTON, CONTROL_DISENABLE_DISAPPER);
	}
	
	//�ϴ��Ϻ��б�
	for(i = 0; i < configInformation.PNNumQuantity; i++)
	{
		write_select_val(ID_PN_NUMBER_SELECT, i, configInformation.PNNumStringIdx[i], configInformation.PNNum[i]);
	}
	
	//Ĭ�϶�ȡ���һ���Ϻ�
	write_select_position(ID_PN_NUMBER_SELECT, configInformation.PNNumQuantity - 1);
	
	//��ȡ��ǰ�ϺŶ���
	
}

/**
* @ Function Name : config_page_handle
* @ Author        : hlb
* @ Brief         : ���ý��溯��
* @ Date          : 2017.07.22
* @ Modify        : ...
 **/
void config_page_handle(void)
{
	config_page_init();
}

