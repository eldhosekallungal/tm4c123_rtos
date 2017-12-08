/*
 * logging.c
 *
 *  Created on: Dec 2, 2017
 *      Author: eldhose
 */

#include "peripheral_UART.h"
#include "dataTypes.h"
#include "stringOps.h"
#include "stdarg.h"

void IO_printf(char *fmt, ...) {

	char ch, ch_tmp, *str_tmp, buffer[512];
	int int_tmp;
	va_list arg;

	va_start(arg, fmt);

	while((ch = *fmt++) != '\0') {
		if(ch == '%') {
			switch(ch = *fmt++) {
			case '%':
			{
				BSP_uartTX(UART0, '%');
			}
				break;
			case 'c':
			{
				ch_tmp = va_arg(arg, int);
				BSP_uartTX(UART0, ch_tmp);
			}
				break;
			case 's':
			{
				str_tmp = va_arg(arg, char *);
				while(*str_tmp != '\0') {
					BSP_uartTX(UART0, *str_tmp);
					str_tmp++;
				}
			}
				break;
			case 'd':
			{
				int_tmp = va_arg(arg, int);
				itoa(int_tmp, buffer, 10);
				str_tmp = buffer;
				while(*str_tmp != '\0') {
					BSP_uartTX(UART0, *str_tmp);
					str_tmp++;
				}
			}
				break;
			case 'x':
				break;
			default:
				break;
			}
		}
		else if(ch == '\n') {
			BSP_uartTX(UART0, ch);
			BSP_uartTX(UART0, '\r');
		}
		else {
			BSP_uartTX(UART0, ch);
		}
	}
	va_end(arg);
}

void IO_scanf(char *fmt, ...) {
	char ch, *pCh, *pStr, ch_tmp, buffer[512];
	int *pInt;
	unsigned int i_cnt;
	va_list arg;

	va_start(arg, fmt);

	while((ch = *fmt++) != '\0') {
		if(ch == '%') {
			switch(ch = *fmt++) {
			case '%':
			{
				BSP_uartTX(UART0, '%');
			}
				break;
			case 'c':
			{
				pCh = va_arg(arg, char *);
				*pCh = BSP_uartRX(UART0);
			}
				break;
			case 's':
			{
				pStr = va_arg(arg, char *);
				ch_tmp = BSP_uartRX(UART0);
				while((ch_tmp != '\n') && (ch_tmp != '\r')) {
					*pStr = ch_tmp;
					pStr++;
					ch_tmp = BSP_uartRX(UART0);
				}
				*pStr = '\0';
			}
				break;
			case 'd':
			{
				pInt = va_arg(arg, int *);
				ch_tmp = BSP_uartRX(UART0);
				i_cnt = 0;
				while((ch_tmp != '\n') && (ch_tmp != '\r')) {
					buffer[i_cnt] = ch_tmp;
					i_cnt++;
					ch_tmp = BSP_uartRX(UART0);
				}
				buffer[i_cnt] = '\0';
				atoi(buffer, pInt);
			}
				break;
			case 'x':
				break;
			default:
				break;
			}
		}
		else if(ch == '\n') {
			BSP_uartTX(UART0, ch);
			BSP_uartTX(UART0, '\r');
		}
		else {
			BSP_uartTX(UART0, ch);
		}
	}
	va_end(arg);
}
