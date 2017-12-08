/*
 * dataTypes.h
 *
 *  Created on: Sep 4, 2017
 *      Author: eldhose
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdint.h>
#include <stddef.h>

#define __UNIX__

/* Register definition */
#define  _ADDR     volatile unsigned long
#define  _REG(X)  (*(volatile unsigned long *)(X))
#ifndef NULL
#define NULL (void *)0
#endif

#ifdef __UNIX__
#define CHR_TER    "\n\r"
#else
#define CHR_TER    "\n"
#endif

#define FALSE (0)
#define TRUE  (!FALSE)

/* Debug flag */
#ifdef DEBUG
#define DEBUG_EN TRUE
#else
#define DEBUG_EN FALSE
#endif
/* Return status types */
#define STATUS_OK    0

typedef unsigned char bool;
/* function pointer for logging */
typedef void (*pInfoFunc)(char *str, ...);

#endif /* DATATYPES_H_ */
