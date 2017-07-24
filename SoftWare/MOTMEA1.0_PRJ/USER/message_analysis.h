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

#define FRAME_PULL_CONTROL_HEAD					0xe0				//下拉数据发送帧数据包头字符
#define FRAME_PULL_CONTROL_SIZE					9					//下拉数据发送帧大小
#define FRAME_PULL_CONTROL_VAL_ADDRESS			6					//下拉数据发送帧伪内容地址
#define	FRAME_PULL_CONTROL_VAL					0					//下拉数据发送帧伪内容
#define FRAME_PULL_CONTROL_DATA_NUM				5					//下拉数据发送帧数据包字节数

#define	FRAME_PULL_PAGE_HEAD					0xe9
#define FRAME_PULL_PAGE_SIZE					6
#define FRAME_PULL_PAGE_VAL_ADDRESS				3
#define FRAME_PULL_PAGE_VAL						0
#define FRAME_PULL_PAGE_DATA_NUM				2

#define FRAME_WRITE_STATE_DATA_HEAD				0xe6				//写状态数据包头字符
#define FRAME_WRITE_STATE_ARG_ADDRESS			5					//写状态参数地址		
#define FRAME_WRITE_STATE_SIZE					8					//写状态帧字符数
#define FRAME_WRITE_STATE_DATA_NUM 				4					//写状态数据包字节数

#define FRAME_WRITE_PAGE_DATA_HEAD				0xe8
#define FRAME_WRITE_PAGE_ARG_ADDRESS_HIGH		3
#define FRAME_WRITE_PAGE_ARG_ADDRESS_LOW		4
#define FRAME_WRITE_PAGE_SIZE					7
#define FRAME_WRITE_PAGE_DATA_NUM				3

#define FRAME_WRITE_TIME_DATA_HEAD				0xe5
#define FRAME_WRITE_TIME_HOUR_ADDRESS			5					
#define FRAME_WRITE_TIME_MINUTE_ADDRESS			6					
#define FRAME_WRITE_TIME_SECOND_ADDRESS			7				
#define FRAME_WRITE_TIME_SIZE					10
#define FRAME_WRITE_TIME_DATA_NUM 				6

#define FRAME_WRITE_DATE_DATA_HEAD				0xe5
#define FRAME_WRITE_DATE_YEAR_ADDRESS_HIHG		5	
#define FRAME_WRITE_DATE_YEAR_ADDRESS_LOW		6	
#define FRAME_WRITE_DATE_MONTH_ADDRESS			7					
#define FRAME_WRITE_DATE_DAY_ADDRESS			8				
#define FRAME_WRITE_DATE_SIZE					11
#define FRAME_WRITE_DATE_DATA_NUM 				7

#define FRAME_WRITE_BAR_DATA_HEAD				0xe5
#define FRAME_WRITE_BAR_VAL_ADDRESS_HIGH		5
#define FRAME_WRITE_BAR_VAL_ADDRESS_LOW			6
#define	FRAME_WRITE_BAR_SIZE					9
#define FRAME_WRITE_BAR_DATA_NUM				5

#define FRAME_WRITE_SELECT_POS_DATA_HEAD		0xe5
#define FRAME_WRITE_SELECT_POS_VAL_ADDRESS		5
#define	FRAME_WRITE_SELECT_POS_SIZE				8
#define FRAME_WRITE_SELECT_POS_DATA_NUM			4

#define FRAME_WRITE_SELECT_DATA_HEAD			0xe6
#define FRAME_WRITE_SELECT_TYPE					0x20	
#define FRAME_WRITE_SELECT_TYPE_ADDRESS			5
#define FRAME_WRITE_SELECT_IDX_ADDRESS			6
#define FRAME_WRITE_SELECT_VAL_ADDRESS_HIGH		7
#define	FRAME_WRITE_SELECT_MAX_SIZE			    32
#define FRAME_WRITE_SELECT_DATA_MAX_NUM			27

#define FRAME_WRITE_BITMAP_DATA_HEAD			0xe5
#define FRAME_WRITE_BITMAP_VAL_ADDRESS			5
#define	FRAME_WRITE_BITMAP_SIZE					8
#define FRAME_WRITE_BITMAP_DATA_NUM				4

#define FRAME_WRITE_CHAR_DATA_HEAD			    0xe5
#define FRAME_WRITE_CHAR_VAL_ADDRESS_HIGH		5
#define	FRAME_WRITE_CHAR_MAX_SIZE				30	
#define FRAME_WRITE_CHAR_DATA_MAX_NUM			26

#define FRAME_WRITE_INT_DATA_HEAD			    0xe5
#define FRAME_WRITE_INT_VAL_ADDRESS_HIGH		5
#define	FRAME_WRITE_INT_SIZE					11
#define FRAME_WRITE_INT_DATA_NUM			    7
#define FRAME_WRITE_INT_VAL_NUM					4

#define FRAME_REC_READ_MES_DATA_HEAD			0xe0				//数据帧返回数据包读控件数据包头字符
#define FRAME_REC_READ_PAGE_DATA_HEAD			0xe9				//数据帧返回数据包页面数据包头字符
#define FRAME_REC_GUI_READ_DATA_HEAD			0xeb				//数据帧GUI主动返回数据包头字符

#define FRAME_REC_DATA_INT_DEF					0x22				//数据帧返回数据包整型数据类型字符
#define FRAME_REC_DATA_STRING_DEF				0x23				//数据帧返回数据包字符串数据类型字符
#define FRAME_REC_DATA_STATUES_DEF				0x21				//数据帧返回数据包状态数据类型字符		
#define FRAME_REC_DATA_SELECT_DEF				0x25				//数据帧返回数据包索引数据类型字符	
#define FRAME_REC_DATA_BAR_DEF					0x10				//数据帧返回数据包进度条数据类型字符	
#define FRAME_REC_DATA_TIME_DEF					0x13				//数据帧返回数据包时间数据类型字符	
#define FRAME_REC_DATA_DATE_DEF					0x14				//数据帧返回数据包日期数据类型字符	
#define FRAME_REC_DATA_BUTTON_DEF				0x20				//数据帧返回数据包按键数据类型字符	
#define FRAME_REC_DATA_PAGE_DEF					0xff				//数据帧返回数据包页面数据类型字符

#define FRAME_HEAD_ADDRESS						0					//数据帧头字符地址
#define FRAME_DATA_NUM_ADDRESS					1					//数据帧数据包字节数地址
#define FRAME_DATA_HEAD_ADDRESS					2					//数据帧功能字节地址
#define FRAME_DATA_ID_ADDRESS_HIGH				3					//数据帧ID高字节地址
#define FRAME_DATA_ID_ADDRESS_LOW				4					//数据帧ID低字节地址			
#define FRAME_DATA_TYPE_ADDRESS					5					//数据帧数据类型字节地址

#define FRAME_DATA_INT_VALUE_ADDRESS_HIGH		6					//数据帧整型数值高字节地址

#define FRAME_DATA_STRING_BYTE_NUM_ADDRESS		6					//数据帧字符串型数据字节数字节地址
#define FRAME_DATA_STRING_VALUE_ADDRESS_HIGH	7					//数据帧字符串数据高字节地址

#define FRAME_DATA_BITMAP_STATUS_ADDRESS        6					//数据帧位图控件状态字节地址

#define FRAME_DATA_SELECT_ADDRESS				6					//数据帧下拉列表数据字节地址

#define FRAME_DATA_BAR_NOW_PORGRESS_ADDRESS_HIGH 	6				//数据帧进度条当前进度值高字节地址
#define FRAME_DATA_BAR_MAX_PORGRESS_ADDRESS_HIGH	8				//数据帧进度条最大进度值高字节地址

#define FRAME_DATA_TIME_HOUR_ADDRESS			6					//数据帧时间控件小时地址
#define FRAME_DATA_TIME_MINUTE_ADDRESS			7					//数据帧时间控件分钟地址
#define FRAME_DATA_TIME_SECOND_ADDRESS			8					//数据帧时间控件秒地址

#define FRAME_DATA_DATE_YEAR_ADDRESS_HIGH       6					//数据帧日期控件年高字节地址
#define FRAME_DATA_DATE_MONTH_ADDRESS			8					//数据帧日期控件月地址
#define FRAME_DATA_DATE_DAY_ADDRESS				9					//数据帧日期控件日地址
#define FRAME_DATA_DATE_WEEK_ADDRESS			10					//数据帧日期控件星期地址

#define FRAME_DATA_PAGE_IDX_ADDRESS				4					//数据帧页面索引地址
#define FRAME_GUI_DATA_PAGE_IDX_ADDRESS			7					//数据帧GUI返回页面地址

#define	FRAME_GUI_DATA_SELECT_ADDRESS			6					//数据帧GUI返回下拉列表数据地址
#define	FRAME_GUI_DATA_INT_ADDRESS_HIGH			6					//数据帧GUI返回整数高地址
#define FRAME_GUI_DATA_BUTTON_ADDRESS			6					//数据帧GUI返回按键按下地址

//校验位同数据包字节数地址偏移量
#define FRAME_CHECK_ADDRESS_OFFSET				2
//结尾位同数据包字节数地址偏移量
#define FRAME_END_ADDRESS_OFFSET				3
//帧字节数同数据包字节数偏移量					
#define FRAME_SIZE_OFFSET						4
//下拉列表写值字符串字节数同数据包字节数偏移量
#define FRAME_WRITE_SELECT_OFFSET				5
//写字符串，字符串数同数据包字节数偏移量
#define FRAME_WRITE_CHAR_OFFSET					3

//页码定义
#define PAGE_NUM_BEGIN							0
#define PAGE_NUM_TEST							1
#define PAGE_NUM_CONFIG							2
#define PAGE_NUM_ADJUST							3
#define PAGE_NUM_PASSWORD_INPUT					4
#define PAGE_NUM_PASSWORD_WRONG					5
#define PAGE_NUM_DEBUG							6

//控件状态定义
#define CONTROL_DISENABLE						0
#define CONTROL_ENABLE							1
#define CONTROL_DISENABLE_DISAPPER				2


//缓冲项最大长度定义
#define USART_BUFFER_MAX_VAL					128

//最大缓冲数量定义
#define FRAME_BUFFER_NUM_MAX					4

//串口缓冲项定义
typedef struct 
{
	u8   Buff[USART_BUFFER_MAX_VAL];								//缓冲内容
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
	
}Usart_Parser_Moniter_TypeDef;

//外部声明解析器
extern Usart_Parser_Moniter_TypeDef ParserMoniter;	
				
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
bool parser_rec_bitmap_data_deal(u16 IDNum);

//读下拉列表控件返回数据包处理函数
bool parser_rec_select_data_deal(u16 IDNum);

//读进度条控件返回数据包处理函数
bool parser_rec_bar_data_deal(u16 IDNum);

//读时间控件返回数据包处理函数
bool parser_rec_time_data_deal(u16 IDNum);

//读日期控件返回数据包处理函数
bool parser_rec_date_data_deal(u16 IDNum);

//读页面返回数据包处理函数
bool parser_rec_page_data_deal(void);

//串口帧分析函数
bool serial_parser_comm_analysis(void);
	
//串口数据控制器线程
void usart_analysis_handle(void);

//解析器初始化
void parser_init(void);

#endif
