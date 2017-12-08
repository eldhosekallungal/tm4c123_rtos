/*
 * iiscKeypad.c
 *
 *  Created on: Dec 7, 2017
 *      Author: eldhose
 */

/* Output port PD [0 - 3] */
/* Input port PC [4 - 7] */

#include "peripheral_GPIO.h"
#include "peripheral_UART.h"
#include "sysio.h"
#include "dataTypes.h"

void delay_keypad(uint32_t dlx) {

	uint32_t dly;

	while(dlx-- > 0) {
		dly = 200;
		while(dly-- > 0);
	}
}
void boardInit() {

	bspGPIO_Config_t config;

	config.GPIO = GPIOD;
	config.PIN = (PD0 | PD1 | PD2 | PD3);
	config.DIR = GPIO_OUT((PD0 | PD1 | PD2 | PD3));

	BSP_gpioInitAndConfigure(config);

	config.GPIO = GPIOC;
	config.PIN = (PC4 | PC5 | PC6 | PC7);
	config.DIR = GPIO_IN((PC4 | PC5 | PC6 | PC7));
	config.weakPullUp = (PC4 | PC5 | PC6 | PC7);

	BSP_gpioInitAndConfigure(config);

	/* write high */
	BSP_gpioWrite(GPIOD, 0x0F);
}


void iiscKeypad() {

	uint8_t seq, sw, key, read;

	boardInit();

	while(1) {
		/* poll */
		for(seq = 0x01; seq < 0x10; seq = seq << 1) {
			BSP_gpioWrite(GPIOD, seq);
			sw = (((~BSP_gpioRead(GPIOC)) & 0xF0) >> 4);
			if(sw) {
				/* wait for switch to release */
				read = (((~BSP_gpioRead(GPIOC)) & 0xF0) >> 4);
				while((read & sw) != 0) {
					/* for debugging */
					read = (((~BSP_gpioRead(GPIOC)) & 0xF0) >> 4);
				}
				key = ((seq * 4) + sw);
				IO_printf("Pressed %d key\n", key);
			}

			delay_keypad(1000);
		}
	}
}
