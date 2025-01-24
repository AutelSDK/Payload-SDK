/**
 ********************************************************************
 * @file    test_fc_subscription.h
 * @brief   This is the header file for "test_fc_subscription.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2021 AUTEL. All rights reserved.
 *
 * All information contained herein is, and remains, the property of AUTEL.
 * The intellectual and technical concepts contained herein are proprietary
 * to AUTEL and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of AUTEL.
 *
 * If you receive this source code without AUTEL’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify AUTEL of its removal. AUTEL reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TEST_FC_SUBSCRIPTION_H
#define TEST_FC_SUBSCRIPTION_H

/* Includes ------------------------------------------------------------------*/
#include "uav_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
T_UAVReturnCode UAV_Test_FcSubscriptionStartService(void);
T_UAVReturnCode UAV_Test_FcSubscriptionRunSample(void);
T_UAVReturnCode UAV_Test_FcSubscriptionDataShowTrigger(void);
T_UAVReturnCode UAV_Test_FcSubscriptionGetTotalSatelliteNumber(uint8_t *number);

#ifdef __cplusplus
}
#endif

#endif // TEST_FC_SUBSCRIPTION_H
/************************ (C) COPYRIGHT AUTEL Innovations *******END OF FILE******/
