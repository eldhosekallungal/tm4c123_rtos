/*
 * peripheral_UART.c
 *
 *  Created on: Sep 4, 2017
 *      Author: eldhose
 */

#include "peripheral_UART.h"
#include "peripheral_GPIO.h"
#include "sysCtrl.h"
#include "dataTypes.h"

char BSP_uartData;
char BSP_uartBuffer[100];

void BSP_uartSetBoadrate(_ADDR BASE, uint32_t boadRate) {

	uint32_t BRDIF;
	uint32_t integer;
	uint32_t fraction;

	if((_REG(BASE + UARTCTL) & (1 << 5))) {
		/* clock divisor is 8*/
		BRDIF = ((UART_SYSCLOCK * 100) / (8 * (uint32_t)(boadRate / 100)));
	}
	else {
		/* clock divisor is 16 */
		BRDIF = ((UART_SYSCLOCK * 100) / (16 * (uint32_t)(boadRate / 100)));
	}
	integer = (uint16_t)(BRDIF / 10000);
	fraction = (BRDIF - (integer * 10000));
	// IBRD = int(SYSTEM_CORE_CLOCK / (16 * 115,200)) = int(8.680)
	_REG(BASE + UARTIBRD) = (uint16_t)integer;
	// FBRD = round(BRDF * 64 + 0.5) = 33
	_REG(BASE + UARTFBRD) = (uint8_t)(((fraction * 64) + 5000) / 10000);

}
void BSP_uartInitAndConfigure(_ADDR BASE, uint32_t boadRate) {

	uint8_t portNum;

	portNum = (BASE - UART0) >> 3;

	if(portNum > 7) {
		return;
	}
	//
	/* ACTIVATE UART Port */
	SYSCTRL_UART_ENABLE(portNum);
	/* Activate GPIO */
	SYSCTRL_GPIO_ENABLE(portNum);
	/* Wait for GPIO to awake */
	while(!SYSCTRL_GPIO_READY(portNum));
	/* Dsable UART port */
	UART_DISABLE(BASE);
	/* set baud rate function */
	BSP_uartSetBoadrate(BASE, boadRate);
	/* set the line control register */
	UART_SET_LCRH(BASE, (UARTLCRH_WL_8|UARTLCRH_FEN));
	/* Enable UART */
	UART_ENABLE(BASE);
	/* Enable the Alternate function for UART0 */
	GPIO_ENABLE_ALT_FUNC(GPIOA, (PA0|PA1));
	/* Enable digital function in port A */
	GPIO_ENABLE_DIG_IO(GPIOA, PA0|PA1);
	/* Configure the GPIO control register for UART */
	GPIO_CONFIG(GPIOA, U0RX|U0TX);
	/* Disable analog functionality in PA0 and PA1 */
	GPIO_ANALOG_DISABLE(GPIOA, PA0|PA1);

}

void BSP_uartEnableInterrupt(_ADDR BASE) {
// TODO: NVIC switch case
	/* Clear all interrupt */
	uint32_t test = _REG(BASE + UARTIM);
//	_REG(BASE + UARTICR) = (0xFFFFFFFF);
	test = _REG(BASE + UARTIM);
	/* Enable NVIC interrupt mask */
	_REG(PERIPHERAL_BASE_ADDR + EN0) |= (1 << 5);
	/* enable uart interrupt */
	_REG(BASE + UARTIM) = ((1 << 12) || (1 << 4));
	test = _REG(BASE + UARTIM);
}

void BSP_uartDisableInterrupt(_ADDR BASE) {
	// TODO: NVIC switch case
	/* disable uart interrupt */
	_REG(BASE + UARTIM) = 0x00000000;
	/* Disable NVIC interrupt mask */
	_REG(PERIPHERAL_BASE_ADDR + EN0) &= ~(1 << 5);
}

void BSP_uartInterruptClear(_ADDR BASE) {
	_REG(BASE + UARTICR) = (0xFFFFFFFF);
}
/*
 *
 *
 */

void BSP_uartTX(_ADDR BASE, char in) {

	UART_ENABLE(BASE);

	while((_REG(BASE + UARTFR)&0x00000020) != 0);

	_REG(BASE + UARTDR) = in;

}

char BSP_uartRX(_ADDR BASE) {

	UART_ENABLE(BASE);

	while((_REG(BASE + UARTFR)&0x00000010) != 0);
	return (_REG(BASE + UARTDR)&0xFF);
}

char BSP_uartGetCh(_ADDR BASE) {
	return (_REG(BASE + UARTDR)&0xFF);
}
void BSP_UART0_Handler(void) {
	/* Clear the interrupt */
	uint8_t i_cnt;
	_REG(UART0 + UARTICR) &= (0xFFFFFFFF);
	while((_REG(UART0 + UARTFR)&0x00000010) != 0);
	BSP_uartData = (_REG(UART0 + UARTDR) & 0xFF);

	i_cnt = 0;
	while((BSP_uartData != '\n')&&(BSP_uartData != '\r')) {
		BSP_uartBuffer[i_cnt] = BSP_uartData;
		i_cnt++;
	}
	BSP_uartBuffer[i_cnt] = BSP_uartData;
}

