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


//解析器实例化
Serial_Parser_Moniter_TypeDef ParserMoniter;

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
		length = USART_REC_LEN - UsartRxBuffer.GetIdx + UsartRxBuffer.AddIdx;		
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
		if(UsartRxBuffer.GetIdx >= USART_REC_LEN )
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
			configInformation.part1Information.testPWM = data;
			break;
		
		case ID_PART1_TIME:
			configInformation.part1Information.testTime = data;
			break;
		
		case ID_PART1_UP_SPEED:
			configInformation.part1Information.testSpeedUpLimit = data;
			break;
		
		case ID_PART1_DOWN_SPEED:
			configInformation.part1Information.testSpeedDownLimit = data;
			break;
		
		case ID_PART1_UP_CURRENT:
			configInformation.part1Information.testCurrentUpLimit = data;
			break;
		
		case ID_PART1_DOWN_CURRENT:
			configInformation.part1Information.testCurrentDownLimit = data;
			break;

		case ID_PART2_PWM:
			configInformation.part2Information.testPWM = data;
			break;
		
		case ID_PART2_TIME:
			configInformation.part2Information.testTime = data;
			break;
		
		case ID_PART2_UP_SPEED:
			configInformation.part2Information.testSpeedUpLimit = data;
			break;
		
		case ID_PART2_DOWN_SPEED:
			configInformation.part2Information.testSpeedDownLimit = data;
			break;
		
		case ID_PART2_UP_CURRENT:
			configInformation.part2Information.testCurrentUpLimit = data;
			break;
		
		case ID_PART2_DOWN_CURRENT:
			configInformation.part2Information.testCurrentDownLimit = data;
			break;
		
		case ID_PART3_PWM:
			configInformation.part3Information.testPWM = data;
			break;
		
		case ID_PART3_TIME:
			configInformation.part3Information.testTime = data;
			break;
		
		case ID_PART3_UP_SPEED:
			configInformation.part3Information.testSpeedUpLimit = data;
			break;
		
		case ID_PART3_DOWN_SPEED:
			configInformation.part3Information.testSpeedDownLimit = data;
			break;
		
		case ID_PART3_UP_CURRENT:
			configInformation.part3Information.testCurrentUpLimit = data;
			break;
		
		case ID_PART3_DOWN_CURRENT:
			configInformation.part3Information.testCurrentDownLimit = data;
			break;
		
		case ID_PART4_PWM:
			configInformation.part4Information.testPWM = data;
			break;
		
		case ID_PART4_TIME:
			configInformation.part4Information.testTime = data;
			break;
		
		case ID_PART4_UP_SPEED:
			configInformation.part4Information.testSpeedUpLimit = data;
			break;
		
		case ID_PART4_DOWN_SPEED:
			configInformation.part4Information.testSpeedDownLimit = data;
			break;
		
		case ID_PART4_UP_CURRENT:
			configInformation.part4Information.testCurrentUpLimit = data;
			break;
		
		case ID_PART4_DOWN_CURRENT:
			configInformation.part4Information.testCurrentDownLimit = data;
			break;
		
		case ID_PART5_PWM:
			configInformation.part5Information.testPWM = data;
			break;
		
		case ID_PART5_TIME:
			configInformation.part5Information.testTime = data;
			break;
		
		case ID_PART5_UP_SPEED:
			configInformation.part5Information.testSpeedUpLimit = data;
			break;
		
		case ID_PART5_DOWN_SPEED:
			configInformation.part5Information.testSpeedDownLimit = data;
			break;
		
		case ID_PART5_UP_CURRENT:
			configInformation.part5Information.testCurrentUpLimit = data;
			break;
		
		case ID_PART5_DOWN_CURRENT:
			configInformation.part5Information.testCurrentDownLimit = data;
			break;
		
		case ID_PART6_PWM:
			configInformation.part6Information.testPWM = data;
			break;
		
		case ID_PART6_TIME:
			configInformation.part6Information.testTime = data;
			break;
		
		case ID_PART6_UP_SPEED:
			configInformation.part6Information.testSpeedUpLimit = data;
			break;
		
		case ID_PART6_DOWN_SPEED:
			configInformation.part6Information.testSpeedDownLimit = data;
			break;
		
		case ID_PART6_UP_CURRENT:
			configInformation.part6Information.testCurrentUpLimit = data;
			break;
		
		case ID_PART6_DOWN_CURRENT:
			configInformation.part6Information.testCurrentDownLimit = data;
			break;
		
		case ID_POLES_NUMBER:
			configInformation.polesNum = data;
			break;
		
		case ID_PAET_NUMBER:
			configInformation.partNum = data;
			break;
		
		case ID_TEST_CURRENT:
			configInformation.testCurrent = data;
			break;
		
		case ID_CONFIG_PWM:
			configInformation.PWM = data;
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
			configInformation.part1Information.explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.part1Information.explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.part1Information.explain[configInformation.part1Information.explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
		
		case ID_PART2_EXPLAIN:
			configInformation.part2Information.explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.part2Information.explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.part2Information.explain[configInformation.part2Information.explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART3_EXPLAIN:
			configInformation.part3Information.explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.part3Information.explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.part3Information.explain[configInformation.part3Information.explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART4_EXPLAIN:
			configInformation.part4Information.explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.part4Information.explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.part4Information.explain[configInformation.part4Information.explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART5_EXPLAIN:
			configInformation.part5Information.explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.part5Information.explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.part5Information.explain[configInformation.part5Information.explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PART6_EXPLAIN:
			configInformation.part6Information.explainStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.part6Information.explainStringIdx < MAX_BYTE_NUM_SPECIF_EXPLAIN)
				{
					configInformation.part6Information.explain[configInformation.part6Information.explainStringIdx++] =  \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_PN_NUMBER:
			configInformation.PNNumStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(configInformation.PNNumStringIdx < MAX_BYTE_NUM_PN_NUM)
				{
					configInformation.PNNum[configInformation.PNNumStringIdx++] = \
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
					adjustInformation.inputCurrent[adjustInformation.inputCurrentStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_INPUT_PASSWORD:
			passwordInformation.inputPasswordStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.inputPasswordStringIdx < MAX_BYTE_NUM_DISPLAY_CURRENT)
				{
					passwordInformation.inputPassword[passwordInformation.inputPasswordStringIdx++] = \
						ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_STRING_VALUE_ADDRESS_HIGH + i];
				}
				else
				{
					break;
				}
			}
			break;
			
		case ID_OLD_PASSWORD:
			passwordInformation.oldPasswordStringIdx = 0;
			for(i = 0; i < byteNum; i++)
			{
				if(passwordInformation.oldPasswordStringIdx < MAX_BYTE_NUM_DISPLAY_CURRENT)
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
				if(passwordInformation.newPasswordStringIdx < MAX_BYTE_NUM_DISPLAY_CURRENT)
				{
					passwordInformation.newPassword[passwordInformation.newPasswordStringIdx++] = \
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
* @ Function Name : parser_rec_picture_data_deal
* @ Author        : hlb
* @ Brief         : 读位图控件返回数据包处理函数
* @ Date          : 2017.07.10
* @ Input         : u16							数据ID号
* @ Output        : bool						是否接收成功
* @ Modify        : ...
**/
bool parser_rec_picture_data_deal(u16 IDNum)
{
	
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
			break;
		
		default:
			return false;
	}
	return true;
}

//读进度条控件返回数据包处理函数
bool parser_rec_bar_data_deal(u16 IDNum);

//读时间控件返回数据包处理函数
bool parser_rec_time_data_deal(u16 IDNum);

//读日期控件返回数据包处理函数
bool parser_rec_date_data_deal(u16 IDNum);

//读页面返回数据包处理函数
bool parser_rec_page_data_deal(void)
{
	
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
	dataFunc = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FEAME_DATA_FUNC_ADDRESS];
	
	if(dataFunc == FRAME_REC_PAGE_INSTRUCTION_DEF)
	{
		
	}
	else if(dataFunc == FRAME_REC_MES_INSTRUCTION_DEF)
	{
		//读取控件ID
		IDNum = (ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_ID_ADDRESS_HIGH] << 8) + \
		ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_ID_ADDRESS_LOW];
		
		//读取数据包数据类型
		dataType = ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_TYPE_ADDRESS];
		
		switch(dataType)
		{
			case FRAME_REC_DATA_INT_DEF:
				parser_rec_int_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_STRING_DEF:
				parser_rec_string_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_STATUES_DEF:
				parser_rec_picture_data_deal(IDNum);
				break;
			
			case FRAME_REC_DATA_SELECT_DEF:
				break;
			
			case FRAME_REC_DATA_BAR_DEF:
				break;
			
			case FRAME_REC_DATA_TIME_DEF:
				break;
			
			case FRAME_REC_DATA_DATE_DEF:
				break;
			
			default:
				return false;
		}
	}
	
	return true;

}

/**
* @ Function Name : Serial_analysis_handle
* @ Author        : hlb
* @ Brief         : 串口数据控制器线程
* @ Date          : 2017.06.5
* @ Modify        : ...
**/
void Serial_message_handle(void)
{

	//解析器装载缓冲
	parser_moniter_add_buff();
	
	if(get_parser_buffers_length() > 0)
	{
//		for(i = 1; i < ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Idx; i++)
//		{
//			USART_SendData(USART1,ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[i]);
//		}
		
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

