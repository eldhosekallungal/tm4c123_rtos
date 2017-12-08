/*
 * stringOps.c
 *
 *  Created on: Sep 10, 2017
 *      Author: eldhose
 */

#include "stringOps.h"
#include "sysmem.h"

void itoa(int in, char *buffer, uint8_t base) {

	unsigned int i_cnt;

	if(in < 0) {
		buffer[0] = '-';
		in *= -1;
		buffer++;
	}

	/* 2^32 corresponds to 32 binary position */
	/* TODO Implement hex */
	i_cnt = 32;
	buffer[33] = '\0';
	/* if input is zero */
	if(in == 0) {
		*buffer++ = 48;
		*buffer = '\0';
		return;
	}
	/* Else */
	while(in != 0) {
		buffer[i_cnt] = 48 + (in % base);
		in = in / base;
		i_cnt--;
	}

	osalMem_MemCopy(buffer, &buffer[i_cnt+1], (32-i_cnt+1));
}

uint8_t atoi(char *in, int *out) {

	int mul;
	int ret = 0;
	uint8_t i_cnt;
	if(in[0] == '-') {
		mul = -1;
		i_cnt = 1;
	}
	else if((in[0] >= 0x30) && (in[0] <= 0x39)){
		mul = 1;
		i_cnt = 0;
	}
	else {
		return -1;
	}
	while((in[i_cnt] != ' ') && (in[i_cnt] != '\0')) {
		if((in[i_cnt] < 0x30) || (in[i_cnt] > 0x39))
			return -1;
		ret = 10*ret + (uint8_t)(in[i_cnt] - 0x30);
		i_cnt++;
	}

	*out = (ret * mul);
	return i_cnt;
}
