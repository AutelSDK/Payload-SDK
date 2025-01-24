
#include "test_time_sync.h"
#include "test_fc_subscription.h"
#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"
#include "uav_time_sync.h"
/* Private constants ---------------------------------------------------------*/
#define UAV_TEST_TIME_SYNC_TASK_FREQ            (1)
#define UAV_TEST_TIME_SYNC_TASK_STACK_SIZE      (256*4)
#define UAV_TEST_TIME_SYNC_TASK_PRIORITY        (1)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *UAV_Test_TimeSyncTask(void *arg);
static void *UAV_Test_NtpTimeSyncTask(void *arg);
/* Private variables ---------------------------------------------------------*/
static T_UAVTestTimeSyncHandler s_timeSyncHandler;
static T_UAVTaskHandle s_timeSyncThread;
static T_UAVTaskHandle s_ntptimeSyncThread;

/* Exported functions definition ---------------------------------------------*/
/**
 * @brief Register handler function for initialising PPS pin configure and reporting the latest local time when PPS is
 * triggered. This function have to be called before calling UAV_Test_TimeSyncInit().
 * @param timeSyncHandler: pointer to handler function for time synchronization.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Test_TimeSyncRegHandler(T_UAVTestTimeSyncHandler *timeSyncHandler)
{
    if (timeSyncHandler->PpsSignalResponseInit == NULL) {
        UAV_LOG_ERROR("reg time sync handler PpsSignalResponseInit error");
        return UAV_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (timeSyncHandler->GetNewestPpsTriggerLocalTimeUs == NULL) {
        UAV_LOG_ERROR("reg time sync handler GetNewestPpsTriggerLocalTimeUs error");
        return UAV_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    memcpy(&s_timeSyncHandler, timeSyncHandler, sizeof(T_UAVTestTimeSyncHandler));

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode UAV_Test_TimeSyncNTPStartService(void)
{
    T_UAVReturnCode uavStat;
    T_UAVOsalHandler *os_handler = nullptr;
    int64_t timestamp = 0;
    UAV_LOG_INFO("==========ntp_time_sync_routine==========");
    uavStat = UAV_TimeSync_Init();
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("UAV_TimeSync_Init failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    uavStat =  UAV_TimeSync_SetType(UAV_TIME_SYNC_TYPE_NTP);
    if(uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("UAV_TimeSync_SetType failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    os_handler = UAV_Platform_GetOsalHandler();
    os_handler->TaskCreate("user_time_sync_task", 
                                UAV_Test_NtpTimeSyncTask,
                                UAV_TEST_TIME_SYNC_TASK_STACK_SIZE, 
                                NULL, 
                                UAV_TEST_TIME_SYNC_TASK_PRIORITY,
                                &s_ntptimeSyncThread) ;
    if(s_ntptimeSyncThread == nullptr)
    {
        UAV_LOG_ERROR("user time sync task create error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

}

T_UAVReturnCode UAV_Test_TimeSyncPpsStartService(void)
{
    T_UAVReturnCode uavStat;
    T_UAVOsalHandler *os_handler = nullptr;
    int64_t timestamp = 0;
    uavStat = UAV_TimeSync_Init();
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("UAV_TimeSync_Init failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    uavStat =  UAV_TimeSync_SetType(UAV_TIME_SYNC_TYPE_PPS_CLOCK);
    if(uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("UAV_TimeSync_SetType failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (s_timeSyncHandler.PpsSignalResponseInit == NULL) {
        UAV_LOG_ERROR("time sync handler PpsSignalResponseInit interface is NULL error");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs == NULL) {
        UAV_LOG_ERROR("time sync handler GetNewestPpsTriggerLocalTimeUs interface is NULL error");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    // users must register getNewestPpsTriggerTime callback function
    uavStat = UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback(s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("register GetNewestPpsTriggerLocalTimeUsCallback error.");
        return uavStat;
    }
    os_handler = UAV_Platform_GetOsalHandler();
    os_handler->TaskCreate("user_time_sync_task", 
                                UAV_Test_TimeSyncTask,
                                UAV_TEST_TIME_SYNC_TASK_STACK_SIZE, 
                                NULL, 
                                UAV_TEST_TIME_SYNC_TASK_PRIORITY,
                                &s_timeSyncThread) ;
    if(s_timeSyncThread == nullptr)
    {
        UAV_LOG_ERROR("user time sync task create error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    uavStat = s_timeSyncHandler.PpsSignalResponseInit();
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("pps signal response init error");
        return uavStat;
    }

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

}


T_UAVReturnCode UAV_Test_TimeSyncGetNewestPpsTriggerLocalTimeUs(uint64_t *localTimeUs)
{
    if (s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs == NULL) {
        UAV_LOG_ERROR("GetNewestPpsTriggerLocalTimeUs null error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    return s_timeSyncHandler.GetNewestPpsTriggerLocalTimeUs(localTimeUs);
}

/* Private functions definition-----------------------------------------------*/
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif
static void *UAV_Test_NtpTimeSyncTask(void *arg)
{
    T_UAVReturnCode uavStat;
    uint32_t currentTimeMs = 0;
    T_UAVTimeSyncAircraftTime aircraftTime = {0};
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    while(1)
    {
        os_handler->TaskSleepMs(1000 / UAV_TEST_TIME_SYNC_TASK_FREQ);

        uavStat = os_handler->GetTimeMs(&currentTimeMs);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("get current time error: 0x%08llX.", uavStat);
            continue;
        }

        uavStat = UAV_TimeSync_TransferToAircraftTime(currentTimeMs * 1000, &aircraftTime);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("transfer to aircraft time error: 0x%08llX.", uavStat);
            continue;
        }

        UAV_LOG_INFO("current aircraft time is %04d-%02d-%02d %02d:%02d:%02d %d.",
                       aircraftTime.year, aircraftTime.month, aircraftTime.day,
                       aircraftTime.hour, aircraftTime.minute, aircraftTime.second, aircraftTime.microsecond);
    }
}

static void *UAV_Test_TimeSyncTask(void *arg)
{
    T_UAVReturnCode uavStat;
    uint32_t currentTimeMs = 0;
    T_UAVTimeSyncAircraftTime aircraftTime = {0};
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    uint8_t totalSatelliteNumber = 0;


    while (1) {
        os_handler->TaskSleepMs(1000 / UAV_TEST_TIME_SYNC_TASK_FREQ);

        uavStat = UAV_Test_FcSubscriptionGetTotalSatelliteNumber(&totalSatelliteNumber);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("get total satellite number error: 0x%08llX.", uavStat);
            continue;
        }

        uavStat = os_handler->GetTimeMs(&currentTimeMs);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("get current time error: 0x%08llX.", uavStat);
            continue;
        }

        uavStat = UAV_TimeSync_TransferToAircraftTime(currentTimeMs * 1000, &aircraftTime);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("transfer to aircraft time error: 0x%08llX.", uavStat);
            continue;
        }

        UAV_LOG_INFO("current aircraft time is %04d-%02d-%02d %02d:%02d:%02d %d.",
                       aircraftTime.year, aircraftTime.month, aircraftTime.day,
                       aircraftTime.hour, aircraftTime.minute, aircraftTime.second, aircraftTime.microsecond);
    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

/****************** (C) COPYRIGHT AUTEL Innovations *****END OF FILE****/