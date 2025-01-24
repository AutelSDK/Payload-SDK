
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OSAL_H_
#define _OSAL_H_

/* Includes ------------------------------------------------------------------*/
#include "uav_platform.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "limits.h"
#include "timers.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions --------------------------------------------------------*/
int os_task_create(const char *name, void *(*taskFunc)(void *), uint32_t stackSize,
                                void *arg, uint32_t priority,T_UAVTaskHandle *task);

int os_task_start(T_UAVTaskHandle task);

int os_task_destroy(T_UAVTaskHandle task);

int os_task_sleep_ms(uint32_t time_ms);

int os_mutex_create(T_UAVMutexHandle *mutex);

int os_mutex_destroy(T_UAVMutexHandle mutex);

int os_mutex_lock(T_UAVMutexHandle mutex);

int os_mutex_unlock(T_UAVMutexHandle mutex);

int os_semaphore_create(uint32_t initValue, T_UAVSemaHandle *semaphore);

int os_semaphore_destroy(T_UAVSemaHandle semaphore);

int os_semaphor_time_wait(T_UAVSemaHandle semaphore, uint32_t wait_time_ms);

int os_semaphore_wait(T_UAVSemaHandle semaphore);

int os_semaphore_post(T_UAVSemaHandle semaphore);

int os_get_time_ms(uint32_t *ms);

int os_get_time_us(uint64_t *us);

void *os_malloc(uint32_t size);

void os_free(void *ptr);

int os_ms_to_ticks(uint32_t ms);

void os_task_list(char *list);

uint32_t os_get_free_heap_size(void);

#ifdef __cplusplus
}
#endif

#endif // OSAL_H
