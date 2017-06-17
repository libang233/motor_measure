/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : newtype.c
 * Author    : hlb
 * Brief     : 数据结构初始化
 * Date      : 2017.05.30
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
 
#include "sys_manager.h"
#include "newtype.h"
#include "data_init.h"
#include "message_analysis.h"

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
    ParserMoniter.IsFrame_Buffers_Empty = true;
    ParserMoniter.IsFrmae_Buffers_Full = false;
	
	
}

/**
* @ Function Name : data_init
* @ Author        : hlb
* @ Brief         : 数据初始化
* @ Date          : 2017.05.30
* @ Modify        : ...
 **/
void data_init(void)
{
	//初始化全局标志位
	myEvnt.ALL = 0;
	
	//解析器初始化
	parser_init();
	
	//测试数据初始化；
	memset(&testingInformation, 0, sizeof(testingInformation));
	
}
