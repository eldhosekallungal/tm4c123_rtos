/*
 * assert.h
 *
 *  Created on: Oct 13, 2017
 *      Author: eldhose
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#include "dataTypes.h"

/* ASSERT FORMAT <|Assert category 4 bits|rfu 20 bits|Assert type 8 bits|> */
typedef enum {
	/* System asserts */
	ASSERT_SYS_ = 0x00000000,
	/* memory asserts */
	ASSERT_MEM_ALLOC_FAILURE = 0x01000000,
	ASSERT_MEM_DEALLOC_FAILURE,
	ASSERT_MEM_REALLOC_FAILURE,
	/* peripheral asserts */
	ASSERT_PERIPHERAL = 0x02000000,
	/* user asserts */
	ASSERT_USER = 0x03000000,

	ASSERT_NUM_ASSERTS
} sysAssert_t;
#define ASSERT(cond, msg)    {\
	if((cond) == true) \
	    sysAssert(msg, NULL, NULL); \
    }

#define DEBUG_ASSERT(cond, msg) { \
    	if(((cond) == true) && (DEBUG_EN == TRUE)) \
    	    sysAssert(msg, __FUNC__, __LINE__); \
    }

void sysAssert(sysAssert_t assert, char *msg, char *func, char *line);
#endif /* ASSERT_H_ */
