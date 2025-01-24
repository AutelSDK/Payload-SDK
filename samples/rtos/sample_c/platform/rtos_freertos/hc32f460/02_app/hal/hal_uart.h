/**
 ********************************************************************
 * @file    hal.h
 * @version V2.0.0
 * @date    2019/8/30
 * @brief   This is the header file for "hal.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2021 UAV. All rights reserved.
 *
 * All information contained herein is, and remains, the property of UAV.
 * The intellectual and technical concepts contained herein are proprietary
 * to UAV and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of UAV.
 *
 * If you receive this source code without UAV’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify UAV of its removal. UAV reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_H
#define HAL_H

/* Includes ------------------------------------------------------------------*/
#include "uav_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/




/* Exported functions --------------------------------------------------------*/
T_UAVReturnCode usbcdc_Init(void);
T_UAVReturnCode usbcdc_DeInit(void);
T_UAVReturnCode usbcdc_write(const uint8_t *buf, uint32_t len);
T_UAVReturnCode usbcdc_Read(uint8_t *buf, uint32_t len, uint32_t *realLen);
T_UAVReturnCode usbcdc_SetBaudRate(uint32_t baudRate);
T_UAVReturnCode usbcdc_GetStatus(void);

T_UAVReturnCode uart_Init(void);
T_UAVReturnCode uart_DeInit(void);
T_UAVReturnCode uart_write(const uint8_t *buf, uint32_t len);
T_UAVReturnCode uart_Read(uint8_t *buf, uint32_t len, uint32_t *realLen);
T_UAVReturnCode uart_SetBaudRate(uint32_t baudRate);
T_UAVReturnCode uart_GetStatus(void);


#ifdef __cplusplus
}
#endif

#endif // HAL_H
/************************ (C) COPYRIGHT UAV Innovations *******END OF FILE******/
