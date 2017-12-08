/*
 * stringOps.h
 *
 *  Created on: Sep 10, 2017
 *      Author: eldhose
 */

#ifndef STRINGOPS_H_
#define STRINGOPS_H_

#include "dataTypes.h"
#include <string.h>

void itoa(int in, char *buffer, uint8_t base);
uint8_t atoi(char *in, int *out);

#endif /* STRINGOPS_H_ */
