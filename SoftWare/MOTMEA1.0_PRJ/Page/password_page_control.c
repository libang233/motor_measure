/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : password_page_control.c
 * Author    : hlb
 * Brief     : 密码界面程序
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
* @ Brief         : 密码界面校核密码
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
* @ Brief         : 密码界面使能控件
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
* @ Brief         : 密码界面隐藏控件
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
* @ Brief         : 密码界面数据下拉
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
* @ Brief         : 密码界面密码修改框初始化
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/
void password_page_change_init(void)
{
	//使能密码修改框
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
* @ Brief         : 密码界面密码确认事件函数
* @ Date          : 2017.07.29
* @ Output        : bool			是否处理完毕
* @ Modify        : ...
 **/ 
bool password_page_password_OK(void)
{
	if(passwordInformation.isDataPullInput == true)
	{
		//下拉输入密码
		pull_control_data(ID_INPUT_PASSWORD, FRAME_REC_DATA_STRING_DEF);
		
		passwordInformation.isDataPullInput = false;
	}
	if(passwordInformation.isGetOldPassword == true)
	{
		//校核密码
		if(password_page_check(passwordInformation.inputPasswordStringIdx,passwordInformation.inputPassword) == true)
		{
			//进入配置界面
			write_page_num(PAGE_NUM_CONFIG);
			
			//开启配置标志
			configInformation.isConfig = true;
			
			//开启界面初始化标志
			globalInformation.isPageInit = true;
			
			//改变全局界面
			globalInformation.nowPage = PAGE_NUM_CONFIG;
		}
		else
		{
			//显示密码错误界面
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
* @ Brief         : 密码界面修改密码
* @ Date          : 2017.07.29
* @ Output        : bool			是否处理完毕
* @ Modify        : ...
 **/ 
bool password_page_password_change(void)
{
	if(passwordInformation.isDataPullNew == true)
	{
		//下拉密码修改框数据
		pull_control_data(ID_NEW_PASSWORD_TIP, FRAME_REC_DATA_STRING_DEF);
		
		pull_control_data(ID_OLD_PASSWORD, FRAME_REC_DATA_STRING_DEF);
		
		pull_control_data(ID_NEW_PASSWORD, FRAME_REC_DATA_STRING_DEF);
		
		passwordInformation.isDataPullNew = false;
	}
	//得到数据
	if(passwordInformation.isGetNewPassword == true)
	{
		//校核密码
		if(password_page_check(passwordInformation.oldPasswordStringIdx, passwordInformation.oldPassword) == true)
		{
			//修改密码
			sdcard_index_file_password_edit();
			
			//隐藏密码修改框 密码修改成功
			password_page_disappear();
		}
		else
		{
			//显示密码错误页面
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
* @ Brief         : 密码界面初始化
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
* @ Brief         : 密码界面线程
* @ Date          : 2017.07.28
* @ Modify        : ...
 **/
void password_page_handle(void)
{
	if(globalInformation.isPageInit == false)
	{
		//初始化界面
		password_page_init();
		
		globalInformation.isPageInit = true;
	}
	
	if(passwordInformation.isChangeButtonDown == true)
	{
		//初始化密码修改框
		password_page_change_init();
		
		passwordInformation.isChangeButtonDown = false;
	}
	
	if(passwordInformation.isChangeOkButtonDown == true)
	{
		//进入密码修改事件
		if(password_page_password_change() == true)
		{
			passwordInformation.isChangeOkButtonDown = false;
		}		
	}
	
	if(passwordInformation.isOkButtonDown == true)
	{
		//进入密码确定事件
		if(password_page_password_OK() == true)
		{
			passwordInformation.isOkButtonDown = false;
		}
	}
}
 

