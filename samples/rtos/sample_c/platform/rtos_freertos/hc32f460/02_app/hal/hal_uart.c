/**
 ********************************************************************
 * @file    psdk_hal.c
 * @version V2.0.0
 * @date    2019/07/01
 * @brief
 *
 * @copyright (c) 2021 Uav. All rights reserved.
 *
 * All information contained herein is, and remains, the property of Uav.
 * The intellectual and technical concepts contained herein are proprietary
 * to Uav and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of Uav.
 *
 * If you receive this source code without Uav’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify Uav of its removal. Uav reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_uart.h"
#include "bsp_uart_dma.h"
#include "bsp_uart4_dma.h"	
#include "bsp.h"

#define USE_UART4
// #define USE_UART3
//extern debug_drv_t _dbg_drv;


/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/


/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
T_UAVReturnCode usbcdc_Init(void)
{
//	usb_cdc_bsp_init();
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode usbcdc_DeInit(void)
{
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode usbcdc_write(const uint8_t *buf, uint32_t len)
{
	vcp_txdata(buf, len);	
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode usbcdc_Read(uint8_t *buf, uint32_t len, uint32_t *realLen)
{
	*realLen=RingBuffer_read(&usbcdc_dev.rx_ring, buf, len);	
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode usbcdc_GetStatus(void)
{
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode usbcdc_SetBaudRate(uint32_t baudRate)
{
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}




T_UAVReturnCode uart_Init(void)
{
#ifdef USE_UART4
	bsp_uart4_init(115200);	
#else
	bsp_uart3_init(115200);	
#endif
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode uart_DeInit(void)
{
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode uart_write(const uint8_t *buf, uint32_t len)
{
#ifdef USE_UART4
	uart4_dma_tx((uint8_t *)buf, len);
#else
	uart3_dma_tx(buf, len);
#endif
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode uart_Read(uint8_t *buf, uint32_t len, uint32_t *realLen)
{
#ifdef USE_UART4
	*realLen = uart4_read(buf, len);
#else
	*realLen = uart3_read(buf, len);
#endif
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode uart_SetBaudRate(uint32_t baudRate)
{
#ifdef USE_UART4
	bsp_uart4_init(baudRate);	
#else
	bsp_uart3_init(baudRate);	
#endif
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
T_UAVReturnCode uart_GetStatus(void)
{
	return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT Uav Innovations *****END OF FILE****/
