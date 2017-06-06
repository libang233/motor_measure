/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : message_analysis.c
 * Author    : hlb
 * Brief     : 数据分析匹配代码
 * Date      : 2017.05.31
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "sys_manager.h"
#include "usart.h"
#include "newtype.h"
#include "message_analysis.h"

Serial_Parser_Moniter_TypeDef ParseMoniter;

/**
* @ Function Name : get_Usart_buffer_length
* @ Author        : hlb
* @ Brief         : 计算缓冲数据长度
* @ Date          : 2017.06.6
* @ Output        : u16							串口缓冲区长度
* @ Modify        : ...
**/
u16 get_usart_buffer_length(void)
{
	int length;
	length = UsartRxBuffer.AddIdx - UsartRxBuffer.GetIdx;
	if(length < 0)
	{
		length = USART_REC_LEN - UsartRxBuffer.GetIdx + UsartRxBuffer.AddIdx + 1;		
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
	
	if(get_usart_buffer_length() > 0)
	{
		if(UsartRxBuffer.GetIdx >= USART_REC_LEN )
		{
			UsartRxBuffer.GetIdx = 0;
		}
		Res = UsartRxBuffer.Buff[UsartRxBuffer.GetIdx++];
		if(ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].IsBuffer_Add == 0)
		{
			if(Res == FRAME_HEAD_DEF)																	//接收到帧头，开始接收数据
			{
				ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].IsBuffer_Add = 1;
			}
		}
		
		if(ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].IsBuffer_Add == 1)
		{
			if(ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].Idx < SERIAL_BUFFER_MAX_VAL)
			{
				ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].Buff[ \
				ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].Idx++] = Res;							//缓冲项填充数据
			}
			else
			{
				ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].Idx = 0;
				ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].IsBuffer_Add = 0;						//超出最大范围，
			}
			
			if(Res == FRAME_END_DEF)																	//接收到帧尾
			{
				if(ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].Buff[1] \
					== (ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].Idx + 4))						//帧校验
				{
					ParseMoniter.Frame_Buffers[ParseMoniter.AddIdx].IsBuffer_Add = 0;
					if(ParseMoniter.AddIdx < SERIAL_FRAME_BUFFER_NUM_MAX)								//添加缓冲项
					{
						ParseMoniter.AddIdx++;
					}
					else
					{
						ParseMoniter.AddIdx = 0;
					}
					
				}
			}	
		}
	}
}


/**
* @ Function Name : Serial_analysis_handle
* @ Author        : hlb
* @ Brief         : 串口数据的分析控制器线程
* @ Date          : 2017.06.5
* @ Modify        : ...
**/
void Serial_analysis_handle(void)
{
	
	parser_moniter_add_buff();


}
