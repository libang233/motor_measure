#ifndef __MESSAGE_SEND_H_
#define __MESSAGE_SEND_H_

#include "config.h"


void write_int(u16 IDNum, u32 val);
void write_char(u16 IDNum, u8* val, u8 valSize);
void write_bitmap(u16 IDNum, u8 status);
void write_select_val(u16 IDNum, u8 Idx, u8 valSize, u8* val);
void write_select_position(u16 IDNum, u8 pos);
void write_bar(u16 IDNum, u16 val);
void write_date(u16 IDNum, u16 year, u8 month, u8 day);
void write_time(u16 IDNum, u8 hour, u8 minute, u8 second);
void write_page_num(u16 num);
void write_control_state(u16 IDNum, u8 state);

void pull_control_data(u16 IDNum, u8 type);

void usart_send_handle(void);
void usart_tx(u8 *buff, u32 size);

#endif
