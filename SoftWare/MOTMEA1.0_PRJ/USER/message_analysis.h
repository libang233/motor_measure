#ifndef __MESSAGE_ANALYSIS_H_
#define __MESSAGE_ANALYSIS_H_

#include "IDnum.h"
#include "config.h"

/*-------------------------------------------------------------------------------------------------------------
帧默认宏定义
-------------------------------------------------------------------------------------------------------------*/
//帧相关定义
#define FRAME_HEAD_DEF							0x55				//数据帧头字符
#define FRAME_END_DEF							0xaa				//数据帧尾字符
#define FRAME_REC_MES_INSTRUCTION_DEF			0xe0				//数据帧返回数据包读控件数据包头字符
#define FRAME_REC_PAGE_INSTRUCTION_DEF			0xe9				//数据帧返回数据包读页码数据包头字符
#define FRAME_REC_DATA_INT_DEF					0x22				//数据帧返回数据包整型数据类型字符
#define FRAME_REC_DATA_STRING_DEF				0x23				//数据帧返回数据包字符串数据类型字符
#define FRAME_REC_DATA_STATUES_DEF				0x21				//数据帧返回数据包状态数据类型字符		
#define FRAME_REC_DATA_SELECT_DEF				0x25				//数据帧返回数据包索引数据类型字符	
#define FRAME_REC_DATA_BAR_DEF					0x10				//数据帧返回数据包进度条数据类型字符	
#define FRAME_REC_DATA_TIME_DEF					0x13				//数据帧返回数据包时间数据类型字符	
#define FRAME_REC_DATA_DATE_DEF					0x14				//数据帧返回数据包日期数据类型字符	

#define FRAME_DATA_NUM_ADDRESS					1					//数据帧数据包字节数地址
#define FEAME_DATA_FUNC_ADDRESS					2					//数据帧功能字节地址
#define FRAME_DATA_ID_ADDRESS_HIGH				3					//数据帧ID高字节地址
#define FRAME_DATA_ID_ADDRESS_LOW				4					//数据帧ID低字节地址
#define FRAME_DATA_TYPE_ADDRESS					5					//数据帧数据类型字节地址
#define FRAME_DATA_INT_VALUE_ADDRESS_HIGH		6					//数据帧整型数值高字节地址
#define FRAME_DATA_STRING_BYTE_NUM_ADDRESS		6					//数据帧字符串型数据字节数字节地址
#define FRAME_DATA_STRING_VALUE_ADDRESS_HIGH	7					//数据帧字符串数据高字节地址

//缓冲项最大长度定义
#define SERIAL_BUFFER_MAX_VAL					128

//最大缓冲数量定义
#define FRAME_BUFFER_NUM_MAX					4

//串口缓冲项定义
typedef struct 
{
	u8   Buff[SERIAL_BUFFER_MAX_VAL];								//缓冲内容
	u16  Idx;														//缓冲索引，指示长度	
	
}Usart_BuffItem;

//解析器定义
typedef struct
{
	void (*ParseStateFunc)(void);									//解析器解析函数指针
	Usart_BuffItem Frame_Buffers[FRAME_BUFFER_NUM_MAX];				//默认缓冲
	bool IsFrameBuffersEmpty;										//缓冲区是否为空
	bool IsFrmaeBuffersFull;										//缓冲区是否已满
	bool IsBufferAdd;												//缓冲区字符接收标志
	u16  AddIdx;													//添加索引
	u16  GetIdx;													//取出索引
	
}Serial_Parser_Moniter_TypeDef;

//外部声明解析器
extern Serial_Parser_Moniter_TypeDef ParserMoniter;	
				
//计算串口缓冲区长度
u16  get_usart_buffers_length(void);

//解析器数据装载
void parser_moniter_add_buff(void);

//计算解析器缓冲项长度
u8 get_parser_buffers_length(void);

//读数值控件返回数据包处理函数
bool parser_rec_int_data_deal(u16 IDNum);

//读字符串控件返回数据包处理函数
bool parser_rec_string_data_deal(u16 IDNum);

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
void parser_rec_page_data_deal(void);

//串口帧分析函数
bool serial_parser_comm_analysis(void);
	
//串口数据控制器线程
void Serial_message_handle(void);

//解析器初始化
void parser_init(void);

#endif
