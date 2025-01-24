/**
 ********************************************************************
 * @file    test_power_management.c
 * @brief
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


/* Includes ------------------------------------------------------------------*/
#include "test_power_management.h"
#include "uav_power_management.h"
#include "uav_logger.h"

/* Private constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static T_UAVReturnCode UAV_Test_PowerOffNotificationCallback(bool *powerOffPreparationFlag);

/* Private variables ---------------------------------------------------------*/
static T_UAVTestApplyHighPowerHandler s_applyHighPowerHandler;

/* Exported functions definition ---------------------------------------------*/

/**
 * @brief Register handler function for applying high power. This function have to be called before calling
 * UAV_Test_PowerManagementInit(), except for in Linux, because UAV_Test_PowerManagementInit() do not apply high power
 * in Linux OS.
 * @param applyHighPowerHandler: pointer to handler function for applying high power.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Test_RegApplyHighPowerHandler(T_UAVTestApplyHighPowerHandler *applyHighPowerHandler)
{
    if (applyHighPowerHandler->pinInit == NULL) {
        UAV_LOG_ERROR("reg apply high power handler pinInit error");
        return UAV_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (applyHighPowerHandler->pinWrite == NULL) {
        UAV_LOG_ERROR("reg apply high power handler pinWrite error");
        return UAV_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    memcpy(&s_applyHighPowerHandler, applyHighPowerHandler, sizeof(T_UAVTestApplyHighPowerHandler));

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/**
 * @brief Initialise power management module, including apply high power (only RTOS) and register power off notification
 * callback function.
 * @note UAV development board 1.0 can not accept high power, so do not call this function in UAV development board
 * 1.0 project.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Test_PowerManagementStartService(void)
{
    T_UAVReturnCode returnCode;
    // T_UAVAircraftInfoBaseInfo baseInfo = {0};

    returnCode = UAV_PowerManagement_Init();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("power management init error: 0x%08llX.", returnCode);
        return returnCode;
    }


    /*if (((baseInfo.aircraftType == UAV_AIRCRAFT_TYPE_M300_RTK || baseInfo.aircraftType == UAV_AIRCRAFT_TYPE_M350_RTK) &&
        (baseInfo.UAVAdapterType == UAV_SDK_ADAPTER_TYPE_SKYPORT_V2 || baseInfo.UAVAdapterType == UAV_SDK_ADAPTER_TYPE_XPORT)) ||
         baseInfo.aircraftType == UAV_AIRCRAFT_TYPE_FC30)*/ {
        // apply high power
        if (s_applyHighPowerHandler.pinInit == NULL) {
            UAV_LOG_ERROR("apply high power pin init interface is NULL error");
            return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }

        if (s_applyHighPowerHandler.pinWrite == NULL) {
            UAV_LOG_ERROR("apply high power pin write interface is NULL error");
            return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
        }

        returnCode = s_applyHighPowerHandler.pinInit();
        if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("apply high power pin init error");
            return returnCode;
        }

        returnCode = UAV_PowerManagement_RegWriteHighPowerApplyPinCallback(s_applyHighPowerHandler.pinWrite);
        if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("register WriteHighPowerApplyPinCallback error.");
            return returnCode;
        }

        returnCode = UAV_PowerManagement_ApplyHighPowerSync();
        if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("apply high power error");
            return returnCode;
        }
    }

    // register power off notification callback function
    returnCode = UAV_PowerManagement_RegPowerOffNotificationCallback(UAV_Test_PowerOffNotificationCallback);
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("register power off notification callback function error");
        return returnCode;
    }

    UAV_LOG_INFO("power management service start success.");

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode UAV_Test_PowerManagementStopService(void)
{
    T_UAVReturnCode returnCode;

    UAV_LOG_INFO("==========power_management_routine==========");

    returnCode = UAV_PowerManagement_DeInit();

    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("power management deinit error: 0x%08llX.", returnCode);
        return returnCode;
    }

    return returnCode;
}

/* Private functions definition-----------------------------------------------*/
static T_UAVReturnCode UAV_Test_PowerOffNotificationCallback(bool *powerOffPreparationFlag)
{
    UAV_LOG_INFO("aircraft will power off soon.");

    *powerOffPreparationFlag = true;

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/****************** (C) COPYRIGHT UAV Innovations *****END OF FILE****/
