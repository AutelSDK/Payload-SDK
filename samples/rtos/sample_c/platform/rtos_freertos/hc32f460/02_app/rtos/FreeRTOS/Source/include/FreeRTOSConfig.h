/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
 #include <stdint.h>
 #include <stdio.h>
// #include "bsp_serial.h"
 #include "bsp_tim.h"
 extern uint32_t SystemCoreClock;
#endif
 #include "uav_logger.h"
/***************************************************************************************************************/
/*                                           FreeRTOS ����                                                     */
/***************************************************************************************************************/
#define vAssertCalled(char,int) 	UAV_LOG_DEBUG("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) 	vAssertCalled(__FILE__,__LINE__)

/***************************************************************************************************************/
/*                                        FreeRTOS������������ѡ��                                             */
/***************************************************************************************************************/
#define configUSE_PREEMPTION                    1							// 1ʹ����ռʽ�ںˣ�0ʹ��Э��
#define configUSE_TIME_SLICING					1							// 1ʹ��ʱ��Ƭ����(Ĭ��ʽʹ�ܵ�)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1                       	// 1�������ⷽ����ѡ����һ��Ҫ���е�����,һ����Ӳ������ǰ����ָ������ʹ�õ�
																			// MCUû����ЩӲ��ָ��Ļ��˺�Ӧ������Ϊ0��

#define configAPPLICATION_ALLOCATED_HEAP		0							// 1ʹ�ø������ö�ջ�ռ�
#define configUSE_TICKLESS_IDLE					0                       	// 1���õ͹���ticklessģʽ
#define configUSE_QUEUE_SETS					1                       	// Ϊ1ʱ���ö���
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )			// CPUƵ��
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )		// ʱ�ӽ���Ƶ�ʣ���������Ϊ1000�����ھ���1ms
#define configMAX_PRIORITIES                    ( 16 )						// ��ʹ�õ�������ȼ�
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 128 )		// ��������ʹ�õĶ�ջ��С
#define configMAX_TASK_NAME_LEN                 ( 32 )						// ���������ַ�������

#define configUSE_16_BIT_TICKS                  0							// ϵͳ���ļ����������������ͣ�1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configIDLE_SHOULD_YIELD                 1							// Ϊ1ʱ�����������CPUʹ��Ȩ������ͬ���ȼ����û�����
#define configUSE_TASK_NOTIFICATIONS            1                       	// Ϊ1ʱ��������֪ͨ���ܣ�Ĭ�Ͽ���

#define configUSE_MUTEXES                       1							// Ϊ1ʱʹ�û����ź���
#define configQUEUE_REGISTRY_SIZE               90							// ��Ϊ0ʱ��ʾ���ö��м�¼�������ֵ�ǿ��Լ�¼�Ķ��к��ź��������Ŀ��
#define configCHECK_FOR_STACK_OVERFLOW          0							// ����0ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹���,�û������ṩһ��ջ������Ӻ��������ʹ�õĻ� // ��ֵ����Ϊ1����2����Ϊ������ջ�����ⷽ���� #define configUSE_RECURSIVE_MUTEXES             0							// Ϊ1ʱʹ�õݹ黥���ź���
#define configUSE_MALLOC_FAILED_HOOK            0							// 1ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1							// Ϊ1ʱʹ�ü����ź���

/***************************************************************************************************************/
/*                                FreeRTOS���ڴ������й�����ѡ��                                                */
/***************************************************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       	// ֧�ֶ�̬�ڴ�����

#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
#define configTOTAL_HEAP_SIZE					((size_t)(128*1024))        // ϵͳ�����ܵĶѴ�С
#else
//#define configTOTAL_HEAP_SIZE					((size_t)(12*1024))    		// ϵͳ�����ܵĶѴ�С
#define configTOTAL_HEAP_SIZE					((size_t)(64*1024))    		// ϵͳ�����ܵĶѴ�С
#endif

/***************************************************************************************************************/
/*                                FreeRTOS�빳�Ӻ����йص�����ѡ��                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						1                       	// 1��ʹ�ÿ��й��ӣ�0����ʹ��
#define configUSE_TICK_HOOK						1                       	// 1��ʹ��ʱ��Ƭ���ӣ�0����ʹ��

/***************************************************************************************************************/
/*                                FreeRTOS������ʱ�������״̬�ռ��йص�����ѡ��                                 */
/***************************************************************************************************************/
#define configGENERATE_RUN_TIME_STATS	        1                       //Ϊ1ʱ��������ʱ��ͳ�ƹ���
#define configUSE_TRACE_FACILITY				1                       //Ϊ1���ÿ��ӻ����ٵ���
#define configUSE_STATS_FORMATTING_FUNCTIONS	1                       //���configUSE_TRACE_FACILITYͬʱΪ1ʱ���������3������
                                                                        //prvWriteNameToBuffer(),vTaskList(),
                                                                        //vTaskGetRunTimeStats()
#if configGENERATE_RUN_TIME_STATS
	#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()	bsp_tim4_init();    //ÿ������ʹ�����
	#define portGET_RUN_TIME_COUNTER_VALUE()	        RTOS_GetRunTimeBase()
#endif

/***************************************************************************************************************/
/*                                FreeRTOS��Э���йص�����ѡ��                                                 */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/***************************************************************************************************************/
/*                                FreeRTOS��������ʱ���йص�����ѡ��                                           */
/***************************************************************************************************************/
/* Software timer definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( 2 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )

/***************************************************************************************************************/
/*                                FreeRTOS��ѡ��������ѡ��                                                     */
/***************************************************************************************************************/
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xQueueGetMutexHolder            1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_eTaskGetState                   1

/***************************************************************************************************************/
/*                                FreeRTOS���ж��йص�����ѡ��                                                 */
/***************************************************************************************************************/
#ifdef __NVIC_PRIO_BITS
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS                        __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS                        4        /* 15 priority levels */
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     	0xf				//�ж�������ȼ�
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  	2				//ϵͳ�ɹ���������ж����ȼ�
#define configKERNEL_INTERRUPT_PRIORITY     ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/***************************************************************************************************************/
/*                                FreeRTOS���жϷ������йص�����ѡ��                                         */
/***************************************************************************************************************/
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
/* #define xPortSysTickHandler SysTick_Handler */

/***************************************************************************************************************/
/*                     FreeRTOS���CPUʹ����ͳ������ѡ��      ����configUSE_IDLE_HOOK                          */
/***************************************************************************************************************/
#define traceTASK_SWITCHED_IN()  extern void StartIdleMonitor(void); \
                                         StartIdleMonitor()
#define traceTASK_SWITCHED_OUT() extern void EndIdleMonitor(void); \
                                         EndIdleMonitor()

#endif /* FREERTOS_CONFIG_H */

