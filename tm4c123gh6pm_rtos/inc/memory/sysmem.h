/*
 * mem.h
 *
 *  Created on: Sep 8, 2017
 *      Author: eldhose
 */

#ifndef SYS_MEM_H_
#define SYS_MEM_H_

#include "dataTypes.h"

/* Base memory structure */
/*
 * |<valid tag 1 bit>|<parity 1 bit>|<rfu 2 bit>|<checksum 12 bit>|<length 16 bit>|
 *
 */
/* Total heap size will be 10k */
#define NUM_HEAP_PAGES (10)
#define HEAP_PAGE_SIZE (1024)
#define HEAP_SIZE (10 * 1024)

typedef enum {
	DYNMEM_SECTOR_UNALIGNED = 0,
	DYNMEM_SECTOR_ALIGNED_4,
	DYNMEM_SECTOR_ALIGNED_8,
	DYNMEM_SECTOR_ALIGNED_16,
	DYNMEM_NUM_SECTOR
}MEM_eDynMemSector_t;

#define DYNMEM_UNALLOCATED	0
#define DYNMEM_FREED		1
#define DYNMEM_ALLOCATED	2


#define DYNMEM_GET_MEM_STATUS(HEAD)					((HEAD) & 0x03)
#define DYNMEM_SET_MEM_STATUS(pHEAD, VAL)			{\
														(*(pHEAD)) &= ~(0x03);	\
														(*(pHEAD)) |= (VAL);		\
													}
#define DYNMEM_GET_SECTOR(HEAD)						(((HEAD) >> 2) & 0x03)
#define DYNMEM_SET_SECTOR(pHEAD, VAL)				{\
														(*(pHEAD)) &= ~(0x03 << 2);	\
														(*(pHEAD)) |= ((VAL) << 2);	\
													}

#define IS_ALIGNED32(VAL)	(((VAL) % 32) == 0)
#define ALIGN(val, align) {\
	if(((align) > 0)&&((val) != (align))) \
        (val) = (val) + ((align) - ((val) % (align))); \
    }

void osalMem_MemSet(void *pMem, uint32_t val, uint32_t len, uint8_t wl);
void osalMem_MemCopy(void *dest, void *src, uint32_t len);
void osalMem_DynamicMemInit(void *pMem, uint32_t numAlloc, MEM_eDynMemSector_t sector);
void *osalMem_DynamicMemAlloc(uint32_t numMem, MEM_eDynMemSector_t sector);
void *osalMem_AllocFromFree(uint32_t numMem, MEM_eDynMemSector_t sector);
void osalMem_DynamicMemFree(void *pMem);

#endif /* MEM_H_ */
