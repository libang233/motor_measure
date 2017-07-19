#ifndef __MESSAGE_SEND_H_
#define __MESSAGE_SEND_H_

#include "config.h"

//最大发送字节数
#define FRAME_SEND_MAX_SIZE  100

typedef struct
{
	u8 frame[FRAME_SEND_MAX_SIZE];
	u8 Idx;
	
}send_Frame_TypeDef;

void write_control_state(u16 IDNum, u8 state);
void usart_send_handle(void);
void usart_tx(u8 *buff, u32 size);

#endif
