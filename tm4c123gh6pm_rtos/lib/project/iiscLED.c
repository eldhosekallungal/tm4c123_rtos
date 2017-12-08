/*
 * iiscLED.c
 *
 *  Created on: Dec 4, 2017
 *      Author: eldhose
 */
#include "peripheral_GPIO.h"
#include "sysmem.h"
#include "sysio.h"

void delay() {
	uint32_t delay;

	for(delay = 0; delay < 200000; delay++);
	for(delay = 0; delay < 200000; delay++);
}
void iiscLED() {

	uint8_t count = 0;
	bspGPIO_Config_t config;
	config.GPIO = GPIOD;
	config.PIN = PD0 | PD1 | PD2 | PD3;
	config.DIR = GPIO_OUT(PD0 | PD1 | PD2 | PD3);

	BSP_gpioInitAndConfigure(config);

	while(1) {

		BSP_gpioWrite(GPIOD, count);
		delay();
		count++;
		if(count >= 10)
			count = 0;
	}
}
