/**
 ********************************************************************
 * @file    test_positioning.c
 * @brief
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

#include "test_positioning.h"
#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"

#define UAV_POSITIONING_TASK_STACK_SIZE   (1024*2)
#define UAV_POSITIONING_TASK_PRIORITY     1

static T_UAVTaskHandle s_positioningTaskHandle = NULL;
static T_UAVOsalHandler *os_handler = nullptr;

static void *UAV_PositioningTask(void *arg);
/* Exported functions definition ---------------------------------------------*/
T_UAVReturnCode UAV_Test_PositioningStartService(void)
{
    T_UAVReturnCode UavStat;
    UAV_LOG_INFO("============positioning_routine============");
    UavStat = UAV_Positioning_Init();
    if(UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS != UavStat)
    {
        UAV_LOG_ERROR("UAV_Positioning_Init failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    os_handler = UAV_Platform_GetOsalHandler();
    if(os_handler == nullptr)
    {
        UAV_LOG_ERROR("os_handler 0");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    os_handler->TaskCreate("UAV_Test_PositioningTask", 
                            UAV_PositioningTask, 
                            UAV_POSITIONING_TASK_STACK_SIZE, 
                            NULL, 
                            UAV_POSITIONING_TASK_PRIORITY, 
                            &s_positioningTaskHandle);
    if(s_positioningTaskHandle == NULL)
    {
        UAV_LOG_ERROR("UAV_Test_PositioningTask create failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static void *UAV_PositioningTask(void *arg)
{
    T_UAVPositioningPositionInfo positioningData = {0};
    T_UAVReturnCode uavStat = UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    T_UAVPositioningEventInfo eventInfo[2] = {0};
    T_UAVPositioningPositionInfo positionInfo[2] = {0};
    eventInfo[0].eventIndex =0;
    eventInfo[0].timestamp.year =2024;
    eventInfo[0].timestamp.month =5;
    eventInfo[0].timestamp.day =29;
    eventInfo[0].timestamp.hour =9;
    eventInfo[0].timestamp.minute =20;
    eventInfo[0].timestamp.second =59;
    eventInfo[0].timestamp.microsecond=500;
    eventInfo[1].eventIndex =1;
    eventInfo[1].timestamp.year =2024;
    eventInfo[1].timestamp.month =5;
    eventInfo[1].timestamp.day =29;
    eventInfo[1].timestamp.hour =9;
    eventInfo[1].timestamp.minute =20;
    eventInfo[1].timestamp.second =60;
    eventInfo[1].timestamp.microsecond=500;
    while(1)
    {
        uavStat = UAV_Positioning_GetPositioning_Sync(2, &eventInfo[0], &positioningData);
        if(UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS != uavStat)
        {
            UAV_LOG_ERROR("get positioning sync failed");
        }
        else 
        {
            UAV_LOG_INFO("get positioning sync success, %d, [%f,%f,%f], [%f,%f,%f]", 
                            positioningData.property, 
                            positioningData.uavAttitude.roll, positioningData.uavAttitude.pitch, positioningData.uavAttitude.yaw,
                            positioningData.positionDeviation.longitude, positioningData.positionDeviation.latitude, positioningData.positionDeviation.altitude);
        }
        os_handler->TaskSleepMs(2000);
    }

    UAV_Positioning_Deinit();
    return 0;
}
/************************ (C) COPYRIGHT UAV Innovations *******END OF FILE******/