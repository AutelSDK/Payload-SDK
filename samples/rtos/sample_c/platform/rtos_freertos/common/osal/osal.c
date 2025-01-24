/*
 * @Author: kevin && R22006/uavrobotics.cn
 * @Date: 2023-10-12 08:51:39
 * @LastEditors: kevin && R22006/uavrobotics.cn
 * @LastEditTime: 2023-10-17 19:08:17
 * @Description: os instance demo
 * Copyright (c) 2023 Uav Robotics. All rights reserved.
 */

/* Includes ------------------------------------------------------------------*/
#include "osal.h"


/* Private constants ---------------------------------------------------------*/
#define SEM_MUTEX_WAIT_FOREVER      0xFFFFFFFF


int os_task_create(const char *name, void *(*taskFunc)(void *), uint32_t stackSize,
                                void *arg, uint32_t priority,T_UAVTaskHandle *task)
{
    uint32_t stackDepth;
    char nameDealed[configMAX_TASK_NAME_LEN] = {0};
    taskENTER_CRITICAL();
    //attention :  freertos use stack depth param, stack size = (stack depth) * sizeof(StackType_t)
    if (stackSize % sizeof(StackType_t) == 0) {
        stackDepth = stackSize / sizeof(StackType_t);
    } else {
        stackDepth = stackSize / sizeof(StackType_t) + 1;
    }

    if (name != NULL)
        strncpy(nameDealed, name, sizeof(nameDealed) - 1);
    if (xTaskCreate((TaskFunction_t) taskFunc, nameDealed, stackDepth, arg, priority, (TaskHandle_t *)task) != pdPASS) {
        *task = NULL;
    }
    taskEXIT_CRITICAL();

    return 0;
}

int os_task_start(T_UAVTaskHandle task)
{
    vTaskStartScheduler();

    return 0;
}

int os_task_destroy(T_UAVTaskHandle task)
{
    vTaskDelete(task);

    return 0;
}

int os_task_sleep_ms(uint32_t time_ms)
{
    TickType_t ticks;

    ticks = time_ms / portTICK_PERIOD_MS;

    /* Minimum delay = 1 tick */
    vTaskDelay(ticks ? ticks : 1);

    return 0;
}

int os_mutex_create(T_UAVMutexHandle *mutex)
{
    *mutex = xSemaphoreCreateMutex();
    if (*mutex == NULL) {
        return -31;
    }

    return 0;
}

int os_mutex_destroy(T_UAVMutexHandle mutex)
{
    vQueueDelete((SemaphoreHandle_t) mutex);

    return 0;
}

int os_mutex_lock(T_UAVMutexHandle mutex)
{
    TickType_t ticks;

    if (mutex == NULL) {
        return -21;
    }

    ticks = portMAX_DELAY;

    if (xSemaphoreTake(mutex, ticks) != pdTRUE) {
        return -30;
    }

    return 0;
}

int os_mutex_unlock(T_UAVMutexHandle mutex)
{
    if (xSemaphoreGive(mutex) != pdTRUE) {
        return -30;
    }

    return 0;
}

int os_semaphore_create(uint32_t initValue, T_UAVSemaHandle *semaphore)
{
    uint32_t maxCount = UINT_MAX;

    *semaphore = xSemaphoreCreateCounting(maxCount, initValue);

    if (*semaphore == NULL) {
        return -30;
    }

    return 0;
}

int os_semaphore_destroy(T_UAVSemaHandle semaphore)
{
    vSemaphoreDelete(semaphore);

    return 0;
}

int os_semaphor_time_wait(T_UAVSemaHandle semaphore, uint32_t wait_time_ms)
{
    TickType_t ticks;

    if (semaphore == NULL) {
        return -21;
    }

    ticks = 0;
    if (wait_time_ms == SEM_MUTEX_WAIT_FOREVER) {
        ticks = portMAX_DELAY;
    } else if (wait_time_ms != 0) {
        ticks = wait_time_ms / portTICK_PERIOD_MS;
        if (ticks == 0) {
            ticks = 1;
        }
    }
    if (xSemaphoreTake(semaphore, ticks) != pdTRUE) {
        return -30;
    }

    return 0;
}

int os_semaphore_wait(T_UAVSemaHandle semaphore)
{
    os_semaphor_time_wait(semaphore, SEM_MUTEX_WAIT_FOREVER);

    return 0;
}

int os_semaphore_post(T_UAVSemaHandle semaphore)
{
    if (xSemaphoreGive(semaphore) != pdTRUE) {
        return -30;
    }

    return 0;
}

int os_get_time_ms(uint32_t *ms)
{
    *ms = xTaskGetTickCount();

    return 0;
}

int os_get_time_us(uint64_t *us)
{
    *us = xTaskGetTickCount() * 1000;

    return 0;
}

void *os_malloc(uint32_t size)
{
    return pvPortMalloc(size);
}

void os_free(void *ptr)
{
    vPortFree(ptr);
}

int os_ms_to_ticks(uint32_t ms) {
    return pdMS_TO_TICKS(ms);
}

void os_task_list(char *list)
{
    vTaskList(list);
}

uint32_t os_get_free_heap_size(void)
{
    return xPortGetFreeHeapSize();
}

/****************** (C) COPYRIGHT UAV Innovations *****END OF FILE****/
