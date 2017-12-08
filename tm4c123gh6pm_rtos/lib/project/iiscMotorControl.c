/*
 * iiscMotorControl.c
 *
 *  Created on: Dec 4, 2017
 *      Author: eldhose
 */

/* DC motor is connected to two ports
 * Two switched are provided for state and direction
 * Speed of the motor is controlled using a PWM module, which is implemented using GPIO
 * Speed is inputed from UART
 */


#include "peripheral_GPIO.h"
#include "peripheral_UART.h"
#include "stringOps.h"
#include "sysmem.h"
#include "sysio.h"

#define PWM_FREQ	100

static bool onOff = 0;
static bool dir = 0;
static int speed = 0;

void swIRQHandler() {
	/* came here on pressing the switch */
	if(BSP_gpioInterruptStatus(GPIOF, PF0) == TRUE) {
		onOff = ~onOff;
	}
	if(BSP_gpioInterruptStatus(GPIOF, PF4) == TRUE) {
		dir = ~dir;
	}

	/* change cond and direction */
	BSP_gpioWrite(GPIOD, ((PD0 & onOff) | (PD1 & (dir << 1))));
	BSP_gpioInterruptClear(GPIOF, (PF0 | PF4));

}

void speedIRQHandler() {

	char buffer[100];
	int cnt = 0;
	int prevSpeed = speed;

	BSP_uartDisableInterrupt(UART0);

	buffer[0] = BSP_uartGetCh(UART0);

	while((buffer[cnt] != '\n') && (buffer[cnt] != '\r')) {
		cnt++;
		buffer[cnt] = BSP_uartRX(UART0);
	}
	buffer[cnt] = '\0';

	if(atoi(buffer, &speed) == -1) {
		IO_printf("No a valid input\n");
	}
	else {
		if((speed < 0) || (speed > 100)) {
			IO_printf("Speed must be between 0 and 100\n");
			speed = prevSpeed;
		}
	}

	BSP_uartInterruptClear(UART0);

	BSP_uartEnableInterrupt(UART0);
}

void delay_motor(uint8_t sp) {

	uint8_t i;
	for(i = 0; i < sp; i++);
}
void iiscMotorControl() {
	char buffer[100];
	int cnt = 0;
	int prevSpeed;

	/* Initialize GPIO */
	bspGPIO_Config_t config;
	config.GPIO = GPIOF;
	config.PIN = PF0 | PF4;
	config.DIR = GPIO_IN(PFO | PF4);
	config.weakPullUp = PF0 | PF4;

	BSP_gpioInitAndConfigure(config);

	config.GPIO = GPIOD;
	config.PIN = PD0 | PD1;
	config.DIR = GPIO_OUT(PD0 | PD1);

	BSP_gpioInitAndConfigure(config);

	BSP_gpioWrite(GPIOD, 0x00);	// turn off the motor

	// turn on GPIO interrupt

	BSP_gpioInterruptEnable(GPIOF, (PF0 | PF4), GPIO_INTR_FALLING);

	BSP_uartInitAndConfigure(UART0, 115200);

	// wait for interrupt
	while(1) {
		cnt = 0;
		while((_REG(UART0 + UARTFR)&0x00000010) != 0) {
			/* generate PWM */
			if(onOff != FALSE) {
				BSP_gpioWrite(GPIOD, ((dir << 1) | 1));
				delay_motor(speed);
				BSP_gpioWrite(GPIOD, ((dir << 1) | 0));
				delay_motor(100-speed);
			}
		}
		buffer[cnt] = BSP_uartGetCh(UART0);
		while((buffer[cnt] != '\n') && (buffer[cnt] != '\r')) {
			cnt++;
			buffer[cnt] = BSP_uartRX(UART0);
		}
		buffer[cnt] = '\0';

		if(atoi(buffer, &speed) == -1) {
			IO_printf("No a valid input\n");
		}
		else {
			if(onOff == FALSE) {
				IO_printf("Motor is off\n");
				speed = prevSpeed;
			}
			else if((speed < 0) || (speed > 100)) {
				IO_printf("Speed must be between 0 and 100\n");
				speed = prevSpeed;
			}
			else {
				IO_printf("Current speed %d , new speed %d\n", prevSpeed, speed);
				prevSpeed = speed;
			}
		}
	}
}
