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

extern semaphore_t *pSemGPIO;

void testTask();
void testTask2();

static char MEM[1024];

void iiscTestOperatingSystem() {


	bspGPIO_Config_t config;
	config.PIN = PF0 | PF1 | PF2 | PF3| PF4;
	config.DIR = GPIO_IN(PF0 | PF4) | GPIO_OUT(PF1 | PF2 | PF3);
	config.GPIO = GPIOF;
	config.weakPullUp = PF0 | PF4;
	osalMem_DynamicMemInit(MEM, 1024, 0);

	/* init the semaphore */
	uint8_t data = 0;
	pSemGPIO = OS_createSemaphote(SEM_GPIO, &data, 1);

	BSP_uartInitAndConfigure(UART0, 115200);
	BSP_gpioInitAndConfigure(config);
	BSP_gpioInterruptEnable(GPIOF, PF0|PF4, GPIO_INTR_FALLING);

	OS_sTaskHandle_t *pHandleTask1 = NULL, *pHandleTask2 = NULL;
	OS_createTask(pHandleTask1, testTask, "none", 100, 1, NULL);
	OS_createTask(pHandleTask2, testTask2, "none", 100, 1, NULL);
	OS_scheduleTask();

	while(1);
}

void testTask() {

	while(1) {
		OS_semaphoteWait(pSemGPIO);
		IO_printf("Task 1\n");
	}
}

void testTask2() {
	while(1) {
		OS_semaphoteWait(pSemGPIO);
		IO_printf("Task 2\n");
	}
}

