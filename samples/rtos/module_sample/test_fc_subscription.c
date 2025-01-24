#include "test_fc_subscription.h"
#include "uav_fc_subscription.h"
#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"



/* Private constants ---------------------------------------------------------*/
#define FC_SUBSCRIPTION_TASK_FREQ         (1)
#define FC_SUBSCRIPTION_TASK_STACK_SIZE   (5*256)
#define FC_SUBSCRIPTION_TASK_PRIORITY     (1)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static T_UAVTaskHandle s_userFcSubscriptionThread;
static bool s_userFcSubscriptionDataShow = false;
static uint8_t s_totalSatelliteNumberUsed = 0;
static uint32_t s_userFcSubscriptionDataCnt = 0;

static void UAV_Test_FcSubscriptionTask(void *arg);

static void myPositionFusedMsgCB(void *data, int len)
{
    T_UAVSubscriptionPositionFused *posSt =  (T_UAVSubscriptionPositionFused*)data;
    UAV_LOG_DEBUG("longitude:%lf, latitude:%f, altitude:%f, visibleSatelliteNumber:%f", posSt->longitude, posSt->latitude, posSt->altitude, posSt->visibleSatelliteNumber);
}

static void myPositionGpsMsgCB(void *data, int len)
{
    T_UAVSubscriptionGpsPosition *posSt =  (T_UAVSubscriptionGpsPosition*)data;
    UAV_LOG_DEBUG("longitude:%d, latitude:%d, altitude:%d, visibleSatelliteNumber:%d", posSt->x, posSt->y, posSt->z);
}

static void myEulerAngularVelocityMsgCB(void *data, int len)
{
    T_UAVSubscriptionAngularRateRaw *eulerSt =  (T_UAVSubscriptionAngularRateRaw*)data;
    UAV_LOG_DEBUG("wroll:%f, wpitch:%f, wyaw:%f", eulerSt->x, eulerSt->y, eulerSt->z);
}

static void myVelocityComponentMsgCB(void *data, int len)
{
    T_UAVSubscriptionGpsVelocity *velCompSt =  (T_UAVSubscriptionGpsVelocity*)data;
    UAV_LOG_DEBUG("vX:%f, vY:%f, vZ:%f", velCompSt->x, velCompSt->y, velCompSt->z);
}

static void myEulerAngularMsgCB(void *data, int len)
{
    T_UAVSubscriptionEulerAngular *eulerSt =  (T_UAVSubscriptionEulerAngular*)data;
    UAV_LOG_DEBUG("roll:%f, pitch:%f, yaw:%f", eulerSt->roll, eulerSt->pitch, eulerSt->yaw);
}
static void myFlightStatusMsgCB(void *data, int len)
{
    T_UAVSubscriptionFlightStatus *flightStatus  =  (T_UAVSubscriptionFlightStatus*)data;
    UAV_LOG_DEBUG("flightStatus:%d", flightStatus);
}
static void myBatteryInfoMsgCB(void *data, int len)
{
    T_UAVSubscriptionWholeBatteryInfo *batteryInfo = (T_UAVSubscriptionWholeBatteryInfo*)data;
    UAV_LOG_DEBUG("batt: %d%%, %dmV, %dmA, %dmAH,",batteryInfo->percentage, batteryInfo->voltage, batteryInfo->current, batteryInfo->capacity);

}

/* Exported functions definition ---------------------------------------------*/
T_UAVReturnCode UAV_Test_FcSubscriptionStartService(void)
{
    T_UAVReturnCode uavStat;
    T_UAVOsalHandler *os_handler = NULL;
    UAV_LOG_INFO("============fc_subscription_routine============");
    os_handler = UAV_Platform_GetOsalHandler();
    uavStat = UAV_Subscription_Init();
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("init data subscription module error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    uavStat = UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_BATTERY_INFO, UAV_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               myBatteryInfoMsgCB);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("Subscribe topic battery info error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        UAV_LOG_INFO("Subscribe topic battery info success.");
    }
    uavStat = UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY, UAV_DATA_SUBSCRIPTION_TOPIC_50_HZ,
                                               myVelocityComponentMsgCB);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("Subscribe topic velocity error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        UAV_LOG_INFO("Subscribe topic velocity success.");
    }


    uavStat = UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_POSITION, UAV_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               myPositionGpsMsgCB);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("Subscribe topic gps position error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        UAV_LOG_INFO("Subscribe topic gps position success.");
    }

    uavStat = UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED, UAV_DATA_SUBSCRIPTION_TOPIC_1_HZ,
                                               myPositionFusedMsgCB);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("Subscribe topic position fused error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        UAV_LOG_INFO("Subscribe topic position fused success.");
    }
    UAV_LOG_INFO("subscribe success.");

    os_handler->TaskCreate("UAV_Test_FcSubscriptionTask", 
                            UAV_Test_FcSubscriptionTask, 
                            FC_SUBSCRIPTION_TASK_STACK_SIZE, 
                            NULL, 
                            FC_SUBSCRIPTION_TASK_PRIORITY, 
                            &s_userFcSubscriptionThread);
    if(s_userFcSubscriptionThread == NULL)
    {
        UAV_LOG_ERROR("UAV_Test_FcSubscriptionTask create failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    } else {
        UAV_LOG_DEBUG("UAV_Test_FcSubscriptionTask create success.");
    }

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static void UAV_Test_FcSubscriptionTask(void *arg)
{
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    T_UAVReturnCode uavStat;
    while(1)
    {
        os_handler->TaskSleepMs(1000*60);
        uavStat = UAV_unSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("unSubscribe topic gps velocity error.");
        } else {
            UAV_LOG_INFO("unSubscribe topic gps velocity success.");
        }

        uavStat = UAV_unSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_BATTERY_INFO);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("unSubscribe topic battery info error.");
        } else {
            UAV_LOG_INFO("unSubscribe topic battery info success.");
        }

        uavStat = UAV_unSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_POSITION);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("unSubscribe topic gps position error.");
        } else {
            UAV_LOG_INFO("unSubscribe topic gps position success.");
        }

        uavStat = UAV_unSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED);
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_ERROR("unSubscribe topic position fused error.");
        } else {
            UAV_LOG_INFO("unSubscribe topic position fused success.");
        }

        os_handler->TaskDestroy(s_userFcSubscriptionThread);
    }
}
T_UAVReturnCode UAV_Test_FcSubscriptionDataShowTrigger(void)
{
    s_userFcSubscriptionDataShow = !s_userFcSubscriptionDataShow;

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode UAV_Test_FcSubscriptionGetTotalSatelliteNumber(uint8_t *number)
{
    *number = s_totalSatelliteNumberUsed;

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}



/****************** (C) COPYRIGHT AUTEL Innovations *****END OF FILE****/
