#ifndef __MESSAGE_SEND_H_
#define __MESSAGE_SEND_H_

#include "config.h"

void usart_send_handle(void);
void dma_usart_send(u8 res[], u8 i);

#endif
