#ifndef __MESSAGE_ANALYSIS_H_
#define __MESSAGE_ANALYSIS_H_

#include "IDnum.h"
#include "config.h"

/*-------------------------------------------------------------------------------------------------------------
帧默认宏定义
-------------------------------------------------------------------------------------------------------------*/
//帧相关定义
#define FRAME_HEAD_DEF							0x55
#define FRAME_END_DEF							0xaa

//缓冲项最大长度定义
#define SERIAL_BUFFER_MAX_VAL					128

//最大缓冲数量定义
#define SERIAL_FRAME_BUFFER_NUM_MAX				4

//串口缓冲项定义
typedef struct 
{
	u8   Buff[SERIAL_BUFFER_MAX_VAL];								//缓冲内容
	u16  Idx;														//缓冲索引，指示长度
	bool IsBuffer_Add;
}Serial_BuffItem;

//解析器定义
typedef struct
{
	Serial_BuffItem Frame_Buffers[SERIAL_FRAME_BUFFER_NUM_MAX];		//默认缓冲
	bool IsFrame_Buffers_Empty;										//缓冲区是否为空
	bool IsFrmae_Buffers_Full;										//缓冲区是否已满
	u16  AddIdx;													//添加索引
	u16  GetIdx;													//取出索引
}Serial_Parser_Moniter_TypeDef;

extern Serial_Parser_Moniter_TypeDef ParserMoniter;					//外部声明解析器

//计算串口缓存区长度
u16  get_usart_buffer_length(void);

//解析器数据装载
void parser_moniter_add_buff(void);
	
//串口数据分析控制器
void Serial_analysis_handle(void);

#endif
