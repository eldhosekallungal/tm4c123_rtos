/* Dynamic memory allocation */
#include "dataTypes.h"
#include "sysos.h"
#include "sysmem.h"

struct deallocMem{
	uint32_t head;
	struct deallocMem *pNext;
};

static struct {
	void *pMemStart;
	void *pMemEnd;
	void *pFreeMem;
	struct deallocMem *freedMem;
}osalDynamicMemBlock[DYNMEM_NUM_SECTOR];

static const uint16_t MEM_dynMemMaxAlloc[DYNMEM_NUM_SECTOR] = {
		(1 << 8)  - 1,
		(1 << 16) - 1,
		(1 << 16) - 1,
		(1 << 16) - 1
};

static void osalMemInternal_SetHeader(void *pFreeMem, uint32_t numMem, MEM_eDynMemSector_t sector);
static bool osalMemInternal_ValidateHeader(uint32_t header);
static uint32_t osalMemInternal_GetOffset(uint32_t header, MEM_eDynMemSector_t sector);
static bool osalMemInternal_GetHeader(void *pMem, uint8_t **ppHead, uint8_t *sector);

static uint32_t crcPoly = 0x000002D3; /* 0 0 1 0 1 1 0 1 0 0 1 1 */

void osalMem_MemSet(void *pMem, uint32_t val, uint32_t len, uint8_t wl) {

	uint8_t *pU8Mem;
	uint16_t *pU16Mem;
	uint32_t *pU32Mem;

	OSAL_ASSERT(((len % wl) == 0), OSAL_MEM_UNALIGNED);

	switch(wl) {
	case sizeof(uint8_t):
		pU8Mem = (uint8_t *)pMem;
		while(len-- > 0) {
			*pU8Mem = (val & 0xFF);
			pU8Mem++;
		}
		break;
	case sizeof(uint16_t):
		pU16Mem = (uint16_t *)pMem;
		while(len-- > 0) {
			*pU16Mem = (val & 0xFFFF);
			pU16Mem++;
		}
		break;
	case sizeof(uint32_t):
		pU32Mem = (uint32_t *)pMem;
		while(len-- > 0) {
			*pU32Mem = (val);
			pU32Mem++;
		}
		break;
	default:
		OSAL_ASSERT(0, OSAL_MEM_BAD_WORD_LENGTH);
		break;
	}
}

void osalMem_MemCopy(void *dest, void *src, uint32_t len) {
	/* 4 byte array copy */
	uint8_t rem;
	rem = len%4;
	while(len > rem) {
		*(uint32_t*)dest++ = *(uint32_t*)src++;
		len -= 4;
	}
	while(rem-- > 0) {
		*(uint8_t *)dest++ = *(uint8_t *)src++;
	}
}
void osalMem_DynamicMemInit(void *pMem, uint32_t numAlloc, MEM_eDynMemSector_t sector) {
	uint32_t *head;
	uint8_t DYNMEM_HEAD_SIZE;

	OSAL_ASSERT(pMem != NULL, OSAL_MEM_NULL_POINTER);
	OSAL_ASSERT(numAlloc > 0, OSAL_MEM_NO_MEM);
	OSAL_ASSERT(sector < DYNMEM_NUM_SECTOR, OSAL_MEM_BAD_SECTOR);
	OSAL_ASSERT(IS_ALIGNED32(numAlloc), OSAL_MEM_UNALIGNED);	/* should be 32 bit aligned */

	DYNMEM_HEAD_SIZE = (2 << sector);

	osalDynamicMemBlock[sector].pMemStart = pMem;
	osalDynamicMemBlock[sector].pMemEnd = pMem + numAlloc;
	osalDynamicMemBlock[sector].pFreeMem = pMem + DYNMEM_HEAD_SIZE;		/* 4 bytes are header */
	osalDynamicMemBlock[sector].freedMem = NULL;

#ifdef OSAL_CLEAR_DYNAMIC_MEM_ON_STARTUP
	uint32_t i_cnt;
	uint32_t *pClrMem = (uint32_t *)pMem;
	for(i_cnt = 0; i_cnt < numAlloc; i_cnt++) {
		*pClrMem++ = 0;
	}
#endif

	head = (uint32_t *)(osalDynamicMemBlock[sector].pFreeMem - DYNMEM_HEAD_SIZE);
	*head = 0x00000000;

	return;
}

void *osalMem_DynamicMemAlloc(uint32_t numMem, MEM_eDynMemSector_t sector) {

	void *ret;
	uint8_t DYNMEM_HEAD_SIZE;

	if(sector > DYNMEM_NUM_SECTOR)
		return NULL;

	if(numMem > MEM_dynMemMaxAlloc[sector])
		return NULL;

	if(osalDynamicMemBlock[sector].pMemStart == NULL)
		return NULL;

	DYNMEM_HEAD_SIZE = (2 << sector);

	ALIGN(numMem, DYNMEM_HEAD_SIZE);	/* align to head size */
	/* Last 2 bytes are */
	/* Try to allocate from free area */
	ret = osalMem_AllocFromFree(numMem, sector);
	if(ret != NULL)
		return ret;
	if((osalDynamicMemBlock[sector].pFreeMem + numMem) > osalDynamicMemBlock[sector].pMemEnd)
		return NULL;

	/* store the memory pointer */
	ret = osalDynamicMemBlock[sector].pFreeMem;
	/* clear all header */
	osalMem_MemSet((ret - DYNMEM_HEAD_SIZE), 0, DYNMEM_HEAD_SIZE, 1);
	/* set the header */
	osalMemInternal_SetHeader(osalDynamicMemBlock[sector].pFreeMem, numMem, sector);
	/* increment the memory pointer to next memory location*/
	osalDynamicMemBlock[sector].pFreeMem += (numMem + DYNMEM_HEAD_SIZE);
	/* return available memory */
	return ret;
}

void *osalMem_AllocFromFree(uint32_t numMem, MEM_eDynMemSector_t sector) {

	struct deallocMem *pDeallocMem, *pHead;
	uint32_t numFreeMem = 0;
	uint8_t DYNMEM_HEAD_SIZE;

	DYNMEM_HEAD_SIZE = (2 << sector);

	pDeallocMem = osalDynamicMemBlock[sector].freedMem;
	pHead = osalDynamicMemBlock[sector].freedMem;
	while((numFreeMem < numMem) && (pDeallocMem != NULL)) {
		OSAL_ASSERT(osalMemInternal_ValidateHeader(pDeallocMem->head), OSAL_DYNMEM_INVALID_HEADER);
		numFreeMem = osalMemInternal_GetOffset(pDeallocMem->head, sector);
		while((numFreeMem < numMem) && (pDeallocMem->pNext != NULL)) {
			if((pDeallocMem + osalMemInternal_GetOffset(pDeallocMem->head, sector) + DYNMEM_HEAD_SIZE) == pDeallocMem->pNext) {
				pDeallocMem = pDeallocMem->pNext;
				numFreeMem += osalMemInternal_GetOffset(pDeallocMem->head, sector);
			}
			else {
				break;
			}
		}
		if(numFreeMem >= numMem) {
			/* found the memory*/
			break;
		}
		/* change the head */
		pHead = pDeallocMem;
		pDeallocMem = pDeallocMem->pNext;
	}

	if(numFreeMem >= numMem) {
		/* memory is available allocate it */
		return (((void *)pHead) + DYNMEM_HEAD_SIZE);
	}
	else {
		return NULL;
	}
}
void osalMem_DynamicMemFree(void *pMem) {

	uint8_t *head;
	uint8_t sector;
	uint8_t DYNMEM_HEAD_SIZE;

	struct deallocMem *pDeallocMem, *pTmp;

	if(pMem == NULL)
		return;

	OSAL_ASSERT(osalMemInternal_GetHeader(pMem, &head, &sector), OSAL_DYNMEM_INVALID_HEADER);

	if((pMem < osalDynamicMemBlock[sector].pMemStart) || (pMem > osalDynamicMemBlock[sector].pMemEnd))
		OSAL_ASSERT(FALSE, OSAL_MEM_BAD_SECTOR);



	/* if we are freeing a freed memory, just return */
	if(DYNMEM_GET_MEM_STATUS(*head) == DYNMEM_FREED)
		return;

	/* mark the header as freed memory */
	DYNMEM_SET_MEM_STATUS(head, DYNMEM_FREED);

	DYNMEM_HEAD_SIZE = (2 << sector);

	/* check initial condition */
	if(osalDynamicMemBlock[sector].freedMem == NULL) {
		osalDynamicMemBlock[sector].freedMem = (struct deallocMem *)(pMem - DYNMEM_HEAD_SIZE);
		osalDynamicMemBlock[sector].freedMem->pNext = NULL;
		return;
	}
	pDeallocMem = osalDynamicMemBlock[sector].freedMem;
	/* sort and add to the linked list */
	while(pDeallocMem->pNext != NULL) {
		if(((unsigned long)pDeallocMem < (unsigned long)(pMem - DYNMEM_HEAD_SIZE)) &&
			((unsigned long)pDeallocMem->pNext > (unsigned long)(pMem - DYNMEM_HEAD_SIZE))) {
			break;
		}
		pDeallocMem = pDeallocMem->pNext;
	}
	pTmp = pDeallocMem->pNext;
	pDeallocMem->pNext = (struct deallocMem *)(pMem - DYNMEM_HEAD_SIZE);
	pDeallocMem->pNext->pNext = pTmp;

	pMem = NULL;
}
void osalMemInternal_SetHeader(void *pFreeMem, uint32_t numMem, MEM_eDynMemSector_t sector) {

	uint32_t header;
	uint32_t crc;
	uint8_t *pHeader;
	uint8_t DYNMEM_HEAD_SIZE;

	DYNMEM_HEAD_SIZE = (2 << sector);

	pHeader = (uint8_t *)(pFreeMem - DYNMEM_HEAD_SIZE);

	/* head size is depend on the sector */

	header = 0x00000000;
	crc = 0x00000000;

	switch(sector) {
	case DYNMEM_SECTOR_UNALIGNED:
	{
		/* highest 8 bytes are offset */
		header |= ((numMem & 0x000000FF) << 8);
		/* 4 byte CRC */
//		crc = mathCalcCRC32b(header, crcPoly, 4);
		header |= ((crc & 0x0000000F) << 4);
		/* 2 bytes sector */
		header |= ((sector & 0x03) << 2);
		/* 2 byte header flag */
		header |= (DYNMEM_ALLOCATED & 0x03);
		break;
	}
	case DYNMEM_SECTOR_ALIGNED_4:
	case DYNMEM_SECTOR_ALIGNED_8:
	case DYNMEM_SECTOR_ALIGNED_16:
	{
		/* last 16 bits are offset*/
		header |= ((numMem & 0x0000FFFF) << 16);
		/* 12 bits are CRC */
//		crc = mathCalcCRC32b(header, crcPoly, 12);
		header |= ((crc & 0x00000FFF) << 4);
		/* next 2 bits are sector */
		header |= ((sector & 0x03) << 2);
		/* since the last bits have high tolerance this can be avoided from CRC calculation */
		/* last 2 bits are header flag */
		header |= (DYNMEM_ALLOCATED & 0x03);
		break;
	}
	default:
		break;
	}
	{
		/* endian independent */
		*pHeader++ = ((header & 0x000000FF) >> 0);
		*pHeader++ = ((header & 0x0000FF00) >> 8);
		*pHeader++ = ((header & 0x00FF0000) >> 16);
		*pHeader++ = ((header & 0xFF000000) >> 24);
	}
}

static bool osalMemInternal_ValidateHeader(uint32_t header) {
	return TRUE;
}

static uint32_t osalMemInternal_GetOffset(uint32_t header, MEM_eDynMemSector_t sector) {

	uint32_t offset = 0;

	switch(sector) {
	case DYNMEM_SECTOR_UNALIGNED:
		offset = ((header & 0x0000FF00) >> 8);
		break;
	case DYNMEM_SECTOR_ALIGNED_4:
	case DYNMEM_SECTOR_ALIGNED_8:
	case DYNMEM_SECTOR_ALIGNED_16:
		offset = ((header & 0xFFFF0000) >> 16);
		break;
	default:
		offset = 0;
		break;
	}

	return offset;
}

static bool osalMemInternal_GetHeader(void *pMem, uint8_t **ppHead, uint8_t *sector) {

	return TRUE;
}
