#include <string>
#include <thread>
#include <sys/time.h>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_time_sync.h"
#include "uav_positioning.h"
#include "uav_sdk_app_info.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


uint64_t get_local_time_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void positioningThreadFunc(void)
{
    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    {
        // 测试 UAV_TimeSync_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_TimeSync_Init 函数");
        T_UAVReturnCode iRet =UAV_TimeSync_Init();
        CHECK_TEST("UAV_TimeSync_Init iRet:{}",iRet);
        LOG_INFO("UAV_TimeSync_Init iRet:{}",iRet);
    }

    {
        // 测试 UAV_TimeSync_SetType 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_TimeSync_SetType 函数");
        E_UAVTimeSyncType type=UAV_TIME_SYNC_TYPE_NTP;
        T_UAVReturnCode iRet;
        iRet =UAV_TimeSync_SetType(type);
        CHECK_TEST("UAV_TimeSync_SetType iRet:{} ",iRet);
        LOG_INFO("UAV_TimeSync_SetType iRet:{} ",iRet);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    {
        // 测试 UAV_TimeSync_NTP 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_TimeSync_TransferToAircraftTime 函数");
        T_UAVTimeSyncAircraftTime timestamp = {0};
        T_UAVReturnCode iRet;

        iRet =UAV_TimeSync_TransferToAircraftTime(get_local_time_us(), &timestamp);
        CHECK_TEST("UAV_TimeSync_TransferToAircraftTime iRet:{} ",iRet);
        LOG_INFO("UAV_TimeSync_TransferToAircraftTime iRet:{} ",iRet);
    }

    {
        // 测试 UAV_TimeSync_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Positioning_Init 函数");
        T_UAVReturnCode iRet =UAV_Positioning_Init();
        CHECK_TEST("UAV_Positioning_Init iRet:{}",iRet);
        LOG_INFO("UAV_Positioning_Init iRet:{}",iRet);
    }

    {
        // 测试 UAV_Positioning_GetPositioning_Sync 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Positioning_GetPositioning_Sync 函数");
        T_UAVPositioningEventInfo eventInfo[2];
        T_UAVPositioningPositionInfo positionInfo[2];
        eventInfo[0].eventIndex =0;
        eventInfo[0].timestamp.year =2024;
        eventInfo[0].timestamp.month =12;
        eventInfo[0].timestamp.day =26;
        eventInfo[0].timestamp.hour =7;
        eventInfo[0].timestamp.minute =50;
        eventInfo[0].timestamp.second =59;
        eventInfo[0].timestamp.milliseconds =500;
        eventInfo[1].eventIndex =1;
        eventInfo[1].timestamp.year =2024;
        eventInfo[1].timestamp.month =12;
        eventInfo[1].timestamp.day =26;
        eventInfo[1].timestamp.hour =7;
        eventInfo[1].timestamp.minute =50;
        eventInfo[1].timestamp.second =60;
        eventInfo[1].timestamp.milliseconds =500;

        while (1)
        {
            
            T_UAVReturnCode iRet =UAV_Positioning_GetPositioning_Sync(2, (T_UAVPositioningEventInfo *)&eventInfo,(T_UAVPositioningPositionInfo *)&positionInfo);
            if (iRet == UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
            {
                LOG_INFO("get positioning sync success, {}, [{},{},{}], [{},{},{}]", 
                                    (int)positionInfo[0].property, 
                                    positionInfo[0].uavAttitude.roll, positionInfo[0].uavAttitude.pitch, positionInfo[0].uavAttitude.yaw,
                                    positionInfo[0].positionDeviation.longitude, positionInfo[0].positionDeviation.latitude, positionInfo[0].positionDeviation.altitude);
            }
            CHECK_TEST("UAV_Positioning_GetPositioning_Sync iRet:{}",iRet);
            LOG_INFO("UAV_Positioning_GetPositioning_Sync iRet:{}",iRet);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    {
        // 测试 UAV_TimeSync_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Positioning_Deinit 函数");
        UAV_Positioning_Deinit();
        CHECK_TEST("UAV_Positioning_Deinit iRet:{}",0);
        LOG_INFO("UAV_Positioning_Deinit iRet:{}",0);
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
    UAV_Core_SetAlias("positioning");
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread positioningThread(positioningThreadFunc);
    positioningThread.detach();
    return UAV_Core_ApplicationStart();
}
