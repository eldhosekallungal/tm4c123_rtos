/*
 * testMain.c
 *
 *  Created on: Sep 2, 2017
 *      Author: eldhose
 */

/* projects */
#include "peripheral_GPIO.h"
#include "peripheral_UART.h"
#include "sysCtrl.h"
#include "sysos.h"
#include "sysio.h"
#include "sysmem.h"


semaphore_t *pSemGPIO = NULL;
static char mem[1024 * 10];

void iiscLEDonSwitch() {

	//iiscTestBlinky();
	//iiscProjectLinkedList_main()

	uint8_t color = PF1 & 0xFF;

	bspGPIO_Config_t config;
	config.PIN = PF0 | PF1 | PF2 | PF3| PF4;
	config.DIR = GPIO_IN(PF0 | PF4) | GPIO_OUT(PF1 | PF2 | PF3);
	config.GPIO = GPIOF;
	config.weakPullUp = PF0 | PF4;
	osalMem_DynamicMemInit(mem, 10*1024, 0);

	/* init the semaphore */
	uint8_t data = 0;
	pSemGPIO = OS_createSemaphote(SEM_GPIO, &data, 1);

	BSP_uartInitAndConfigure(UART0, 115200);
	BSP_gpioInitAndConfigure(config);
	BSP_gpioInterruptEnable(GPIOF, PF0|PF4, GPIO_INTR_FALLING);

	while(1) {
		OS_semaphoteWait(pSemGPIO);

		uint8_t IM = 0;
		IM = *(((uint8_t *)pSemGPIO->data)) & 0xFF;

		if(IM & PF0) {// switch PF0 is pressed
			IO_printf("SW0 is pressed\n");
			BSP_gpioWrite(GPIOF, 0x00);
		}
		else if(IM & PF4) {
			IO_printf("SW1 is pressed\n");
			BSP_gpioWrite(GPIOF, color);
			color = color << 1;
			if(color > (PF3 & 0xFF))
				color = PF1 & 0xFF;
		}
	}
}
