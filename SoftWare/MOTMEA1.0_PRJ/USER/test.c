/*----------------------------------------------------------------------------------------------------------------
 * Copyright(c)
 * ---------------------------------------------------------------------------------------------------------------
 * File Name : test.c
 * Author    : hlb
 * Brief     : ²âÊÔÎÄ¼þ
 * Date      : 2017.07.21
 * ---------------------------------------------------------------------------------------------------------------
 * Modifier                                    Data                                             Brief
 * -------------------------------------------------------------------------------------------------------------*/
#include "sys_manager.h"
#include "IDNum.h"
#include "test.h"
#include "ff.h"
#include "sdio.h"
#include "message_send.h"
#include "message_analysis.h"
#include "exfuns.h"
#include "malloc.h"
#include "usart.h"

u32 res;
u32 a;
u8 result[1];
FIL fsrc,fdst;
u8 status[1];
u8 buffer[512];
u8 textFileBuffer[] = "xixixi";

void test(void)
{

	
//	pull_control_data(ID_PN_NUMBER_SELECT, FRAME_REC_DATA_SELECT_DEF);
//	while(1)
//	{
//		USART_SendData(USART1,'1');
//	}
//	static int i = 1;
//	if(i == 1)
//	{
//		status[0] = SD_Init();
//		exfuns_init();
//		while(status[0])
//		{
//			USART_SendData(USART1,status[0]);
//		}
//		exfuns_init();
//		result[0] = f_mount(fs[0],"0:",1);	
//		i = 0;
//	}
//	

//	
//	res = f_open(&fdst,"0:/demo2.txt  ", FA_CREATE_NEW | FA_WRITE);
//	if(res == FR_OK)
//	{
//		res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw);
//		f_close(&fdst);
//		res = f_open(&fdst, "0:/demo2.txt  ", FA_OPEN_EXISTING | FA_READ);
//		br = 1;
//		for(a = 0; a < 512; a++)
//		{
//			buffer[a] = 0;
//		}
//		while(1)
//		{

//			res = f_read(&fdst, buffer, sizeof(buffer), &br);
//			if(res || br == 0)
//			{
//				break;
//			}
//			
//		}
//		f_close(&fdst);	
//	}
//	if(res == FR_EXIST)
//	{
//		

//		res = f_open(&fdst, "0:/demo2.txt  ", FA_OPEN_EXISTING | FA_READ);
//		br = 1;
//		for(a = 0; a < 512; a++)
//		{
//			buffer[a] = 0;
//		}
//		while(1)
//		{

//			res = f_read(&fdst, buffer, sizeof(buffer), &br);
//			if(res || br == 0)
//			{
//				break;
//			}
//			
//		}
//		f_close(&fdst);	
//	}	
//	while(true)
//	{
//		for(a = 0; a < 6; i++)
//		{
//			USART_SendData(USART1,buffer[a]);
//		}
//	}

	
	
	

}
