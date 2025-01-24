#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_sdk_app_info.h"
#include "uav_fc_subscription.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


static void myPositionFusedMsgCB(void *data)
{
    T_UAVSubscriptionPositionFused *posSt =  (T_UAVSubscriptionPositionFused*)data;
    LOG_INFO("longitude:{}, latitude:{}, altitude:{}, visibleSatelliteNumber:{}", posSt->longitude, posSt->latitude, posSt->altitude, posSt->visibleSatelliteNumber);
}

static void myPositionGpsMsgCB(void *data)
{
    T_UAVSubscriptionGpsPosition *posSt =  (T_UAVSubscriptionGpsPosition*)data;
    LOG_INFO("longitude:{}, latitude:{}, altitude:{}", posSt->x, posSt->y, posSt->z);
}

static void myEulerAngularVelocityMsgCB(void *data)
{
    T_UAVSubscriptionAngularRateRaw *eulerSt =  (T_UAVSubscriptionAngularRateRaw*)data;
    LOG_INFO("wroll:{}, wpitch:{}, wyaw:{}", eulerSt->x, eulerSt->y, eulerSt->z);
}

static void myVelocityComponentMsgCB(void *data)
{
    T_UAVSubscriptionGpsVelocity *velCompSt =  (T_UAVSubscriptionGpsVelocity*)data;
    LOG_INFO("vX:{}, vY:{}, vZ:{}", velCompSt->x, velCompSt->y, velCompSt->z);
}

static void myEulerAngularMsgCB(void *data)
{
    T_UAVSubscriptionEulerAngular *eulerSt =  (T_UAVSubscriptionEulerAngular*)data;
    LOG_INFO("roll:{}, pitch:{}, yaw:{}", eulerSt->roll, eulerSt->pitch, eulerSt->yaw);
}

static void sub_routine(void)
{
    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    {
        // 测试 UAV_FcSubscription_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_FcSubscription_Init 函数");
        T_UAVReturnCode iRet =UAV_FcSubscription_Init();
        CHECK_TEST("UAV_FcSubscription_Init iRet:{}",iRet);
        LOG_INFO("UAV_FcSubscription_Init iRet:{}",iRet);
    }

    {
        // 测试 UAV_Waypoint_UploadFile_kmz 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_SubscribeTopic 函数");
        T_UAVReturnCode iRet =UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_POSITION, 0, myPositionGpsMsgCB);
        CHECK_TEST("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_POSITION iRet:{}",iRet);
        LOG_INFO("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_POSITION iRet:{}",iRet);
        iRet =UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED, 0, myPositionFusedMsgCB);
        CHECK_TEST("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED iRet:{}",iRet);
        LOG_INFO("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED iRet:{}",iRet);
        iRet =UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW, 0, myEulerAngularVelocityMsgCB);
        CHECK_TEST("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW iRet:{}",iRet);
        LOG_INFO("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW iRet:{}",iRet);
        iRet =UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY, 0, myVelocityComponentMsgCB);
        CHECK_TEST("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY iRet:{}",iRet);
        LOG_INFO("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY iRet:{}",iRet);
        iRet =UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO, 0, myEulerAngularMsgCB);
        CHECK_TEST("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO iRet:{}",iRet);
        LOG_INFO("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO iRet:{}",iRet);
        std::this_thread::sleep_for(std::chrono::seconds(15)); ///< wait a while for kmz file upload success
    }

    {
        // 测试 UAV_Waypoint_UploadFile_kmz 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_UnSubscribeTopic 函数");
        T_UAVReturnCode iRet =UAV_UnSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_POSITION);
        CHECK_TEST("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_POSITION iRet:{}",iRet);
        LOG_INFO("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_POSITION iRet:{}",iRet);
        iRet =UAV_UnSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED);
        CHECK_TEST("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED iRet:{}",iRet);
        LOG_INFO("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED iRet:{}",iRet);
        iRet =UAV_UnSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW);
        CHECK_TEST("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW iRet:{}",iRet);
        LOG_INFO("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW iRet:{}",iRet);
        iRet =UAV_UnSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY);
        CHECK_TEST("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY iRet:{}",iRet);
        LOG_INFO("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY iRet:{}",iRet);
        iRet =UAV_UnSubscribeTopic(UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO);
        CHECK_TEST("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO iRet:{}",iRet);
        LOG_INFO("UAV_UnSubscribeTopic UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO iRet:{}",iRet);
        std::this_thread::sleep_for(std::chrono::seconds(5)); 
    }


    {
        // 测试 UAV_FcSubscription_DeInit 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_FcSubscription_DeInit 函数");
        T_UAVReturnCode iRet =UAV_FcSubscription_DeInit();
        CHECK_TEST("UAV_FcSubscription_DeInit iRet:{}",iRet);
        LOG_INFO("UAV_FcSubscription_DeInit iRet:{}",iRet);
    }

    while (true)
    {
        ///< do business
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char *argv[])
{
    T_AUserInfo usrInfo;

    logger_init(std::string(argv[1]));
    if(false == uav_sdk_app_info_init(&usrInfo)) {
        LOG_ERROR("uav_sdk_app_info_init failed");
        return -1;
    }
    UAV_Core_Init(&usrInfo);
    UAV_Core_SetAlias("subscribe");
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread subscribe(sub_routine);
    subscribe.detach();

    return UAV_Core_ApplicationStart();
}
