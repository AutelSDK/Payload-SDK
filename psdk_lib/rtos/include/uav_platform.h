#ifndef _UAV_PLATFORM_H_
#define _UAV_PLATFORM_H_

#include "uav_typedef.h"
#include "uav_error.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    UAV_HAL_UART_NUM_0 = 0,
    UAV_HAL_UART_NUM_1,
}E_UAVHalUartNum;

typedef enum 
{
	UAV_BAUDRATE_115200=0,
	UAV_BAUDRATE_19200,
	UAV_BAUDRATE_230400,
	UAV_BAUDRATE_460800,
	UAV_BAUDRATE_1000000,
	UAV_BAUDRATE_2000000,
	UAV_BAUDRATE_MAX,
}E_UAVHalUartBaudRate;

/**
 * @brief Platform handle of uart status.
 */
typedef struct 
{
    bool isConnect;	
	
}T_UAVUartStatus;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} T_UAVTime;
/**
* @brief Platform handle of thread task operation.
*/
typedef void *T_UAVTaskHandle;

/**
* @brief Platform handle of mutex operation.
*/
typedef void *T_UAVMutexHandle;

/**
* @brief Platform handle of semaphore operation.
*/
typedef void *T_UAVSemaHandle;

/**
 * @brief Platform handle of timer operation.
 */
typedef void *T_UAVTimerHandle;

typedef struct {
    int (*TaskCreate)(const char *name, void *(*taskFunc)(void *), uint32_t stackSize, 
                            void *arg, uint32_t priority,T_UAVTaskHandle *task);

    int (*TaskStart)(T_UAVTaskHandle task);

    int (*TaskDestroy)(T_UAVTaskHandle task);

    int (*TaskSleepMs)(uint32_t time_ms);

    int (*MutexCreate)(T_UAVMutexHandle *mutex);

    int (*MutexDestroy)(T_UAVMutexHandle mutex);

    int (*MutexLock)(T_UAVMutexHandle mutex);

    int (*MutexUnlock)(T_UAVMutexHandle mutex);

    int (*SemaphoreCreate)(uint32_t initValue, T_UAVSemaHandle *semaphore);

    int (*SemaphoreDestroy)(T_UAVSemaHandle semaphore);

    int (*SemaphoreWait)(T_UAVSemaHandle semaphore);

    int (*SemaphoreTimedWait)(T_UAVSemaHandle semaphore, uint32_t wait_time_ms);

    int (*SemaphorePost)(T_UAVSemaHandle semaphore);

    int (*GetTimeMs)(uint32_t *ms);

    int (*GetTimeUs)(uint64_t *us);

    void *(*Malloc)(uint32_t size);

    void (*Free)(void *ptr);

    int (*MsToTicks)(uint32_t ms);

    void (*TaskList)(char *list);

    uint32_t (*FreeHeapSize)(void);

} T_UAVOsalHandler;

/**
 * @brief Platform handle of uart operation.
 */
typedef struct
{
	T_UAVReturnCode (*init)(void);
	T_UAVReturnCode (*deInit)(void);
	T_UAVReturnCode (*write)(const uint8_t *buf, uint32_t len);
	T_UAVReturnCode (*read)(uint8_t *buf, uint32_t len, uint32_t *realLen);
    T_UAVReturnCode (*setBaudRate)(uint32_t baudRate);
	T_UAVReturnCode (*getStatus)(void);
	uint32_t baudRate;
	
}T_UAVHalUartHandler;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Register the handler for hal uart interfaces by your platform.
 * @note It should be noted that the interface in hal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration. The interface needs to be called at the beginning of
 * the application for registration, otherwise, the subsequent functions will not work properly.
 * @param halUartHandler: pointer to the handler for hal uart interfaces by your platform.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Platform_RegHalUartHandler(const T_UAVHalUartHandler *halUartHandler);

/**
 * @brief Register the handler for osal interfaces by your platform.
 * @note It should be noted that the interface in osal is written and tested well. Users need to implement all the
 * interfaces. Otherwise, the user interface cannot be successfully registered, and then the user interface is registered
 * through the interface. If the registration fails, it needs to be based on the return code. To judge the problem. Make
 * sure that the feature is available after a successful registration. The interface needs to be called at the beginning of
 * the application for registration, otherwise, the subsequent functions will not work properly.
 * @param os_handler: pointer to the handler for osal interfaces by your platform.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Platform_RegOsalHandler(const T_UAVOsalHandler *os_handler);

/**
 * @brief Get the handler of osal interfaces.
 * @return Pointer to osal handler.
 */
T_UAVOsalHandler *UAV_Platform_GetOsalHandler(void);
/**
 * @brief Get the handler of uart interfaces.
 * @return Pointer to uart handler.
 */
T_UAVHalUartHandler *UAV_Platform_GetHalUartHandler(void);


#ifdef __cplusplus
}
#endif
#endif
