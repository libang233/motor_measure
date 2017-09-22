/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : message_analysis.c
 * Author    : hlb
 * Brief     : 数据分析匹配
 * Date      : 2017.05.31
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "sys_manager.h"
#include "usart.h"
#include "newtype.h"
#include "message_analysis.h"
#include "message_send.h"


//解析器实例化
Usart_Parser_Moniter_TypeDef ParserMoniter;

/**
* @ Function Name : parser_init
* @ Author        : hlb
* @ Brief         : 解析器初始化
* @ Date          : 2017.06.5
* @ Modify        : ...
 **/
void parser_init(void)
{
	u16 i;

	//清除缓冲
	for(i = 0; i < FRAME_BUFFER_NUM_MAX; i++)
	{
		memset(&ParserMoniter.Frame_Buffers[i], 0, sizeof(ParserMoniter.Frame_Buffers[i]));
	}
	
    // 初始化缓冲索引
    ParserMoniter.AddIdx = 0;
    ParserMoniter.GetIdx = 0;

    // 缓冲指示标志
    ParserMoniter.IsFrameBuffersEmpty = true;
    ParserMoniter.IsFrmaeBuffersFull = false;
	
	
}

/**
* @ Function Name : get_Usart_buffer_length
* @ Author        : hlb
* @ Brief         : 计算缓冲数据长度
* @ Date          : 2017.06.6
* @ Output        : u16					串口缓冲区长度
* @ Modify        : ...
**/
u16 get_usart_buffers_length(void)
{
	int length;
	
	length = UsartRxBuffer.AddIdx - UsartRxBuffer.GetIdx;
	
	//当写指针在读指针之前
	if(length < 0)
	{
		length = USART_RX_LEN - UsartRxBuffer.GetIdx + UsartRxBuffer.AddIdx;		
	}
	return length;
}

/**
* @ Function Name : parser_moniter_add_buff
* @ Author        : hlb
* @ Brief         : 解析器缓冲区数据添加
* @ Date          : 2017.06.6
* @ Modify        : ...
**/
void parser_moniter_add_buff(void)
{
	u8 Res;
	
	if(get_usart_buffers_length() > 0)
	{
		if(UsartRxBuffer.GetIdx >= USART_RX_LEN )
		{
			UsartRxBuffer.GetIdx = 0;
		}
		Res = UsartRxBuffer.Buff[UsartRxBuffer.GetIdx++];
		if(ParserMoniter.IsBufferAdd == false)
		{
			if(Res == FRAME_HEAD_DEF)																	
			{
				//接收到帧头，开始接收数据
				ParserMoniter.IsBufferAdd = true;
			}
		}
		
		if(ParserMoniter.IsBufferAdd == true)
		{
			if(ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx < USART_BUFFER_MAX_VAL)
			{
				//缓冲项填充数据
				ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Buff[ \
					ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx++] = Res;							
			}
			else
			{
				//超出最大范围，丢弃数据
				ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx = 0;
				ParserMoniter.IsBufferAdd = false;						
			}
			
			//缓冲项索引长度大于最小帧长度
			if(ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx > 3)
			{
				//通过数据长度字节校验字符串是否接收正确
				if(ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Buff[FRAME_DATA_NUM_ADDRESS] + 4 \
						<= ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx)
				{
					//缓冲项字符串结尾是否为数据帧结尾字符
					if(ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Buff[ \
						ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Buff[FRAME_DATA_NUM_ADDRESS] + 3] == FRAME_END_DEF)
					{
						//添加缓冲项
						if(ParserMoniter.AddIdx < FRAME_BUFFER_NUM_MAX - 1)								
						{
							ParserMoniter.AddIdx++;
						}
						else
						{
							ParserMoniter.AddIdx = 0;
						}
						
						//恢复缓冲区接收标志
						ParserMoniter.IsBufferAdd = false;
						//恢复缓冲项索引
						ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx = 0;
					}
					else
					{
						//丢弃数据
						ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx = 0;
						ParserMoniter.IsBufferAdd = false;		
					}
				}				
			}

		}
	}
}

/**
* @ Function Name : get_parser_buffers_length
* @ Author        : hlb
* @ Brief         : 计算解析器缓冲项长度
* @ Date          : 2017.06.7
* @ Output        : u8							器缓冲项长度
* @ Modify        : ...
**/
u8 get_parser_buffers_length(void)
{
	int length;
	
	length = ParserMoniter.AddIdx - ParserMoniter.GetIdx;
	
	//当写指针在读指针之前
	if(length < 0)
	{
		length = FRAME_BUFFER_NUM_MAX - ParserMoniter.GetIdx + ParserMoniter.AddIdx;		
	}
	return length;
}

/**
* @ Function Name : parser_rec_int_data_deal
* @ Author        : hlb
* @ Brief         : 读数值控件返回数据包处理函数
* @ Date          : 2017.06.17
* @ Input         : u16							数据ID号
* @ Output        : bool						是否接收成功
* @ Modify        : ...
**/
bool parser_rec_int_data_deal(u16 IDNum)
{
	u32 data;
	
	//读取数值
	data = (ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIGH] << 24) + \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIGH + 1] << 16) + \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIGH + 2] << 8) + \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIGH + 3]);

	switch(IDNum)
	{
		case ID_PART1_PWM:
			configInformation.partInformation[TEST_PART1].testPWM = data;
			break;
		
		case ID_PART1_TIME:
			configInformation.partInformation[TEST_PART1].testTime = data;
			break;
		
		case ID_PART1_UP_SPEED:
			configInformation.partInformation[TEST_PART1].testSpeedUpLimit = data;
			break;
		
		case ID_PART1_DOWN_SPEED:
			configInformation.partInformation[TEST_PART1].testSpeedDownLimit = data;
			break;
		
		case ID_PART1_UP_CURRENT:
			configInformation.partInformation[TEST_PART1].testCurrentUpLimit = data;
			break;
		
		case ID_PART1_DOWN_CURRENT:
			configInformation.partInformation[TEST_PART1].testCurrentDownLimit = data;
			break;

		case ID_PART2_PWM:
			configInformation.partInformation[TEST_PART2].testPWM = data;
			break;
		
		case ID_PART2_TIME:
			configInformation.partInformation[TEST_PART2].testTime = data;
			break;
		
		case ID_PART2_UP_SPEED:
			configInformation.partInformation[TEST_PART2].testSpeedUpLimit = data;
			break;
		
		case ID_PART2_DOWN_SPEED:
			configInformation.partInformation[TEST_PART2].testSpeedDownLimit = data;
			break;
		
		case ID_PART2_UP_CURRENT:
			configInformation.partInformation[TEST_PART2].testCurrentUpLimit = data;
			break;
		
		case ID_PART2_DOWN_CURRENT:
			configInformation.partInformation[TEST_PART2].testCurrentDownLimit = data;
			break;
		
		case ID_PART3_PWM:
			configInformation.partInformation[TEST_PART3].testPWM = data;
			break;
		
		case ID_PART3_TIME:
			configInformation.partInformation[TEST_PART3].testTime = data;
			break;
		
		case ID_PART3_UP_SPEED:
			configInformation.partInformation[TEST_PART3].testSpeedUpLimit = data;
			break;
		
		case ID_PART3_DOWN_SPEED:
			configInformation.partInformation[TEST_PART3].testSpeedDownLimit = data;
			break;
		
		case ID_PART3_UP_CURRENT:
			configInformation.partInformation[TEST_PART3].testCurrentUpLimit = data;
			break;
		
		case ID_PART3_DOWN_CURRENT:
			configInformation.partInformation[TEST_PART3].testCurrentDownLimit = data;
			break;
		
		case ID_PART4_PWM:
			configInformation.partInformation[TEST_PART4].testPWM = data;
			break;
		
		case ID_PART4_TIME:
			configInformation.partInformation[TEST_PART4].testTime = data;
			break;
		
		case ID_PART4_UP_SPEED:
			configInformation.partInformation[TEST_PART4].testSpeedUpLimit = data;
			break;
		
		case ID_PART4_DOWN_SPEED:
			configInformation.partInformation[TEST_PART4].testSpeedDownLimit = data;
			break;
		
		case ID_PART4_UP_CURRENT:
			configInformation.partInformation[TEST_PART4].testCurrentUpLimit = data;
			break;
		
		case ID_PART4_DOWN_CURRENT:
			configInformation.partInformation[TEST_PART4].testCurrentDownLimit = data;
			break;
		
		case ID_PART5_PWM:
			configInformation.partInformation[TEST_PART5].testPWM = data;
			break;
		
		case ID_PART5_TIME:
			configInformation.partInformation[TEST_PART5].testTime = data;
			break;
		
		case ID_PART5_UP_SPEED:
			configInformation.partInformation[TEST_PART5].testSpeedUpLimit = data;
			break;
		
		case ID_PART5_DOWN_SPEED:
			configInformation.partInformation[TEST_PART5].testSpeedDownLimit = data;
			break;
		
		case ID_PART5_UP_CURRENT:
			configInformation.partInformation[TEST_PART5].testCurrentUpLimit = data;
			break;
		
		case ID_PART5_DOWN_CURRENT:
			configInformation.partInformation[TEST_PART5].testCurrentDownLimit = data;
			break;
		
		case ID_PART6_PWM:
			configInformation.partInformation[TEST_PART6].testPWM = data;
			break;
		
		case ID_PART6_TIME:
			configInformation.partInformation[TEST_PART6].testTime = data;
			break;
		
		case ID_PART6_UP_SPEED:
			configInformation.partInformation[TEST_PART6].testSpeedUpLimit = data;
			break;
		
		case ID_PART6_DOWN_SPEED:
			configInformation.partInformation[TEST_PART6].testSpeedDownLimit = data;
			break;
		
		case ID_PART6_UP_CURRENT:
			configInformation.partInformation[TEST_PART6].testCurrentUpLimit = data;
			break;
		
		case ID_PART6_DOWN_CURRENT:
			configInformation.partInformation[TEST_PART6].testCurrentDownLimit = data;
			break;
		
		case ID_POLES_NUMBER:
			configInformation.polesNum = data;
			break;
		
		case ID_PART_NUMBER:
			configInformation.partNum = data;
			if(configInformation.partNum > MAX_PART_NUM)
			{
				configInformation.partNum = MAX_PART_NUM;
			}
			configInformation.isPartUpdate = false;
			break;
		
		case ID_TEST_CURRENT:
			configInformation.testCurrent = data;
			break;
		
		case ID_CONFIG_PWM:
			configInformation.PWMSignFrequen = data;
			break;
		
		case ID_DEBUG_INPUT_POLES:
			debugInformation.inputPoles = data;
			break;
		
		case ID_DEBUG_INPUT_PWM_FREQ:
			debugInformation.inputPWMFrequen = data;
			break;
		
		case ID_DEBUG_INPUT_PWM_DUTY:
			debugInformation.inputPWM = data;
			break;
		
		default:
			return false;
		
	}
	return true;
	
}

/**
* @ Function Name : parser_rec_string_data_deal
* @ Author        : hlb
* @ Brief         : 读字符串控件返回数据包处理函数
* @ Date          : 2017.07.09
* @ Input         : u16							数据ID号
* @ Output        : bool						是否接收成功
* @ Modify        : ...
**/
bool parser_rec_string_data_deal(u16 IDNum)
{
	u8 i;
	u8 byteNum;
	
	//读取字符串数据字节数
	byteNum = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_BYTE_NUM_ADDRESS];
	
	switch(IDNum)
	{
		case ID_PART1_EXPLAIN:
			configInformation.partInformation[TEST_PART1].explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.partInformation[TEST_PART1].explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.partInformation[TEST_PART1].explain[configInformation.partInformation[TEST_PART1].explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
		
		case ID_PART2_EXPLAIN:
			configInformation.partInformation[TEST_PART2].explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.partInformation[TEST_PART2].explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.partInformation[TEST_PART2].explain[configInformation.partInformation[TEST_PART2].explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART3_EXPLAIN:
			configInformation.partInformation[TEST_PART3].explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.partInformation[TEST_PART3].explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.partInformation[TEST_PART3].explain[configInformation.partInformation[TEST_PART3].explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART4_EXPLAIN:
			configInformation.partInformation[TEST_PART4].explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.partInformation[TEST_PART4].explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.partInformation[TEST_PART4].explain[configInformation.partInformation[TEST_PART4].explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART5_EXPLAIN:
			configInformation.partInformation[TEST_PART5].explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.partInformation[TEST_PART5].explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.partInformation[TEST_PART5].explain[configInformation.partInformation[TEST_PART5].explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART6_EXPLAIN:
			configInformation.partInformation[TEST_PART6].explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.partInformation[TEST_PART6].explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.partInformation[TEST_PART6].explain[configInformation.partInformation[TEST_PART6].explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PN_NUMBER:
			configInformation.PNNumDisplayIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.PNNumDisplayIdx < MAX_BYTE_NUM_PN_NUM)
				{
					configInformation.PNNumDisplay[configInformation.PNNumDisplayIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PN_DESCRIP:
			configInformation.descriptionStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.descriptionStringIdx < MAX_BYTE_NUM_PN_NUM_DESCRIP)
				{
					configInformation.PNNumDescription[configInformation.descriptionStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			

		case ID_ADJUST_REA_CURRNT:
			adjustInformation.inputCurrentStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(adjustInformation.inputCurrentStringIdx < MAX_BYTE_NUM_DISPLAY_CURRENT)
				{
					adjustInformation.inputCurrentString[adjustInformation.inputCurrentStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			adjustInformation.isDataReced = true;
			break;
			
		case ID_INPUT_PASSWORD:
			passwordInformation.inputPasswordStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.inputPasswordStringIdx < MAX_BYTE_NUM_PASSWORD)
				{
					passwordInformation.inputPassword[passwordInformation.inputPasswordStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			passwordInformation.isGetOldPassword = true;
			break;
			
		case ID_OLD_PASSWORD:
			passwordInformation.oldPasswordStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.oldPasswordStringIdx <MAX_BYTE_NUM_PASSWORD)
				{
					passwordInformation.oldPassword[passwordInformation.oldPasswordStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;

		case ID_NEW_PASSWORD:
			passwordInformation.newPasswordStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.newPasswordStringIdx < MAX_BYTE_NUM_PASSWORD)
				{
					passwordInformation.newPassword[passwordInformation.newPasswordStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			passwordInformation.isGetNewPassword = true;
			break;		
			
		case ID_OLD_PASSWORD_TIP:
			passwordInformation.oldPasswordTipStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.oldPasswordTipStringIdx < MAX_BYTE_NUM_PASSWORD_TIP)
				{
					passwordInformation.oldPasswordTip[passwordInformation.oldPasswordTipStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
		
		case ID_NEW_PASSWORD_TIP:
			passwordInformation.newPasswordTipStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.newPasswordTipStringIdx < MAX_BYTE_NUM_PASSWORD_TIP)
				{
					passwordInformation.newPasswordTip[passwordInformation.newPasswordTipStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}			
			break;
			
		default:
			return false;
			
	}
	return true;
}

/**
* @ Function Name : parser_rec_bitmap_data_deal
* @ Author        : hlb
* @ Brief         : 读位图控件返回数据包处理函数
* @ Date          : 2017.07.10
* @ Input         : u16							数据ID号
* @ Output        : bool						是否接收成功
* @ Modify        : ...
**/
bool parser_rec_bitmap_data_deal(u16 IDNum)
{
	switch(IDNum)
	{
		case ID_TRIGGER:
			configInformation.trigger = \
				ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BITMAP_STATUS_ADDRESS];
			break;
		
		case ID_TEST_OK:
			testingInformation.resultBitmapOK = \
				ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BITMAP_STATUS_ADDRESS];
			break;
		
		case ID_TEST_NG:
			testingInformation.resultBitmapNG = \
				ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BITMAP_STATUS_ADDRESS];
			break;
	
		default:
			return false;
	}
	
	return true;
}

/**
* @ Function Name : parser_rec_select_data_deal
* @ Author        : hlb
* @ Brief         : 读下拉列表控件返回数据包处理函数
* @ Date          : 2017.07.10
* @ Input         : u16							数据ID号
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_select_data_deal(u16 IDNum)
{
	switch(IDNum)
	{
		case ID_PN_NUMBER_SELECT:
			configInformation.PNNumSelect = \
				ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_SELECT_ADDRESS];
			break;
		
		case ID_POWER_SELECT:
			configInformation.powerSelect = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_SELECT_ADDRESS];
		
			//电源作为最后一个数据下拉 以判断数据是否全部接收
			configInformation.isDataReced = true;
			break;
		
		default:
			return false;
	}
	
	return true;
}

/**
* @ Function Name : parser_rec_bar_data_deal
* @ Author        : hlb
* @ Brief         : 读进度条控件返回数据包处理函数
* @ Date          : 2017.07.11
* @ Input         : u16							数据ID号
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_bar_data_deal(u16 IDNum)
{
	if(IDNum == ID_TEST_PROGRESS_BAR)
	{
		testingInformation.testProgressBar = \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BAR_NOW_PORGRESS_ADDRESS_HIGH] << 8) \
			+ ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BAR_NOW_PORGRESS_ADDRESS_HIGH + 1];
		testingInformation.testProgressBarMax = \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BAR_MAX_PORGRESS_ADDRESS_HIGH] << 8) \
			+ ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_BAR_NOW_PORGRESS_ADDRESS_HIGH + 1];
	}
	else
	{
		return false;
	}
	
	return true;
}

/**
* @ Function Name : parser_rec_time_data_deal
* @ Author        : hlb
* @ Brief         : 读时间控件返回数据包处理函数
* @ Date          : 2017.07.12
* @ Input         : u16							数据ID号
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_time_data_deal(u16 IDNum)
{
	if(IDNum == ID_BEGIN_TIME)
	{
		beginInformation.time.hour = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_TIME_HOUR_ADDRESS];
		beginInformation.time.minute = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_TIME_MINUTE_ADDRESS];
		beginInformation.time.second = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_TIME_SECOND_ADDRESS];
	}
	else
	{
		return false;
	}
	
	return true;
}

/**
* @ Function Name : parser_rec_date_data_deal
* @ Author        : hlb
* @ Brief         : 读日期控件返回数据包处理函数
* @ Date          : 2017.07.12
* @ Input         : u16							数据ID号
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_date_data_deal(u16 IDNum)
{
	if(IDNum == ID_BEGIN_DATE)
	{
		beginInformation.date.year = \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_DATE_YEAR_ADDRESS_HIGH] << 8) + \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_DATE_YEAR_ADDRESS_HIGH + 1];
		beginInformation.date.month = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_DATE_MONTH_ADDRESS];
		beginInformation.date.day = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_DATE_DAY_ADDRESS];
		beginInformation.date.week = \
			ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_DATE_WEEK_ADDRESS];
	}
	else
	{
		return false;
	}
	
	return true;
}

/**
* @ Function Name : parser_rec_page_data_deal
* @ Author        : hlb
* @ Brief         : 读页面返回数据包处理函数
* @ Date          : 2017.07.12
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_page_data_deal(void)
{
	globalInformation.nowPage = \
		ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_PAGE_IDX_ADDRESS];

	return true;
}

/**
* @ Function Name : parser_rec_gui_select_data_deal
* @ Author        : hlb
* @ Brief         : 下拉列表返回数据包处理函数
* @ Date          : 2017.07.26
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool paser_rec_gui_select_data_deal(u16 IDNum)
{
	switch(IDNum)
	{
		case ID_PN_NUMBER_SELECT:
			configInformation.PNNumSelect = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_SELECT_ADDRESS];
		
			configInformation.isDataUpdate = false;
			break;
		default:
			return false;
	}
	return true;
}

/**
* @ Function Name : parser_rec_gui_page_data_deal
* @ Author        : hlb
* @ Brief         : 读页面返回数据包处理函数
* @ Date          : 2017.07.24
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_gui_page_data_deal(u16 IDNum)
{
	globalInformation.nowPage = \
		ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_PAGE_IDX_ADDRESS];
	//更新界面初始化标志
	globalInformation.isPageInit = false;

	return true;
}

/**
* @ Function Name : parser_rec_gui_int_data_deal
* @ Author        : hlb
* @ Brief         : GUI整型返回数据包处理函数
* @ Input         : u16							数据ID号
* @ Date          : 2017.07.24
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_gui_int_data_deal(u16 IDNum)
{
	switch(IDNum)
	{
		case ID_PART_NUMBER:
			configInformation.partNum = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_INT_ADDRESS_HIGH + 3];
			if(configInformation.partNum > MAX_PART_NUM)
			{
				//限制最大段数				
				configInformation.partNum = MAX_PART_NUM;
				write_int(ID_PART_NUMBER, MAX_PART_NUM);
			}
			//更新段号
			configInformation.isPartUpdate = false;
			break;
			
		case ID_DEBUG_INPUT_POLES:
			debugInformation.inputPoles = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_INT_ADDRESS_HIGH + 3];
			break;
		
		case ID_DEBUG_INPUT_PWM_FREQ:
			debugInformation.inputPWMFrequen = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_INT_ADDRESS_HIGH + 3];
			debugInformation.isSetPWM = true;
			break;
		
		case ID_DEBUG_INPUT_PWM_DUTY:
			debugInformation.inputPWM = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_INT_ADDRESS_HIGH + 3];
			debugInformation.isSetPWM = true;
			break;
		
		default:
			return false;
	}
	
	return true;
}

/**
* @ Function Name : parser_rec_gui_select_data_deal
* @ Author        : hlb
* @ Brief         : GUI下拉列表返回数据包处理函数
* @ Input         : u16							数据ID号
* @ Date          : 2017.07.24
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_gui_select_data_deal(u16 IDNum)
{
	switch(IDNum)
	{
		case ID_PN_NUMBER_SELECT:
			configInformation.PNNumSelect = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_SELECT_ADDRESS];
			//更新数据标志开启
			configInformation.isDataUpdate = false;
			break;
		
		default:
			return false;
	}
	
	return true;
}


/**
* @ Function Name : parser_rec_gui_button_data_deal
* @ Author        : hlb
* @ Brief         : GUI按键返回数据包处理函数
* @ Input         : u16							数据ID号
* @ Date          : 2017.07.22
* @ Output        : bool						是否处理成功
* @ Modify        : ...
**/
bool parser_rec_gui_button_data_deal(u16 IDNum)
{
	if(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_GUI_DATA_BUTTON_ADDRESS] == 1)
	{
		switch(IDNum)
		{
	
			case ID_CONFIG_SAVE_BUTTON:
				configInformation.isSaveButtonDown = true;
				configInformation.isDataPull	   = true;
				break;
			
			case ID_DELETE_PN_BUTTON:
				configInformation.isDeleteButtonDown = true;
				break;
			
			case ID_ADJUST_SAVE_BUTTON:
				adjustInformation.isSaveArgButtonDown = true;
				adjustInformation.isDataPull	      = true;
				break;
			
			case ID_ADJUST_AUTO_ZERO_BUTTON:
				adjustInformation.isAutoZeroButtonDown = true;
				break;
			
			case ID_PASSWORD_OK_BUTTON:
				passwordInformation.isOkButtonDown  = true;
				passwordInformation.isDataPullInput = true;
				break;
			
			case ID_PASSWORD_CHANGE_BUTTON:
				passwordInformation.isChangeButtonDown = true;
				break;
			
			case ID_PASSWORD_CHANGE_OK_BUTTON:
				passwordInformation.isChangeOkButtonDown = true;
				passwordInformation.isDataPullNew   	 = true;
				break;
			
			default:
				return false;
		}
		return true;
	}
	
	return false;
	
}

/**
* @ Function Name : serial_parser_comm_analysis
* @ Author        : hlb
* @ Brief         : 解析器数据分析
* @ Date          : 2017.06.7
* @ Output        : bool							是否解析成功
* @ Modify        : ...
**/
bool serial_parser_comm_analysis(void)
{
	u8  dataNum;
	u8  dataFunc;
	u16 IDNum;
	u8  dataType;
	
	//读取数据包字符数
	dataNum = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_NUM_ADDRESS];
	
	//读取数据包功能字符
	dataFunc = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_HEAD_ADDRESS];
	
	//读取控件ID
	IDNum = (ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_ID_ADDRESS_HIGH] << 8) + \
	ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_ID_ADDRESS_LOW];
	
	//读取数据包数据类型
	dataType = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_TYPE_ADDRESS];
	
	//接收gui返回信息
	if(dataFunc == FRAME_REC_GUI_READ_DATA_HEAD )
	{
		switch(dataType)
		{
			case FRAME_REC_DATA_BUTTON_DEF:
				parser_rec_gui_button_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_PAGE_DEF:
				parser_rec_gui_page_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_SELECT_DEF:
				paser_rec_gui_select_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_INT_DEF:
				parser_rec_gui_int_data_deal(IDNum);
				break;
			
			default:
				return false;
				
		}
	}
	//接收到界面下拉信息
	else if(dataFunc == FRAME_REC_READ_PAGE_DATA_HEAD)
	{
		parser_rec_page_data_deal();
	}
	//接收到数据下拉信息
	else if(dataFunc == FRAME_REC_READ_MES_DATA_HEAD)
	{
		switch(dataType)
		{
			
			case FRAME_REC_DATA_INT_DEF:
				parser_rec_int_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_STRING_DEF:
				parser_rec_string_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_BITMAP_DEF:
				parser_rec_bitmap_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_SELECT_DEF:
				parser_rec_select_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_BAR_DEF:
				parser_rec_bar_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_TIME_DEF:
				parser_rec_time_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_DATE_DEF:
				parser_rec_date_data_deal(IDNum);
				break;
			
			default:
				return false;
		}
	}
	
	return true;

}

/**
* @ Function Name : usart_analysis_handle
* @ Author        : hlb
* @ Brief         : 串口数据解析线程
* @ Date          : 2017.06.05
* @ Modify        : ...
**/
void usart_analysis_handle(void)
{
	
	//解析器装载缓冲
	parser_moniter_add_buff();
	
	if(get_parser_buffers_length() > 0)
	{
		//解析帧
		if(serial_parser_comm_analysis() == true)
		{
			ParserMoniter.GetIdx++;	
		}
		else
		{		
			ParserMoniter.GetIdx++;
		}
		
		if(ParserMoniter.GetIdx >= FRAME_BUFFER_NUM_MAX)
		{
			ParserMoniter.GetIdx = 0;
		}
	}
}

