#ifndef __DMA_H_
#define __DMA_H_

#include "stm32f10x_conf.h"
#include "config.h"

/*------------------------------------------------------USART1----------------------------------------------------*/

#define USART1_TX_DMA_CHANNEL      DMA1_Channel4
#define USART1_RX_DMA_CHANNEL      DMA1_Channel5

#define USART1_TX_DMA_PRIORITY     DMA_Priority_Medium
#define USART1_RX_DMA_PRIORITY     DMA_Priority_VeryHigh


void dma_usart_check_tx_state(void);
void dma_usart_tx_restart(u8 *buff, u32 size);
void dma_usart_tx(u8 *buff, u32 size);
void usart1_dma_init(void);

#endif
