/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : message_send.c
 * Author    : hlb
 * Brief     : 数据发送代码
 * Date      : 2017.07.18
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/

#include "message_send.h"
#include "message_analysis.h"
#include "usart.h"
#include "dma.h"


/**
* @ Function Name : usart_tx
* @ Author        : hlb
* @ Brief         : 串口发送接口
* @ Date          : 2017.07.19
* @ Input         : u8 *buff   发送的缓冲地址
*                   u32 size 发送的数据长度
* @ Modify        : ...
**/
void usart_tx(u8 *buff, u32 size)
{
	u32 freeSize = 0;
	freeSize = USART_TX_LEN - UsartTxBuffer.Idx[UsartTxBuffer.PartAvailAction];
	
	if(freeSize >= size)
	{
		memcpy(&UsartTxBuffer.Buff[UsartTxBuffer.PartAvailAction][UsartTxBuffer.Idx[UsartTxBuffer.PartAvailAction]], \
			buff, \
			size);
	}
	UsartTxBuffer.Idx[UsartTxBuffer.PartAvailAction] += size;
}

/**
* @ Function Name : frame_complete_tx
* @ Author        : hlb
* @ Brief         : 完整数据帧并发送接口
* @ Date          : 2017.07.19
* @ Input         : u8 *frame   发送的缓冲地址
					u8 dataSize 数据包大小
* @ Modify        : ...
**/
void frame_complete_tx(u8 *frame, u8 dataSize)
{
	u8 i;
	
	frame[FRAME_HEAD_ADDRESS] = FRAME_HEAD_DEF;
	frame[FRAME_DATA_NUM_ADDRESS] = dataSize;
	
	//计算校验位
	frame[dataSize + FRAME_CHECK_ADDRESS_OFFSET] = 0;
	for(i = 0; i < dataSize; i++)
	{
		frame[dataSize + FRAME_CHECK_ADDRESS_OFFSET] += \
			frame[FRAME_DATA_HEAD_ADDRESS + i];
	}
	frame[dataSize + FRAME_END_ADDRESS_OFFSET] = FRAME_END_DEF;
	
	usart_tx(frame, dataSize + FRAME_SIZE_OFFSET);
}

/**
* @ Function Name : write_int
* @ Author        : hlb
* @ Brief         : 写数值
* @ Date          : 2017.07.20
* @ Input         : u16   IDNum      ID号
					u32    val       数值
* @ Modify        : ... 
**/
void write_int(u16 IDNum, u32 val)
{
	int i;
	u8 frame[FRAME_WRITE_INT_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS]			  = FRAME_WRITE_INT_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	  = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		  = IDNum;
	for(i = FRAME_WRITE_INT_VAL_NUM - 1; i >= 0; i--)
	{
		frame[FRAME_WRITE_INT_VAL_ADDRESS_HIGH + FRAME_WRITE_INT_VAL_NUM - 1 - i] = (val << (8 * i));
	}
	
	frame_complete_tx(frame, FRAME_WRITE_INT_DATA_NUM);
}

/**
* @ Function Name : write_char
* @ Author        : hlb
* @ Brief         : 写字符
* @ Date          : 2017.07.20
* @ Input         : u16   IDNum      ID号
					u8    *val       字符串数据
					u8     valSize	 字符串长度
* @ Modify        : ... 
**/
void write_char(u16 IDNum, u8 *val, u8 valSize)
{
	u8 i;
	u8 frame[FRAME_WRITE_CHAR_MAX_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS]			  = FRAME_WRITE_CHAR_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	  = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		  = IDNum;
	for(i = 0; i < valSize; i++)
	{
		frame[FRAME_WRITE_CHAR_VAL_ADDRESS_HIGH + i] = val[i];
	}
	
	frame_complete_tx(frame, valSize + FRAME_WRITE_CHAR_OFFSET);
}

/**
* @ Function Name : write_bitmap
* @ Author        : hlb
* @ Brief         : 写位图
* @ Date          : 2017.07.20
* @ Input         : u16   IDNum       ID号
					u8    status      状态
* @ Modify        : ... 
**/
void write_bitmap(u16 IDNum, u8 status)
{
	u8 frame[FRAME_WRITE_BITMAP_SIZE];

	frame[FRAME_DATA_HEAD_ADDRESS]			  = FRAME_WRITE_BITMAP_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	  = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		  = IDNum;
	frame[FRAME_WRITE_BITMAP_VAL_ADDRESS]  	  = status;
	
	frame_complete_tx(frame, FRAME_WRITE_BITMAP_DATA_NUM);
}


/**
* @ Function Name : write_select_val
* @ Author        : hlb
* @ Brief         : 写下拉列表
* @ Date          : 2017.07.20
* @ Input         : u16   IDNum       ID号
					u8    Idx	      索引
					u8    valSize     写入内容
*                   u8   *val   	  内容字符数
* @ Modify        : ... 
**/
void write_select_val(u16 IDNum, u8 Idx, u8 valSize, u8 *val)
{
	u8 i;
	u8 frame[FRAME_WRITE_SELECT_MAX_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS] 			   = FRAME_WRITE_SELECT_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	   = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		   = IDNum;
	frame[FRAME_WRITE_SELECT_TYPE_ADDRESS]	   = FRAME_WRITE_SELECT_TYPE;
	frame[FRAME_WRITE_SELECT_IDX_ADDRESS]	   = Idx;
	for(i = 0; i < valSize; i++)
	{
		frame[FRAME_WRITE_SELECT_VAL_ADDRESS_HIGH + i] = val[i];
	}
	
	frame_complete_tx(frame, valSize + FRAME_WRITE_SELECT_OFFSET);
}

/**
* @ Function Name : write_select_position
* @ Author        : hlb
* @ Brief         : 写下拉列表
* @ Date          : 2017.07.20
* @ Input         : u16   IDNum   ID号
*                   u8    pos     选择项
* @ Modify        : ... 
**/
void write_select_position(u16 IDNum, u8 pos)
{
	u8 frame[FRAME_WRITE_SELECT_POS_SIZE];

	frame[FRAME_DATA_HEAD_ADDRESS]			  = FRAME_WRITE_SELECT_POS_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	  = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		  = IDNum;
	frame[FRAME_WRITE_SELECT_POS_VAL_ADDRESS] = pos;
	
	frame_complete_tx(frame, FRAME_WRITE_SELECT_POS_DATA_NUM);
}


/**
* @ Function Name : write_bar
* @ Author        : hlb
* @ Brief         : 写进度条
* @ Date          : 2017.07.19
* @ Input         : u16   IDNum   ID号
*                   u16   val     进度值
* @ Modify        : ... 
**/
void write_bar(u16 IDNum, u16 val)
{
	u8 frame[FRAME_WRITE_BAR_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS]			  = FRAME_WRITE_BAR_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	  = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		  = IDNum;
	frame[FRAME_WRITE_BAR_VAL_ADDRESS_HIGH]   = (val >> 8);
	frame[FRAME_WRITE_BAR_VAL_ADDRESS_LOW]	  = val;
	
	frame_complete_tx(frame, FRAME_WRITE_BAR_DATA_NUM);
}

/**
* @ Function Name : write_date
* @ Author        : hlb
* @ Brief         : 写日期
* @ Date          : 2017.07.19
* @ Input         : u16   IDNum   ID号
*                   u16   year    年
					u8    moth    月
					u8    day     日
* @ Modify        : ... 
**/
void write_date(u16 IDNum, u16 year, u8 month, u8 day)
{
	u8 frame[FRAME_WRITE_DATE_SIZE];

	frame[FRAME_DATA_HEAD_ADDRESS]	   		  = FRAME_WRITE_DATE_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	  = (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		  = IDNum;
	frame[FRAME_WRITE_DATE_YEAR_ADDRESS_HIHG] = (year >> 8);
	frame[FRAME_WRITE_DATE_YEAR_ADDRESS_LOW]  = year;
	frame[FRAME_WRITE_DATE_MONTH_ADDRESS]	  = month;
	frame[FRAME_WRITE_DATE_DAY_ADDRESS]	 	  = day;
	
	frame_complete_tx(frame, FRAME_WRITE_DATE_DATA_NUM);
	
}


/**
* @ Function Name : write_time
* @ Author        : hlb
* @ Brief         : 写时间
* @ Date          : 2017.07.19
* @ Input         : u16   IDNum   ID号
*                   u8    hour    时
					u8    minute  分
					u8    second  秒
* @ Modify        : ...
**/
void write_time(u16 IDNum, u8 hour, u8 minute, u8 second)
{
	u8 i;
	u8 frame[FRAME_WRITE_TIME_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS]	   		= FRAME_WRITE_TIME_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	= (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		= IDNum;
	frame[FRAME_WRITE_TIME_HOUR_ADDRESS]	= hour;
	frame[FRAME_WRITE_TIME_MINUTE_ADDRESS]	= minute;
	frame[FRAME_WRITE_TIME_SECOND_ADDRESS]	= second;
	
	frame_complete_tx(frame, FRAME_WRITE_TIME_DATA_NUM);
	
}


/**
* @ Function Name : write_page_num
* @ Author        : hlb
* @ Brief         : 写页码
* @ Date          : 2017.07.19
*                   u16   num     写入页码
* @ Modify        : ...
**/
void write_page_num(u16 num)
{
	u8 frame[FRAME_WRITE_PAGE_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS]			 = FRAME_WRITE_PAGE_DATA_HEAD;
	frame[FRAME_WRITE_PAGE_ARG_ADDRESS_HIGH] = (num >> 8);
	frame[FRAME_WRITE_PAGE_ARG_ADDRESS_LOW]	 = num;
	
	frame_complete_tx(frame, FRAME_WRITE_PAGE_DATA_NUM);
}

/**
* @ Function Name : write_control_state
* @ Author        : hlb
* @ Brief         : 写控件的状态
* @ Date          : 2017.07.19
* @ Input         : u16  IDNum   ID号
*                   u8   state   写入状态
* @ Modify        : ...
**/
void write_control_state(u16 IDNum, u8 state)
{
	u8 frame[FRAME_WRITE_STATE_SIZE];
	

	frame[FRAME_DATA_HEAD_ADDRESS]	   		= FRAME_WRITE_STATE_DATA_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   	= (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]		= IDNum;
	frame[FRAME_WRITE_STATE_ARG_ADDRESS]	= state;

	frame_complete_tx(frame,FRAME_WRITE_STATE_DATA_NUM);
}

/**
* @ Function Name : pull_control_data
* @ Author        : hlb
* @ Brief         : 发送下拉控件数据指令
* @ Date          : 2017.07.23
* @ Input         : u16  IDNum   ID号
					u8   type	 数据类型
* @ Modify        : ...
**/
void pull_control_data(u16 IDNum, u8 type)
{
	u8 frame[FRAME_PULL_CONTROL_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS]	   			= FRAME_PULL_CONTROL_HEAD;
	frame[FRAME_DATA_ID_ADDRESS_HIGH]   		= (IDNum >> 8);
	frame[FRAME_DATA_ID_ADDRESS_LOW]			= IDNum;
	frame[FRAME_DATA_TYPE_ADDRESS]				= type;
	frame[FRAME_PULL_CONTROL_VAL_ADDRESS]		= FRAME_PULL_CONTROL_VAL;
	
	frame_complete_tx(frame, FRAME_PULL_CONTROL_DATA_NUM);
	
}

/**
* @ Function Name : pull_page_num
* @ Author        : hlb
* @ Brief         : 发送下拉控件数据指令
* @ Date          : 2017.07.23
* @ Modify        : ...
**/
void pull_page_num(void)
{
	u8 frame[FRAME_PULL_PAGE_SIZE];
	
	frame[FRAME_DATA_HEAD_ADDRESS] = FRAME_PULL_PAGE_HEAD;
	frame[FRAME_PULL_PAGE_VAL_ADDRESS] = FRAME_PULL_PAGE_VAL;
	
	frame_complete_tx(frame, FRAME_PULL_PAGE_DATA_NUM);
	
}


/**
* @ Function Name : dma_usart_tx_handle
* @ Author        : hlb
* @ Brief         : dma串口发送处理函数
* @ Date          : 2017.07.19
* @ Modify        : ...
**/
void dma_usart_tx_handle(void)
{
	//如果DMA非忙
	if(!UsartTxBuffer.DmaBusy)
	{
		//激活的缓冲区非空
		if(UsartTxBuffer.Idx[UsartTxBuffer.PartAvailAction] != BUFFER_HEAD_DEFAULT)
		{
			//设置DMA传输对象
			dma_usart_tx(UsartTxBuffer.Buff[UsartTxBuffer.PartAvailAction], \
			UsartTxBuffer.Idx[UsartTxBuffer.PartAvailAction]);
			
			//恢复缓冲区索引
			UsartTxBuffer.Idx[UsartTxBuffer.PartAvailAction] = BUFFER_HEAD_DEFAULT;
			
			//激活另外一个缓冲，锁定当前缓冲
			UsartTxBuffer.PartAvailAction = 1 - UsartTxBuffer.PartAvailAction;
			
			//锁定DMA
			UsartTxBuffer.DmaBusy = true;
		}
	}
}

/**
* @ Function Name : usart_send_handle
* @ Author        : hlb
* @ Brief         : 串口数据发送线程
* @ Date          : 2017.07.19
* @ Modify        : ...
**/
void usart_send_handle(void)
{
	//检查DMA发送状态
	dma_usart_chek_tx_state();

	//发送处理线程
	dma_usart_tx_handle();
}

