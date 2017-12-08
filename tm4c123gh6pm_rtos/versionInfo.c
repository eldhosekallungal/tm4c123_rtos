/*
 * versionInfo.c
 *
 *  Created on: Oct 13, 2017
 *      Author: eldhose
 */

/* HEADER */
#include "dataTypes.h"

static char name[] = "__UART_LINKED_LIST_IISC__";
static char cert[] = "Open license";
static char version[] = "1.0.0";
static char svnInfo[] = "-nil-";
static char buildInfo[] = "<date=- nil- >, <time= -nil- >, <flags = -nil- >";

/* call back pointer type */

void info(void *funcPtr) {

	((pInfoFunc)funcPtr)("PROJECT: %s\nLICENSE: %s\nVERSION: %s\nSVN: %s\nBUILD INFO: %s\n", name, cert, version, svnInfo, buildInfo);
}

