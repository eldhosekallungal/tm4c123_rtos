/*
 * peripheral_UART.h
 *
 *  Created on: Sep 2, 2017
 *      Author: eldhose
 */

#ifndef PERIPHERAL_UART_H_
#define PERIPHERAL_UART_H_

#include "dataTypes.h"

#define UART_SYSCLOCK    16000000

/* UART base address */
#define UART0 0x4000C000
#define UART1 0x4000D000
#define UART2 0x4000E000
#define UART3 0x4000F000
#define UART4 0x40010000
#define UART5 0x40011000
#define UART6 0x40012000
#define UART7 0x40013000

/* UART registers in offset as per datasheet */
#define UARTDR     0x000
#define UARTRSR    0x004
#define UARTECR    0x008	/* TODO doubt on this address */
#define UARTFR     0x018
//
#define UARTIBRD   0x024
#define UARTFBRD   0x028
#define UARTLCRH   0x02C
#define UARTCTL    0x030
#define UARTIFLS   0x034
#define UARTIM     0x038
#define UARTRIS    0x03C
#define UARTMIS    0x040
#define UARTICR    0x044
//
//
//
//
#define UARTCC    0xFC8
// Skipped

/* UART Line control register values */
#define UARTLCRH_SPS    0x00000080
#define UARTLCRH_WL_5   0x00000000
#define UARTLCRH_WL_6   0x00000020
#define UARTLCRH_WL_7   0x00000040
#define UARTLCRH_WL_8   0x00000060
#define UARTLCRH_FEN    0x00000010
#define UARTLCRH_STP2   0x00000008
#define UARTLCRH_EPS    0x00000004
#define UARTLCRH_PEN    0x00000002
#define UARTLCRH_BRK    0x00000001

#define UARTFR_TXFF     0x00000020  // UART Transmit FIFO Full
#define UARTFR_RXFE     0x00000010  // UART Receive FIFO Empty
#define UARTCTL_EN      0x00000001  // UART Enable

#define UART_ENABLE(BASE)    _REG(BASE + UARTCTL) |= (UARTCTL_EN)
#define UART_DISABLE(BASE)   _REG(BASE + UARTCTL) &= ~(UARTCTL_EN)
#define UART_SET_LCRH(BASE, VAL)    _REG(BASE + UARTLCRH) = (VAL)
#define UART_CLEAR(BASE)    _REG(BASE + UARTDR) = 0x00000000

#define UART_TX(BASE, C_IN)    {\
	                               UART_ENABLE(BASE);\
	                               while((_REG(BASE + UARTFR)&0x00000020) != 0);\
	                               _REG(BASE + UARTDR) = C_IN;\
	                           }

void BSP_uartInitAndConfigure(_ADDR BASE, uint32_t boadRate);
void BSP_uartEnableInterrupt(_ADDR BASE);
void BSP_uartDisableInterrupt(_ADDR BASE);
void BSP_uartInterruptClear(_ADDR BASE);
void BSP_uartSetBoadrate(_ADDR BASE, uint32_t boadRate);
char BSP_uartRX(_ADDR BASE);
char BSP_uartGetCh(_ADDR BASE);
void BSP_uartTX(_ADDR BASE, char in);
void BSP_uartIntrHandler(void);

#endif /* PERIPHERAL_UART_H_ */
