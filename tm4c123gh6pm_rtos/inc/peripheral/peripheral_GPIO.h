/*
 * peripheral_GPIO.h
 *
 *  Created on: Sep 6, 2017
 *      Author: eldhose
 */

#ifndef PERIPHERAL_GPIO_H_
#define PERIPHERAL_GPIO_H_

#include "dataTypes.h"
#include "peripheral.h"

#define APB    0
#define AHB    1
#define BUS_APERTURE    APB

#if(BUS_APERTURE == APB)
#define GPIOA    0x40004000
#define GPIOB    0x40005000
#define GPIOC    0x40006000
#define GPIOD    0x40007000
#define GPIOE    0x40024000
#define GPIOF    0x40025000
#elif(BUS_APERTURE == AHB)
#define GPIOA    0x40058000
#define GPIOB    0x40059000
#define GPIOC    0x4005A000
#define GPIOD    0x4005B000
#define GPIOE    0x4005C000
#define GPIOF    0x4005D000
#endif

#define GPIODATA	0x3FC	/* don't know how this came */
#define GPIODIR		0x400
#define GPIOIS		0x404
#define GPIOIBE		0x408
#define GPIOIEV		0x40C
#define GPIOIM		0x410
#define GPIORIS		0x414
#define GPIOMIS		0x418
#define GPIOICR		0x41C
#define GPIOAFSEL   0x420
#define GPIODR2R	0x500
#define GPIODR4R	0x504
#define GPIODR8R	0x508
#define GPIOODR		0x50C
#define GPIOPUR		0x510
#define GPIOPDR		0x514
#define GPIOSLR		0x518
#define GPIODEN     0x51C
#define GPIOLOCK	0x520
#define GPIOCR		0x524
#define GPIOAMSEL	0x528
#define GPIOPCTL    0x52C
#define GPIOADCCTL	0x530
#define GPIODMACTL	0x534

/* gpio lock magic number */
#define GPIO_LOCK_VAL	0x4C4F434B
/* Port Definition for DEN and AFSEL for PORT */
#define PA0    0x01
#define PA1    0x02
#define PA2    0x04
#define PA3    0x08
#define PA4    0x10
#define PA5    0x20
#define PA6    0x40
#define PA7    0x80

#define PB0    0x01
#define PB1    0x02
#define PB2    0x04
#define PB3    0x08
#define PB4    0x10
#define PB5    0x20
#define PB6    0x40
#define PB7    0x80

#define PC4    0x10
#define PC5    0x20
#define PC6    0x40
#define PC7    0x80

#define PD0    0x01
#define PD1    0x02
#define PD2    0x04
#define PD3    0x08
#define PD4    0x10
#define PD5    0x20
#define PD6    0x40
#define PD7    0x80

#define PE0    0x01
#define PE1    0x02
#define PE2    0x04
#define PE3    0x08
#define PE4    0x10
#define PE5    0x20

#define PF0    0x01
#define PF1    0x02
#define PF2    0x04
#define PF3    0x08
#define PF4    0x10

/* Interrupt configurations */
#define GPIO_INTR_LEVEL		0
#define GPIO_INTR_RAISING	1
#define GPIO_INTR_FALLING	2
#define GPIO_INTR_BOTH_EDGE	3

/* GPIO Configurations */
#define U0RX    0x00000001
#define U0TX    0x00000010

#define GPIO_OUT(PX)	(PX)
#define GPIO_IN(PX)		(0)

#define GPIO_SET_INTER_MASK(PX, MASK)
/* FUNCTIONS */
#define GPIO_ENABLE_ALT_FUNC(GPIO, PXX)    	_REG(GPIO + GPIOAFSEL) |= (PXX)
#define GPIO_DISABLE_ALT_FUNC(GPIO, PXX)    _REG(GPIO + GPIOAFSEL) &= ~(PXX)
#define GPIO_ENABLE_DIG_IO(GPIO, PXX)      	_REG(GPIO + GPIODEN) |= (PXX)
#define GPIO_CONFIG(GPIO, PXX)             	_REG(GPIO + GPIOPCTL)  = (_REG(GPIO + GPIOPCTL)& 0xFFFFFF00) + (PXX)
#define GPIO_ANALOG_DISABLE(GPIO, PXX)     	_REG(GPIO + GPIOAMSEL) &= ~(PXX)
#define GPIO_ANALOG_ENABLE(GPIO, PXX)		_REG(GPIO + GPIOAMSEL) |= (PXX)
#define GPIO_SET_DIR(GPIO, PXX, DIR)		{\
											if((DIR) == 1) \
												_REG(GPIO + GPIODIR) |= (PXX); \
											else if((DIR) == 0) \
												_REG(GPIO + GPIODIR) &= ~(PXX); \
											}

#define GPIO_LOCK(GPIO)		_REG((GPIO + GPIOLOCK)) = ~(GPIO_LOCK_VAL)
#define GPIO_UNLOCK(GPIO)	_REG((GPIO + GPIOLOCK)) = (GPIO_LOCK_VAL)
#define GPIO_ISLOCKED(GPIO)	_REG((GPIO + GPIOLOCK))

#define GPIO_COMMIT_SET(GPIO, PX)	_REG((GPIO + GPIOCR)) |= (PX)
#define GPIO_COMMIT_CLEAR(GPIO, PX)	_REG((GPIO + GPIOCR)) &= ~(PX)

typedef struct {
	/* this is the GPIO user configuration structure */
	_ADDR GPIO;	/* GPIO Address */
	/* sleep and deep sleep modes are not included */
	uint8_t PIN;
	uint8_t DIR;
	uint8_t weakPullUp;
	uint8_t slewRateControl;
	/* slew rate control is enable only if drive strength is 8mA */
	/* set the alternate function in the GPIO */
	uint8_t driveStrength;	/* 0 - 2mA, 1 - 4mA, 3 - 8mA */
	/* GPIO commit register and GPIO lock register must be 1 for writing following data */
	/* 1. weak pull up, this will automatically set pull down select register  */

	/* interrupt mask */
}bspGPIO_Config_t;

bool BSP_gpioGetPortNumber(_ADDR GPIO, uint8_t *port);
bool BSP_gpioInitAndConfigure(bspGPIO_Config_t config);
void BSP_gpioInterruptEnable(_ADDR GPIO, uint32_t pin, uint8_t INT_MASK);
void BSP_gpioInterruptClear(_ADDR GPIO, uint8_t pin);
bool BSP_gpioInterruptStatus(_ADDR GPIO, uint8_t pin);
uint8_t BSP_gpioRead(_ADDR GPIO);
void BSP_gpioWrite(_ADDR GPIO, uint8_t data);
void BSP_gpioInterruptHandler_F();
#endif /* PERIPHERAL_GPIO_H_ */
