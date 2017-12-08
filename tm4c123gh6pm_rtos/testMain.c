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

#define IISC_PROJECT_LINKEDLIST		0
#define IISC_NUM_PROJECTS			1

extern void iiscProjectLinkedList_main();
extern void iiscTestBlinky();
extern void iiscLEDonSwitch();
extern void iiscLED();
extern void iiscMotorControl();

int main() {

//	iiscTestBlinky();
//	iiscProjectLinkedList_main();
//	iiscLEDonSwitch();
//	iiscLED();
	iiscMotorControl();

	while(1);
	return 0;
}
