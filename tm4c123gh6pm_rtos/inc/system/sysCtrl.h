/*
 * system.h
 *
 *  Created on: Sep 4, 2017
 *      Author: eldhose
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "dataTypes.h"

#define SYSTEM_CORE_CLOCK    16000000 // 16MHz
#define SYSCTRL    0x400FE000

#define DID0       0x000
#define DID1       0x004
#define PBORCTL    0x030
#define RIS        0x050
#define IMC        0x054

/* Register 9 GPIO high performance bus control */
#define GPIOHBCTL  0x06C
/* Register 43 GP Timer software reset */
#define SRTIMER    0x504
/* Register 44 GPIO software reset */
#define SRGPIO     0x508
/* Register 47 UART software reset */
#define SRUART     0x518
/* Register 54 PWM software reset */
#define SRPWM      0x540
/* Register 57 wide GP Timer software reset */
#define SRWTIMER   0x55C
/* Register 60 GPIO runmode enable/disable */
#define RCGCGPIO   0x608
/* Register 63 UART runmode enable/disable */
#define RCGCUART   0x618
/* Register 70 PWM runmode enable/disable */
#define RCGCPWM    0x640
/* Register 73 WTIMER runmode enable/disable */
#define RCGCWTIMER 0x65C
/* Register 107 GP Timer ready */
#define PRTIMER    0xA04
/* Register 108 GPIO ready */
#define PRGPIO     0xA08
/* Register 111 UART ready */
#define PRUART     0xA18
/* Register 118 PWM ready */
#define PRPWM      0xA40
/* Register 136 Run mode clock gate control */
#define RCGC2	   0x108

/* Device version info */
#define SYSCTRL_DID0_VER         ((_REG((SYSCTRL + DID0)) & (0x70000000)) >> 28)
#define SYSCTRL_DID0_CLASS       ((_REG((SYSCTRL + DID0)) & (0x00FF0000)) >> 16)
#define SYSCTRL_DID0_MAJOR       ((_REG((SYSCTRL + DID0)) & (0x0000FF00)) >> 8)
#define SYSCTRL_DID0_MINOR       ((_REG((SYSCTRL + DID0)) & (0x000000FF)) >> 0)
#define SYSCTRL_DID1_VER         ((_REG((SYSCTRL + DID1)) & (0xF0000000)) >> 28)
#define SYSCTRL_DID1_FAM         ((_REG((SYSCTRL + DID1)) & (0x0F000000)) >> 24)
#define SYSCTRL_DID1_PARTNO      ((_REG((SYSCTRL + DID1)) & (0x00FF0000)) >> 16)
#define SYSCTRL_DID1_PINCOUNT    ((_REG((SYSCTRL + DID1)) & (0x0000E000)) >> 13)
#define SYSCTRL_DID1_TEMP        ((_REG((SYSCTRL + DID1)) & (0x000000E0)) >> 5)
#define SYSCTRL_DID1_PKG         ((_REG((SYSCTRL + DID1)) & (0x00000018)) >> 3)
#define SYSCTRL_DID1_ROHS        ((_REG((SYSCTRL + DID1)) & (0x00000004)) >> 2)
#define SYSCTRL_DID1_QUAL        ((_REG((SYSCTRL + DID1)) & (0x00000003)) >> 0)
/* Brown out reset control */
// Functionality not defined well
/* Raw interrupt status */
#define SYSCTRL_RIS_BOR0         ((_REG((SYSCTRL + RIS)) & (1 << 11)) >> 11)
#define SYSCTRL_RIS_VDDA         ((_REG((SYSCTRL + RIS)) & (1 << 10)) >> 10)
#define SYSCTRL_RIS_MOSCPUP      ((_REG((SYSCTRL + RIS)) & (1 << 8)) >> 8)
#define SYSCTRL_RIS_USBPLL       ((_REG((SYSCTRL + RIS)) & (1 << 7)) >> 7)
#define SYSCTRL_RIS_PLL          ((_REG((SYSCTRL + RIS)) & (1 << 6)) >> 6)
#define SYSCTRL_RIS_MOF          ((_REG((SYSCTRL + RIS)) & (1 << 3)) >> 3)
#define SYSCTRL_RIS_BOR1         ((_REG((SYSCTRL + RIS)) & (1 << 1)) >> 1)
/* Interrupt mask control status */
#define SYSCTRL_IMC_BOR0         ((_REG((SYSCTRL + IMC)) & (1 << 11)) >> 11)
#define SYSCTRL_IMC_VDDA         ((_REG((SYSCTRL + IMC)) & (1 << 10)) >> 10)
#define SYSCTRL_IMC_MOSCPUP      ((_REG((SYSCTRL + IMC)) & (1 << 8)) >> 8)
#define SYSCTRL_IMC_USBPLL       ((_REG((SYSCTRL + IMC)) & (1 << 7)) >> 7)
#define SYSCTRL_IMC_PLL          ((_REG((SYSCTRL + IMC)) & (1 << 6)) >> 6)
#define SYSCTRL_IMC_MOF          ((_REG((SYSCTRL + IMC)) & (1 << 3)) >> 3)
#define SYSCTRL_IMC_BOR1         ((_REG((SYSCTRL + IMC)) & (1 << 1)) >> 1)
/* GPIO high performance bus status */
#define SYSCTRL_GPIOHBCTL_A      ((_REG((SYSCTRL + GPIOHBCTL)) & (1 << 0)) >> 0)
#define SYSCTRL_GPIOHBCTL_B      ((_REG((SYSCTRL + GPIOHBCTL)) & (1 << 1)) >> 1)
#define SYSCTRL_GPIOHBCTL_C      ((_REG((SYSCTRL + GPIOHBCTL)) & (1 << 2)) >> 2)
#define SYSCTRL_GPIOHBCTL_D      ((_REG((SYSCTRL + GPIOHBCTL)) & (1 << 3)) >> 3)
#define SYSCTRL_GPIOHBCTL_E      ((_REG((SYSCTRL + GPIOHBCTL)) & (1 << 4)) >> 4)
#define SYSCTRL_GPIOHBCTL_F      ((_REG((SYSCTRL + GPIOHBCTL)) & (1 << 5)) >> 5)
/* GPIO high performance bus enable/disable */
#define SYSCTRL_GPIOHBCTL_APB(PORT)    _REG((SYSCTRL + GPIOHBCTL)) &= (~(1 << (PORT)))
#define SYSCTRL_GPIOHBCTL_AHB(PORT)    _REG((SYSCTRL + GPIOHBCTL)) |= (1 << (PORT))
/*
 * Software reset is a two step process
 * 1. Set the bit, which held the peripheral in reset
 * 2. Clear the bit, which release the peripheral
 */
/* Software timer reset */
#define SYSCTRL_SRTIMER_RESET(PX)    {\
	_REG((STYCTRL + SRTIMER)) |= (1 << (PX))\
	_REG((STYCTRL + SRTIMER)) &= (~(1 << (PX)))\
    }
/* GPIO Software reset */
#define SYSCTRL_SRGPIO_RESET(PX)    {\
	_REG((SYSCTRL + SRGPIO)) |= (1 << (PX))\
	_REG((SYSCTRL + SRGPIO)) &= (~(1 << (PX)))\
    }
/* UART software reset */
#define SYSCTRL_SRUART_RESET(PX)    {\
	_REG((SYSCTRL + SRUART)) |= (1 << (PX))\
	_REG((SYSCTRL + SRUART)) &= (~(1 << (PX)))\
    }
/* PWM software reset */
#define SYSCTRL_SRPWM_RESET(PX)    {\
	_REG((SYSCTRL + SRPWM)) |= (1 << (PX))\
	_REG((SYSCTRL + SRPWM)) &= (~(1 << (PX)))\
    }
/* GP Wide Timer Reset */
#define SYSCTRL_SRWTIMER_RESET(PX)    {\
	_REG((SYSCTRL + SRWTIMER)) |= (1 << (PX))\
	_REG((SYSCTRL + SRWTIMER)) &= (~(1 << (PX)))\
    }
/* GPIO runmode enable/disable */
#define SYSCTRL_GPIO_ENABLE(PX)    _REG((SYSCTRL + RCGCGPIO)) |= (1<<(PX))
#define SYSCTRL_GPIO_DISABLE(PX)    _REG((SYSCTRL + RCGCGPIO)) &= (~(1<<(PX)))
#define SYSCTRL_RCGC2_ENABLE(PX)	_REG((SYSCTRL + RCGC2)) |= (1 << (PX))
#define SYSCTRL_RCGC2_DISABLE(PX)	_REG((SYSCTRL + RCGC2)) &= (~(1 << (PX)))
/* UART runmode enable/disable */
#define SYSCTRL_UART_ENABLE(PX)    _REG((SYSCTRL + RCGCUART)) |= ((1<<(PX)))
#define SYSCTRL_UART_DISABLE(PX)    _REG((SYSCTRL + RCGCUART)) &= (~(1<<(PX)))
/* PWM runmode enable/disable */
#define SYSCTRL_PWM_ENABLE(PX)    _REG((SYSCTRL + RCGCPWM)) |= ((1<<(PX)))
#define SYSCTRL_PWM_DISABLE(PX)    _REG((SYSCTRL + RCGCPWM)) &= (~(1<<(PX)))
/* Wide Timer runmode enable/disable */
#define SYSCTRL_WTIMER_ENABLE(PX)    _REG((SYSCTRL + RCGCWTIMER)) |= ((1<<(PX)))
#define SYSCTRL_WTIMER_DISABLE(PX)    _REG((SYSCTRL + RCGCWTIMER)) &= (~(1<<(PX)))
/* GP Timer ready */
#define SYSCTRL_TIMER_READY(PX)  ((_REG((SYSCTRL + PRTIMER)) & (1 << (PX))) >> (PX))
/* GPIO ready */
#define SYSCTRL_GPIO_READY(PX)   ((_REG((SYSCTRL + PRGPIO)) & (1<<(PX))) >> (PX))
/* UART ready */
#define SYSCTRL_UART_READY(PX)   ((_REG((SYSCTRL + PRUART)) & (1<<(PX))) >> (PX))
/* PWM ready */
#define SYSCTRL_PWM_READY(PX)   ((_REG((SYSCTRL + PRPWM)) & (1<<(PX))) >> (PX))

#define SYSCTRL_RCGC1_UART0      0x00000001  // UART0 Clock Gating Control
#define SYSCTRL_RCGC2_GPIOA      0x00000001  // port A Clock Gating Control
#endif /* SYSTEM_H_ */
