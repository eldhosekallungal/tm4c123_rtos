/*
 * iiscTestBlinky.c
 *
 *  Created on: Dec 2, 2017
 *      Author: eldhose
 */

#include "peripheral_GPIO.h"
#include "peripheral_UART.h"
#include "sysCtrl.h"
#include "sysio.h"
#include "dataTypes.h"
#include "string.h"

void iiscTestBlinky() {
	char in[100];
	bspGPIO_Config_t config;

	config.GPIO = GPIOF;
	config.PIN = PF1 | PF2 | PF3;
	config.DIR = GPIO_OUT(PF1 | PF2 | PF3);
	config.driveStrength = 0;
	config.slewRateControl = 0;
	config.weakPullUp = 0;

	BSP_gpioInitAndConfigure(config);
	BSP_uartInitAndConfigure(UART0, 115200);
	while(1) {
		IO_scanf("Type the colour [R G B] or off %s\n", &in);
		if(strcmp(in, "r") == 0)
			BSP_gpioWrite(GPIOF, PF1);
		else if(strcmp(in, "g") == 0)
			BSP_gpioWrite(GPIOF, PF3);
		else if(strcmp(in, "b") == 0)
			BSP_gpioWrite(GPIOF, PF2);
		else if(strcmp(in, "off") == 0)
			BSP_gpioWrite(GPIOF, 0);
		else
			IO_printf("unknown input\n");
	}
}
