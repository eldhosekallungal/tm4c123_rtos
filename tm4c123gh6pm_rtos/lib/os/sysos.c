/*
 * sysos.c
 *
 *  Created on: Dec 2, 2017
 *      Author: eldhose
 */
#include "sysmem.h"
#include "sysos.h"
#include "peripheral.h"
#include "dataTypes.h"
#include "sysio.h"

unsigned long system_assert_register;

volatile static OS_sTaskHandle_t *RunPt = NULL;
void *maxPriorityTask = NULL;
unsigned long OS_STATE = OS_STATE_UNKNOWN;

static void OS_scheduler();

void OS_systemInit(sysCallBack_t IO_callback, sysCallBack_t MEM_callBack) {

	OSAL_ASSERT((MEM_callBack != NULL), OSAL_MEM_NULL_POINTER);
	OSAL_ASSERT((IO_callback != NULL), OSAL_MEM_NULL_POINTER);
	/* initialize memory first */

	IO_callback();
	MEM_callBack();

	RunPt = NULL;
	maxPriorityTask = NULL;

	OS_STATE = OS_STATE_INIT;
}
semaphore_t *OS_createSemaphote(semType_t type, void *data, uint32_t dataLen) {

	semaphore_t *ret;

	ret = (semaphore_t *)osalMem_DynamicMemAlloc(sizeof(semaphore_t), DYNMEM_SECTOR_ALIGNED_4);
	ret->count = 0;
	ret->type = type;

	if((data != NULL) && (dataLen != 0)) {
		ret->data = osalMem_DynamicMemAlloc(dataLen, DYNMEM_SECTOR_ALIGNED_4);
		(void)osalMem_MemCopy(ret->data, data, dataLen);
	}
	else {
		ret->data = NULL;
	}

	return ret;
}

void OS_delateSemaphore(semaphore_t *pSem) {

	if(pSem->data != NULL) {
		osalMem_DynamicMemFree(pSem->data);
	}

	osalMem_DynamicMemFree(pSem);
}

void OS_semaphoreSignal(semaphore_t *pSem) {
	pSem->count++;
	/* rest has to be implemented */
}

void OS_semaphoteWait(semaphore_t *pSem) {

	pSem->count--;

	/* Wait until semapore is signaled */
	/* This has to changed to task suspension later*/
	while(pSem->count < 0);
}

void OS_createTask(OS_sTaskHandle_t *handle, void *task, char *taskName, unsigned long taskStackSize, unsigned long taskPriority, void *pTaskParams) {

	volatile OS_sTaskHandle_t *pCurr, *pPrev;
	handle = (OS_sTaskHandle_t *)osalMem_DynamicMemAlloc(sizeof(OS_sTaskHandle_t), DYNMEM_SECTOR_ALIGNED_16);
	handle->taskName = taskName;
	ALIGN(taskStackSize, 8);	/* stack should be 8 byte aligned */
	handle->sp = (unsigned long *)osalMem_DynamicMemAlloc(taskStackSize * sizeof(unsigned long), DYNMEM_SECTOR_ALIGNED_16);
	handle->stackTop = handle->sp;
	handle->taskState = OS_TASK_RUNNING;
	handle->xTaskMessageHandle = NULL;
	handle->taskStackSize = taskStackSize;
	/* add to task list */
	if(RunPt == NULL) {/* fresh task */
		RunPt = handle;
		handle->nextTask = NULL;
		maxPriorityTask = handle;
	}
	else if(RunPt->nextTask == NULL) {/* second task */
		RunPt->nextTask = handle;
		handle->nextTask = RunPt;
		if(RunPt->taskPriority < handle->taskPriority)
			maxPriorityTask = (void *)handle;
	}
	else {/* furthur tasks */
		pPrev = (OS_sTaskHandle_t *)maxPriorityTask;
		pCurr = pPrev->nextTask;
		while(pCurr->taskPriority >= taskPriority) {
			pPrev = pCurr;
			pCurr = pCurr->nextTask;
		}
		pPrev->nextTask = handle;
		handle->nextTask = pCurr;

		pPrev = (OS_sTaskHandle_t *)maxPriorityTask;
		if(pPrev->taskPriority < handle->taskPriority)
			maxPriorityTask = (void *)handle;
	}

	handle->sp = handle->sp + (taskStackSize - 16);

	*(handle->sp)     =  0x04040404; // R04
	*(handle->sp + 1) =  0x05050505; // R05
	*(handle->sp + 2) =  0x06060606; // R06
	*(handle->sp + 3) =  0x07070707; // R07
	*(handle->sp + 4) =  0x08080808; // R08
	*(handle->sp + 5) =  0x09090909; // R09
	*(handle->sp + 6) =  0x10101010; // R10
	*(handle->sp + 7) =  0x11111111; // R11
	*(handle->sp + 8) =  0x00000000; // R00
	*(handle->sp + 9) =  0x01010101; // R01
	*(handle->sp + 10) = 0x02020202; // R02
	*(handle->sp + 11) = 0x03030303; // R03
	*(handle->sp + 12) = 0x12121212; // R12
	*(handle->sp + 13) = 0x14141414; // R14 link register
	*(handle->sp + 14) = (unsigned long)task;	// task pointer
	*(handle->sp + 15) = 0x1000000;	// PSR, thumb bit is 1

}

void OS_scheduleTask() {

	if(OS_STATE == OS_STATE_RUNNING)
		return;
	if(RunPt == NULL)
		return;

	/* Turn off sysTick */
	_REG(PERIPHERAL_BASE_ADDR + STCTRL) = 0x00;
	_REG(PERIPHERAL_BASE_ADDR + STCURRENT) = 0x00;
	_REG(PERIPHERAL_BASE_ADDR + SYSPRI3) = 0x70000000;	// Lowest priority for systick
	_REG(PERIPHERAL_BASE_ADDR + STRELOAD) = (OS_TIME_SLICE - 1);
	_REG(PERIPHERAL_BASE_ADDR + STCTRL) = 0x07;

	/* start the operating system */
	asm volatile (
		"LDR R0, =RunPt\n"
		"LDR R1, [R0]\n"
		"LDR SP, [R1]\n"
		"POP {R4-R11}\n"
		"POP {R0-R3}\n"
		"POP {R12}\n"
		"ADD SP, #4\n"
		"POP {LR}\n"
		"ADD SP, #4\n"
		"CPSIE I\n"
	);
	OS_STATE = OS_STATE_RUNNING;
	/* The link register is modified to first task, so it will return to first task */
	asm volatile ("BX LR\n");
}

void OS_sysTickHandler() {
	/* When sysTick interrupt occurred here it comes */

	if(RunPt->nextTask == NULL)
		return;
	if(OS_STATE != OS_STATE_RUNNING)
		return;

	asm volatile (
		"CPSID I\n"	 			// Disable interrupt
		"PUSH {R4-R11}\n"		// Push registers
		"LDR R0, =RunPt\n"		// Get the thread control block
		"LDR R1, [R0]\n"		// Load the thread stack pointer
		"STR SP, [R1]\n"		// Store the current running stack pointer (MSP) to thread SP
		"LDR R1, [R1, #4]\n"	// Move to next task
		"STR R1, [R0]\n"		// change RunPt
		"LDR SP, [R1]\n"		// Load new stack pointer
		"POP {R4-R11}\n"		// Pop all registers
		"CPSIE I\n"				// Enable interrupts
		"BX LR\n"				// return
	);

}

static void OS_scheduler() {
	RunPt = RunPt->nextTask;
}
