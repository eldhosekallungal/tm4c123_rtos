#include "sysCtrl.h"
#include "dataTypes.h"
/* Device identification */



/* System core clock init */

/* set the configuration */
/* Run mode clock control register configuration */
#define AUTO_CLOCK_GATING			FALSE
#define SYSTEM_CLOCK_DIVISOR		0xF
#define ENABLE_SYSTEM_CLOCK_DIVIDER	FALSE
#define ENABLE_PWM_CLOCK_DIVIDER	FALSE
#define PWM_CLOCK_DIVISOR			0x7
#define PLL_PWR_DOWN				TRUE
#define PLL_BYPASS					TRUE
#define XTAL_VALUE					0x0B
#define OSC_SOURCE					0x01 /* precision internal oscillator */
#define MAIN_OSC_DISABLE			TRUE
/* Run mode clock control - 2 register configuration */
/* initialize the system clock for above configurations */
void BSP_systemCoreClockInit();
