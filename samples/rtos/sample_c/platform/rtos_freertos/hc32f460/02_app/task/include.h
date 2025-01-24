 #ifndef __INCLUDE_H
 #define __INCLUDE_H
 
#ifndef USE_DDL_DRIVER
#define USE_DDL_DRIVER
#endif

#ifndef HC32F460
#define HC32F460
#endif

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
#include "bsp_led.h"

#include "usb_dev_user.h"
#include "usb_dev_desc.h"
#include "usb_dev_cdc_class.h"
#include "cdc_data_process.h"
 
 #endif
 