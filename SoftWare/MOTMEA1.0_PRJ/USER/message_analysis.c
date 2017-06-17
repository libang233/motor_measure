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
		if(ParserMoniter.IsBuffer_Add == false)
		{
			if(Res == FRAME_HEAD_DEF)																	
			{
				//接收到帧头，开始接收数据
				ParserMoniter.IsBuffer_Add = true;
			}
		}
		
		if(ParserMoniter.IsBuffer_Add == true)
		{
			if(ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx < SERIAL_BUFFER_MAX_VAL)
			{
				//缓冲项填充数据
				ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Buff[ \
				ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx++] = Res;							
			}
			else
			{
				//超出最大范围，丢弃数据
				ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx = 0;
				ParserMoniter.IsBuffer_Add = false;						
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
						ParserMoniter.IsBuffer_Add = false;
						//恢复缓冲项索引
						ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx = 0;
					}
					else
					{
						//丢弃数据
						ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx = 0;
						ParserMoniter.IsBuffer_Add = false;		
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
* @ Modify        : ...
**/
void parser_rec_int_data_deal(u16 IDNum)
{
	u32 data;
	
	data = (ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG] << 24) + \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG + 1] << 16) + \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG + 2] << 8) + \
			(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG + 3]);

	switch(IDNum)
	{
		case PART1PWM:
			part1Information.testPWM = data;
			break;
		
		case PART1TIME:
			part1Information.testTime = data;
			break;
		
		case PART1UPSPEED:
			part1Information.testSpeedUpLimit = data;
			break;
		
		case PART1DOWNSPEED:
			part1Information.testSpeedDownLimit = data;
			break;
		
		case PART1UPCURRENT:
			part1Information.testCurrentUpLimit = data;
			break;
		
		case PART1DOWNCURRENT:
			part1Information.testCurrentDownLimit = data;
			break;

		case PART2PWM:
			part2Information.testPWM = data;
			break;
		
		case PART2TIME:
			part2Information.testTime = data;
			break;
		
		case PART2UPSPEED:
			part2Information.testSpeedUpLimit = data;
			break;
		
		case PART2DOWNSPEED:
			part2Information.testSpeedDownLimit = data;
			break;
		
		case PART2UPCURRENT:
			part2Information.testCurrentUpLimit = data;
			break;
		
		case PART2DOWNCURRENT:
			part2Information.testCurrentDownLimit = data;
			break;
		
		case PART3PWM:
			part3Information.testPWM = data;
			break;
		
		case PART3TIME:
			part3Information.testTime = data;
			break;
		
		case PART3UPSPEED:
			part3Information.testSpeedUpLimit = data;
			break;
		
		case PART3DOWNSPEED:
			part3Information.testSpeedDownLimit = data;
			break;
		
		case PART3UPCURRENT:
			part3Information.testCurrentUpLimit = data;
			break;
		
		case PART3DOWNCURRENT:
			part3Information.testCurrentDownLimit = data;
			break;
		
		case PART4PWM:
			part4Information.testPWM = data;
			break;
		
		case PART4TIME:
			part4Information.testTime = data;
			break;
		
		case PART4UPSPEED:
			part4Information.testSpeedUpLimit = data;
			break;
		
		case PART4DOWNSPEED:
			part4Information.testSpeedDownLimit = data;
			break;
		
		case PART4UPCURRENT:
			part4Information.testCurrentUpLimit = data;
			break;
		
		case PART4DOWNCURRENT:
			part4Information.testCurrentDownLimit = data;
			break;
		
		case PART5PWM:
			part5Information.testPWM = data;
			break;
		
		case PART5TIME:
			part5Information.testTime = data;
			break;
		
		case PART5UPSPEED:
			part5Information.testSpeedUpLimit = data;
			break;
		
		case PART5DOWNSPEED:
			part5Information.testSpeedDownLimit = data;
			break;
		
		case PART5UPCURRENT:
			part5Information.testCurrentUpLimit = data;
			break;
		
		case PART5DOWNCURRENT:
			part5Information.testCurrentDownLimit = data;
			break;
		
		case PART6PWM:
			part6Information.testPWM = data;
			break;
		
		case PART6TIME:
			part6Information.testTime = data;
			break;
		
		case PART6UPSPEED:
			part6Information.testSpeedUpLimit = data;
			break;
		
		case PART6DOWNSPEED:
			part6Information.testSpeedDownLimit = data;
			break;
		
		case PART6UPCURRENT:
			part6Information.testCurrentUpLimit = data;
			break;
		
		case PART6DOWNCURRENT:
			part6Information.testCurrentDownLimit = data;
			break;
		
		case POLESNUMBER:
			configInformation.polesNum = data;
			break;
		
		case PAETNUMBER:
			configInformation.partNum = data;
			break;
		
		case TESTCURRENT:
			configInformation.testCurrent = data;
			break;
		
		case CONFIGPWM:
			configInformation.PWM = data;
			break;
		
		case DEBUGINPUTPOLES:
			debugInformation.inputPoles = data;
			break;
		
		case DEBUGINPUTPWMFREQ:
			debugInformation.inputPWMFrequen = data;
			break;
		
		case DEBUGINPUTPWEMDUTY:
			debugInformation.inputPWM = data;
			break;
		
		default:
			break;
	}
}

//读字符串控件返回数据包处理函数
void parser_rec_string_data_deal(u16 IDNum);

//读位图控件返回数据包处理函数
void parser_rec_status_data_deal(u16 IDNum);

//读下拉列表控件返回数据包处理函数
void parser_rec_selec_data_deal(u16 IDNum);

//读进度条控件返回数据包处理函数
void parser_rec_bar_data_deal(u16 IDNum);

//读时间控件返回数据包处理函数
void parser_rec_time_data_deal(u16 IDNum);

//读日期控件返回数据包处理函数
void parser_rec_date_data_deal(u16 IDNum);

//读页面返回数据包处理函数
void parser_rec_page_data_deal(u16 IDNum);


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
	u8  i;
	u8  dataNum;
	u8  dataFunc;
	u16 IDNum;
	u8  dataType;
	u32 intData;
	
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
				break;
			
			case FRAME_REC_DATA_STATUES_DEF:
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
			
//		//接收到整型数据
//		if((controlID >= 204 && controlID <= 206) || (controlID >= 112 && controlID <= 147) \
//			|| controlID == 107 || controlID == 108 || controlID == 110 || controlID == 197)
//		{
//			intData = (ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG] << 24) + \
//				(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG + 1] << 16) + \
//				(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG + 2] << 8) + \
//				(ParserMoniter.Frame_Buffers[ParserMoniter.GetIdx].Buff[FRAME_DATA_INT_VALUE_ADDRESS_HIHG + 3]);
//			
//			switch(controlID)
//			{		
//					
//					
//			}
//		}
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
	static u16 i;
	u16 t;
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
			
		}
	}
	
	t = ParserMoniter.Frame_Buffers[ParserMoniter.AddIdx].Idx;


}

