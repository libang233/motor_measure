/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : password_page_control.c
 * Author    : hlb
 * Brief     : ����������
 * Date      : 2017.07.28
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "password_page_control.h"
#include "sys_manager.h"
#include "message_analysis.h"
#include "message_send.h"
#include "sdcard_control.h"



 
 /**
* @ Function Name : password_page_check
* @ Author        : hlb
* @ Brief         : �������У������
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/  
bool password_page_check(u8 size, u8 *inputPassword)
{
	u8 i;
	
	if(size == passwordInformation.realPasswordStringIdx)
	{
		for(i = 0; i < size; i++)
		{
			if(inputPassword[i] != passwordInformation.realPassword[i])
			{
				return false;
			}
		}
	}
	else 
	{
		return false;
	}
	
	return true;
}
 
 
 /**
* @ Function Name : password_page_enable
* @ Author        : hlb
* @ Brief         : �������ʹ�ܿؼ�
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/  
void password_page_enable(void)
{
	write_control_state(ID_OLD_PASSWORD, CONTROL_ENABLE);
	write_control_state(ID_NEW_PASSWORD, CONTROL_ENABLE);
	write_control_state(ID_OLD_PASSWORD_TIP, CONTROL_ENABLE);
	write_control_state(ID_NEW_PASSWORD_TIP, CONTROL_ENABLE);
	write_control_state(ID_PASSWORD_CHANGE_OK_BUTTON, CONTROL_ENABLE);	
}

 /**
* @ Function Name : password_page_disappear
* @ Author        : hlb
* @ Brief         : ����������ؿؼ�
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/  
void password_page_disappear(void)
{
	write_control_state(ID_OLD_PASSWORD, CONTROL_DISENABLE_DISAPPER);
	write_control_state(ID_NEW_PASSWORD, CONTROL_DISENABLE_DISAPPER);
	write_control_state(ID_OLD_PASSWORD_TIP, CONTROL_DISENABLE_DISAPPER);
	write_control_state(ID_NEW_PASSWORD_TIP, CONTROL_DISENABLE_DISAPPER);
	write_control_state(ID_PASSWORD_CHANGE_OK_BUTTON, CONTROL_DISENABLE_DISAPPER);
}

 /**
* @ Function Name : password_page_data_pull
* @ Author        : hlb
* @ Brief         : ���������������
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/ 
void password_page_data_pull(void)
{
	pull_control_data(ID_INPUT_PASSWORD, FRAME_REC_DATA_STRING_DEF);
	pull_control_data(ID_NEW_PASSWORD_TIP, FRAME_REC_DATA_STRING_DEF);
	pull_control_data(ID_OLD_PASSWORD, FRAME_REC_DATA_STRING_DEF);
	pull_control_data(ID_NEW_PASSWORD, FRAME_REC_DATA_STRING_DEF);
}	

 /**
* @ Function Name : password_page_change_init
* @ Author        : hlb
* @ Brief         : ������������޸Ŀ��ʼ��
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/
void password_page_change_init(void)
{
	//ʹ�������޸Ŀ�
	password_page_enable();
	
	passwordInformation.newPasswordStringIdx = MAX_BYTE_NUM_PASSWORD;
	
	passwordInformation.oldPasswordStringIdx = MAX_BYTE_NUM_PASSWORD;
	
	passwordInformation.oldPasswordTipStringIdx =  MAX_BYTE_NUM_PASSWORD;
	
	memset(&passwordInformation.newPasswordTip, 0, sizeof(passwordInformation.newPasswordTip));
	
	memset(&passwordInformation.oldPassword, 0, sizeof(passwordInformation.oldPassword));
	
	memset(&passwordInformation.newPassword, 0, sizeof(passwordInformation.newPassword));
	
	write_char(ID_OLD_PASSWORD_TIP, passwordInformation.oldPasswordTip, passwordInformation.oldPasswordTipStringIdx);
	
	write_char(ID_NEW_PASSWORD_TIP, passwordInformation.newPasswordTip, passwordInformation.newPasswordTipStringIdx);
	
	write_char(ID_OLD_PASSWORD, passwordInformation.oldPassword, passwordInformation.oldPasswordStringIdx);
	
	write_char(ID_NEW_PASSWORD, passwordInformation.newPassword, passwordInformation.newPasswordStringIdx);

}

 /**
* @ Function Name : password_page_password_OK
* @ Author        : hlb
* @ Brief         : �����������ȷ���¼�����
* @ Date          : 2017.07.29
* @ Output        : bool			�Ƿ������
* @ Modify        : ...
 **/ 
bool password_page_password_OK(void)
{
	if(passwordInformation.isDataPullInput == true)
	{
		//������������
		pull_control_data(ID_INPUT_PASSWORD, FRAME_REC_DATA_STRING_DEF);
		
		passwordInformation.isDataPullInput = false;
	}
	if(passwordInformation.isGetOldPassword == true)
	{
		//У������
		if(password_page_check(passwordInformation.inputPasswordStringIdx,passwordInformation.inputPassword) == true)
		{
			//�������ý���
			write_page_num(PAGE_NUM_CONFIG);
			
			//�������ñ�־
			configInformation.isConfig = true;
			
			//���������ʼ����־
			globalInformation.isPageInit = true;
			
			//�ı�ȫ�ֽ���
			globalInformation.nowPage = PAGE_NUM_CONFIG;
		}
		else
		{
			//��ʾ����������
			write_page_num(PAGE_NUM_PASSWORD_WRONG);
		}
		
		passwordInformation.isGetOldPassword = false;		
		
		return true;
	}

	return false;
}



 /**
* @ Function Name : password_page_password_change
* @ Author        : hlb
* @ Brief         : ��������޸�����
* @ Date          : 2017.07.29
* @ Output        : bool			�Ƿ������
* @ Modify        : ...
 **/ 
bool password_page_password_change(void)
{
	if(passwordInformation.isDataPullNew == true)
	{
		//���������޸Ŀ�����
		pull_control_data(ID_NEW_PASSWORD_TIP, FRAME_REC_DATA_STRING_DEF);
		
		pull_control_data(ID_OLD_PASSWORD, FRAME_REC_DATA_STRING_DEF);
		
		pull_control_data(ID_NEW_PASSWORD, FRAME_REC_DATA_STRING_DEF);
		
		passwordInformation.isDataPullNew = false;
	}
	//�õ�����
	if(passwordInformation.isGetNewPassword == true)
	{
		//У������
		if(password_page_check(passwordInformation.oldPasswordStringIdx, passwordInformation.oldPassword) == true)
		{
			//�޸�����
			sdcard_index_file_password_edit();
			
			//���������޸Ŀ� �����޸ĳɹ�
			password_page_disappear();
		}
		else
		{
			//��ʾ�������ҳ��
			write_page_num(PAGE_NUM_PASSWORD_WRONG);
		}
		
		
		passwordInformation.isGetNewPassword = false;

		return true;
	}		
	
	return false;
}


 /**
* @ Function Name : password_page_init
* @ Author        : hlb
* @ Brief         : ��������ʼ��
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/ 
void password_page_init(void)
{
	password_page_disappear();
}

 /**
* @ Function Name : password_page_handle
* @ Author        : hlb
* @ Brief         : ��������߳�
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/
void password_page_handle(void)
{
	if(globalInformation.isPageInit == false)
	{
		//��ʼ������
		password_page_init();
		
		globalInformation.isPageInit = true;
	}
	
	if(passwordInformation.isChangeButtonDown == true)
	{
		//��ʼ�������޸Ŀ�
		password_page_change_init();
		
		passwordInformation.isChangeButtonDown = false;
	}
	
	if(passwordInformation.isChangeOkButtonDown == true)
	{
		//���������޸��¼�
		if(password_page_password_change() == true)
		{
			passwordInformation.isChangeOkButtonDown = false;
		}		
	}
	
	if(passwordInformation.isOkButtonDown == true)
	{
		//��������ȷ���¼�
		if(password_page_password_OK() == true)
		{
			passwordInformation.isOkButtonDown = false;
		}
	}
}
 

