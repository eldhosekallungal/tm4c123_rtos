/*
 * sysos.h
 *
 *  Created on: Dec 2, 2017
 *      Author: eldhose
 */

#ifndef SYSOS_H_
#define SYSOS_H_

#include "sysCtrl.h"

extern unsigned long system_assert_register;

/* memory asserts */
#define OSAL_MEM_NULL_POINTER		0x00100000
#define OSAL_MEM_NO_MEM				0x00100001
#define OSAL_MEM_BAD_SECTOR			0x00100002
#define OSAL_MEM_UNALIGNED			0x00100003
#define OSAL_MEM_BAD_WORD_LENGTH	0x00100004
#define OSAL_DYNMEM_INVALID_HEADER	0x00200000

#define OSAL_GPIO_UNKNOWN_PORT		0x01000000

#define OSAL_ASSERT(cond, AM)	{\
		if(!(cond)) { \
		system_assert_register = (AM); \
		for(;;); \
		} \
}

#define OS_TIME_MS	1
#define OS_TIME_SLICE	((SYSTEM_CORE_CLOCK / 1000) * OS_TIME_MS)

typedef void (*sysCallBack_t) ();

typedef enum {
	OS_STATE_INIT = 0,	/* initialization state*/
	OS_STATE_RUNNING,	/* running state */
	OS_STATE_SUSPENDED,	/* suspended, no tasks are running */
	OS_STATE_ASSERT,		/* Assert occurred */
	OS_STATE_UNKNOWN
}OS_eState_t;
typedef enum {
	OS_TASK_RUNNING = 0,
	OS_TASK_BLOCKED,
	OS_TASK_SUSPENDED
}OS_eTaskState_t;
typedef enum {
	SEM_OS,
	SEM_SYSTEM,
	SEM_MEMORY,
	SEM_GPIO,
	SEM_UART,
	SEM_TIMER,
}semType_t;
typedef struct semaphore {

	semType_t type;
	int32_t count;
	void *data;
}semaphore_t;

typedef struct OS_sTaskMessage {
	unsigned long mbox_id;
	unsigned long mbox_max_len;
	void *mbox;
}OS_sTaskMessage_t;
typedef struct OS_sTaskHandle {
	volatile unsigned long *sp;
	volatile struct OS_sTaskHandle *nextTask;
	volatile unsigned long *stackTop;
	volatile unsigned char taskState;
	volatile char *taskName;
	volatile unsigned long taskPriority;
	volatile OS_sTaskMessage_t *xTaskMessageHandle;
	volatile void *pTaskParams;
	volatile unsigned long taskStackSize;
}OS_sTaskHandle_t;

semaphore_t *OS_createSemaphote(semType_t type, void *data, uint32_t dataLen);
void OS_delateSemaphore(semaphore_t *pSem);
void OS_semaphoreSignal(semaphore_t *pSem);
void OS_semaphoteWait(semaphore_t *pSem);
void OS_createTask(OS_sTaskHandle_t *handle, void *task, char *taskName, unsigned long taskStackSize, unsigned long taskPriority, void *pTaskParams);
void OS_scheduleTask();
void OS_sysTickHandler();

#endif /* SYSOS_H_ */
