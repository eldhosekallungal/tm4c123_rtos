/*
 * peripheral_GPIO.c
 *
 *  Created on: Sep 6, 2017
 *      Author: eldhose
 */

#include "peripheral_GPIO.h"
#include "sysCtrl.h"
#include "sysos.h"

extern semaphore_t *pSemGPIO;

#undef GPIO_SET_DIR
#define GPIO_SET_DIR(GPIO, PIN, DIR)	_REG(GPIO + GPIODIR) = ((_REG(GPIO + GPIODIR) & ~(PIN)) | ((PIN)&(DIR)))

bool BSP_gpioGetPortNumber(_ADDR GPIO, uint8_t *port) {

	switch(GPIO) {
	case GPIOA:
		*port = 0;
		break;
	case GPIOB:
		*port = 1;
		break;
	case GPIOC:
		*port = 2;
		break;
	case GPIOD:
		*port = 3;
		break;
	case GPIOE:
		*port = 4;
		break;
	case GPIOF:
		*port = 5;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
/*
 * Example usage
 * bspGPIO_Config_t config;
 * config.GPIO = GPIOA;
 * config.PIN = (PA2 | PA3 | PA4);
 * config.DIR = (GPIO_OUT(PA2 | PA4) | GPIO_IN(PA3)); // PA2, PA4 - out, PA3 - In
 * Others are not required right now
 * BSP_gpioInitAndConfigure(config)
*/
bool BSP_gpioInitAndConfigure(volatile bspGPIO_Config_t config) {
	/* Ref: page number 656 tm4c123gh6pm-Datasheet */
	uint8_t gpio;

	OSAL_ASSERT(BSP_gpioGetPortNumber(config.GPIO, &gpio), OSAL_GPIO_UNKNOWN_PORT);
	/* Enable run mode gate clock control */
	SYSCTRL_RCGC2_ENABLE(gpio);
	/* Enable GPIO*/
	SYSCTRL_GPIO_ENABLE(gpio);
	/* unlock GPIO if it is locked */
	GPIO_UNLOCK(config.GPIO);
	/* set the corresponding commit register */
	GPIO_COMMIT_SET(config.GPIO, config.PIN);
	/* Set the digital configurations */
	/* set the direction */
	GPIO_SET_DIR(config.GPIO, config.PIN, config.DIR);
	GPIO_DISABLE_ALT_FUNC(config.GPIO, config.PIN);
	/* not setting the driver strength right now */
	/* set weak pull up resister */
	_REG(config.GPIO + GPIOPUR) |= (config.weakPullUp);
	GPIO_ENABLE_DIG_IO(config.GPIO, config.PIN);
	GPIO_ANALOG_DISABLE(config.GPIO, config.PIN);
	/* Lock the GPIO */
	GPIO_LOCK(config.GPIO);

	return TRUE;
}

void BSP_gpioInterruptEnable(_ADDR GPIO, uint32_t pin, uint8_t INT_MASK) {

	uint8_t gpio;
	OSAL_ASSERT(BSP_gpioGetPortNumber(GPIO, &gpio), OSAL_GPIO_UNKNOWN_PORT);

	/* unlock GPIO */
	GPIO_UNLOCK(GPIO);
	GPIO_COMMIT_SET(GPIO, pin);

	switch(INT_MASK) {
	case GPIO_INTR_LEVEL:
		/* set the interrupt sense to level triggered */
		_REG(GPIO + GPIOIS) |= (pin);
		break;
	case GPIO_INTR_RAISING:
		/* set the interupt sense to edge */
		_REG(GPIO + GPIOIS) &= ~(pin);
		/* clear the IBE register */
		_REG(GPIO + GPIOIBE) &= ~(pin);
		/* set the IEV bit for raising edge */
		_REG(GPIO + GPIOIEV) |= (pin);
		break;
	case GPIO_INTR_FALLING:
		/* set the interupt sense to edge */
		_REG(GPIO + GPIOIS) &= ~(pin);
		/* clear the IBE register */
		_REG(GPIO + GPIOIBE) &= ~(pin);
		/* set the IEV bit for raising edge */
		_REG(GPIO + GPIOIEV) &= ~(pin);
		break;
	case GPIO_INTR_BOTH_EDGE:
		/* set the interupt sense to edge */
		_REG(GPIO + GPIOIS) &= ~(pin);
		/* set the IBE register */
		_REG(GPIO + GPIOIBE) |= (pin);
		break;
	default:
		break;
	}

	if(GPIO == GPIOF)
		_REG(PERIPHERAL_BASE_ADDR + EN0) |= (1 << 30);
	else
		_REG(PERIPHERAL_BASE_ADDR + EN0) |= (1 << gpio);

	/* Enable the interrupt */
	_REG(GPIO + GPIOIM) |= (pin);
	/* Lock the GPIO */
	GPIO_LOCK(GPIO);
}

bool BSP_gpioInterruptStatus(_ADDR GPIO, uint8_t pin) {

	return ((_REG(GPIO + GPIORIS) & (pin)) != FALSE);
}

void BSP_gpioInterruptClear(_ADDR GPIO, uint8_t pin) {
	_REG(GPIO + GPIOICR) |= (pin);
}

void BSP_gpioWrite(_ADDR GPIO, uint8_t data) {
	_REG(GPIO + GPIODATA) = data & 0xFF;
}

uint8_t BSP_gpioRead(_ADDR GPIO) {
	return (_REG(GPIO + GPIODATA) & 0xFF);
}

void BSP_gpioPinWrite(_ADDR GPIO, uint8_t pin, uint8_t data) {
	/* This uses the bit banding */

}

void BSP_GPIOF_Handler() {
	/* read the raw interrupt status */
	(*(uint8_t *)pSemGPIO->data) = (_REG(GPIOF + GPIORIS) & 0xFF);
	/* clear interrupt */
	_REG(GPIOF + GPIOICR) = 0xFF;
	/* signal the semaphore */
	OS_semaphoreSignal(pSemGPIO);
}
