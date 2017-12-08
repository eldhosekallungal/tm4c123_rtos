/*
 * peripheral.h
 *
 *  Created on: Dec 3, 2017
 *      Author: eldhose
 */

#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#define PERIPHERAL_BASE_ADDR	0xE000E000

/* peripheral offsets */
#define STCTRL				0x010
#define STRELOAD			0x014
#define STCURRENT			0x018
/* NVIC registers */
/* interrupt set enable */
#define EN0					0x100
#define EN1					0x104
#define EN2					0x108
#define EN3					0x10C
#define EN4					0x110
/* interrupt clear enable */
#define DIS0				0x180
#define DIS1				0x184
#define DIS2				0x188
#define DIS3				0x18C
#define DIS4				0x190

#define PEND0               0x200
#define PEND1               0x204
#define PEND2               0x208
#define PEND3               0x20C
#define PEND4               0x210

#define UNPEND0        		0x280
#define UNPEND1        		0x284
#define UNPEND2        		0x288
#define UNPEND3        		0x28C
#define UNPEND4        		0x290

#define ACTIVE0        		0x300
#define ACTIVE1        		0x304
#define ACTIVE2        		0x308
#define ACTIVE3        		0x30C
#define ACTIVE4        		0x310

#define PRI0        		0x400
#define PRI1        		0x404
#define PRI2        		0x408
#define PRI3        		0x40C
#define PRI4        		0x410
#define PRI5        		0x414
#define PRI6        		0x418
#define PRI7        		0x41C
#define PRI8        		0x420
#define PRI9        		0x424
#define PRI10        		0x428
#define PRI11        		0x42C
#define PRI12        		0x430
#define PRI13        		0x434
#define PRI14        		0x438
#define PRI15        		0x43C
#define PRI16        		0x440
#define PRI17        		0x444
#define PRI18        		0x448
#define PRI19        		0x44C
#define PRI20        		0x450
#define PRI21        		0x454
#define PRI22        		0x458
#define PRI23        		0x45C
#define PRI24        		0x460
#define PRI25        		0x464
#define PRI26        		0x468
#define PRI27        		0x46C
#define PRI28        		0x470
#define PRI29        		0x474
#define PRI30        		0x478
#define PRI31        		0x47C
#define PRI32        		0x480
#define PRI33        		0x484
#define PRI34        		0x488

#define SWTRIG        		0xF00
#define ACTLR        		0x008
#define CPUID        		0xD00
#define INTCTRL        		0xD04
#define VTABLE        		0xD08
#define APINT        		0xD0C
#define SYSCTRL_I        	0xD10
#define CFGCTRL        		0xD14
#define SYSPRI1        		0xD18
#define SYSPRI2        		0xD1C
#define SYSPRI3        		0xD20
#define SYSHNDCTRL        	0xD24
#define FAULTSTAT        	0xD28
#define HFAULTSTAT        	0xD2C
#define MMADDR       	 	0xD34
#define FAULTADDR        	0xD38
#define MPUTYPE        		0xD90
#define MPUCTRL        		0xD94
#define MPUNUMBER        	0xD98
#define MPUBASE        		0xD9C
#define MPUATTR        		0xDA0
#define MPUBASE1       		0xDA4
#define MPUATTR1       		0xDA8
#define MPUBASE2       		0xDAC
#define MPUATTR2       		0xDB0
#define MPUBASE3       		0xDB4
#define MPUATTR3       		0xDB8
#define CPAC        		0xD88
#define FPCC        		0xF34
#define FPCA        		0xF38
#define FPDSC       		0xF3C

#endif /* PERIPHERAL_H_ */
