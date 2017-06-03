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


/**
* @ Function Name : Serial_analysis
* @ Author        : hlb
* @ Brief         : 串口数据的分析
* @ Date          : 2017.05.31
* @ Modify        : ...
**/
void Serial_analysis(void)
{
	u16 ID;
	u8 flag;
	u8 lenAll;
	u8 lenData;
	if(USART_RX_STA & 0x8000)
	{
		flag = 0;
		lenAll = USART_RX_STA & 0x3fff;				//得到数据长度
		if(USART_RX_BUF[flag] == 0x55)
		{
			flag++;
			lenData = USART_RX_BUF[flag++] - 1;
			flag ++;
			ID = USART_RX_BUF[flag++] * 4096;
			ID = ID + USART_RX_BUF[flag++];
			
			
		}
		USART_RX_STA=0;
		
	}

}
