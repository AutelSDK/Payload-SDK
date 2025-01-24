/*
 * @Author: kevin && R22006/uavrobotics.cn
 * @Date: 2023-10-24 15:58:00
 * @LastEditors: kevin && R22006/uavrobotics.cn
 * @LastEditTime: 2023-10-24 17:32:54
 * @Description: 
 * Copyright (c) 2023 Uav Robotics. All rights reserved.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <lib_crc32.h>
#include <lib_crc16.h>
#include "lib_fifo.h"

#include "hc32_ll.h"
#include "cmsis_os.h"

#include "bsp.h"
#include "bsp_adc.h"
#include "bsp_gpio.h"
#include "bsp_flash.h"
#include "usb_dev_user.h"
#include "usb_dev_desc.h"
#include "usb_dev_cdc_class.h"
#include "cdc_data_process.h"

#include "application.h"
#include "uav_platform.h"
#include "uav_logger.h"

#include "upgApplication.h"


/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

static void start_thread(void const *argument);
static void uart_shake_test(void const *argument);
static void factory_test_task(void const *argument);
static void log_test_task(void const *argument);

int32_t main(void)
{
    osThreadId start_thread_handle = NULL;
#ifdef __BOOT
	SCB->VTOR = FLASH_BOOTLOADER_START_ADDR;
#else
    upg_set_Vector_Table();
#endif
    bsp_init();
    /** application task */
    osThreadDef(start, UAV_User_StartTask, osPriorityNormal, 0,  2 * 256);
    start_thread_handle = osThreadCreate(osThread(start), NULL);	
    osKernelStart();
    for(;;);
}
