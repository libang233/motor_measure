/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : adjust_page_control.c
 * Author    : hlb
 * Brief     : У׼�������
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
* @ Brief         : �������ֵ���ַ���
* @ Date          : 2017.09.18
* @ Input         : u16 cur      	����
					u8  *val        �����ַ���ͷ
					u8  *valSize    �ַ�����С
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
* @ Brief         : �������ֵ
* @ Date          : 2017.09.18
* @ Input         : u16 *cur      �����洢��ַ
					u8  *val      �����ַ���ͷ
					u8  valSize   �ַ�����С
* @ Output        : bool			�Ƿ���ȷ����
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
* @ Brief         : ���͵���
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
	
	//ת���ַ���
	calculate_current_string(adjustInformation.measureCurrent, adjustInformation.measureCurrentString, &adjustInformation.measureCurrentStringIdx);
	
	write_control_state(ID_ADJUST_MEA_CURRNT, ENABLE);
	
	//�ϴ�����
	write_char(ID_ADJUST_MEA_CURRNT, adjustInformation.measureCurrentString, adjustInformation.measureCurrentStringIdx);
	
	write_control_state(ID_ADJUST_MEA_CURRNT, CONTROL_DISENABLE);
}
 

 /**
* @ Function Name : adjust_page_auto_zero
* @ Author        : hlb
* @ Brief         : �����¼�
* @ Date          : 2017.09.20
* @ Output        : bool			�Ƿ������
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
	
	//�޸�sd��������
	sdcard_index_file_current_edit();		
}

 /**
* @ Function Name : adjust_page_save
* @ Author        : hlb
* @ Brief         : �洢�¼�
* @ Date          : 2017.09.20
* @ Output        : bool			�Ƿ������
* @ Modify        : ...
 **/ 
bool adjust_page_save(void)
{
	if(adjustInformation.isDataPull == true)
	{
		//��������
		pull_control_data(ID_ADJUST_REA_CURRNT, FRAME_REC_DATA_STRING_DEF);
		
		adjustInformation.isDataPull = false;
	}
	if(adjustInformation.isDataReced == true)
	{
		//��������ĵ���
		if(calculate_curent(&adjustInformation.inputCurrent, adjustInformation.inputCurrentString, adjustInformation.inputCurrentStringIdx) == true)
		{
			//����ƫ��
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
		
			//�޸�sd��������
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
* @ Brief         : ���ý����߳�
* @ Date          : 2017.09.20
* @ Modify        : ...
 **/
void adjust_page_init(void)
{
	//���ܿؼ�
	write_control_state(ID_ADJUST_MEA_CURRNT, CONTROL_DISENABLE);
}	
	
 /**
* @ Function Name : adjust_page_handle
* @ Author        : hlb
* @ Brief         : У׼�����߳�
* @ Date          : 2017.09.18
* @ Modify        : ...
 **/
void adjust_page_handle(void)
{
	
	if(globalInformation.isPageInit == false)
	{
		//��ʼ������
		adjust_page_init();
		
		globalInformation.isPageInit = true;
	}
	
	if(adjustInformation.isAutoZeroButtonDown == true)
	{
		//�����¼�
		adjust_page_auto_zero();
		
		adjustInformation.isAutoZeroButtonDown = false;
	}
	
	if(adjustInformation.isSaveArgButtonDown == true)
	{
		//�洢�¼�
		if(adjust_page_save() == true)
		{
			adjustInformation.isSaveArgButtonDown = false;
		}
	}
	
	if(adjustInformation.timer++ > ADJUST_PUSH_CYCLE)
	{
		//ˢ�µ���ֵ
		adjust_page_push_cur();	
		
		adjustInformation.timer = 0;
		
	}
}
 

