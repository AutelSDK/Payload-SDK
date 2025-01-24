#include <string>
#include <thread>
#include <sys/time.h>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_time_sync.h"
#include "uav_sdk_app_info.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }

static T_UAVReturnCode GetNewestPpsTriggerLocalTimeUsCallback(uint64_t *localTimeUs)
{
    LOG_INFO("pps time callback iRet");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

uint64_t get_local_time_us(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

static void timesync_routine(void)
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
        E_UAVTimeSyncType type = UAV_TIME_SYNC_TYPE_NTP;
        T_UAVReturnCode iRet;
        iRet =UAV_TimeSync_SetType(type);
        CHECK_TEST("UAV_TimeSync_SetType iRet:{} ",iRet);
        LOG_INFO("UAV_TimeSync_SetType iRet:{} ",iRet);
    }

    {
        // 测试 UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback 函数");
        T_UAVReturnCode iRet;
        iRet =UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback(GetNewestPpsTriggerLocalTimeUsCallback);
        CHECK_TEST("UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback iRet:{} ",iRet);
        LOG_INFO("UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback iRet:{} ",iRet);
    }

    while (true)
    {
        // 测试 UAV_TimeSync_NTP 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_TimeSync_TransferToAircraftTime 函数");
        T_UAVTimeSyncAircraftTime timestamp = {0};
        T_UAVReturnCode iRet;

        iRet = UAV_TimeSync_TransferToAircraftTime(get_local_time_us(), &timestamp);
        CHECK_TEST("UAV_TimeSync_TransferToAircraftTime iRet:{} ",iRet);
        LOG_INFO("UAV_TimeSync_TransferToAircraftTime iRet:{} ",iRet);
        LOG_INFO("current aircraft time is {}-{}-{} {}:{}:{} {}.",
                timestamp.year, timestamp.month, timestamp.day,
                timestamp.hour, timestamp.minute, timestamp.second, timestamp.milliseconds);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    {
        // 测试 UAV_TimeSync_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_TimeSync_Deinit 函数");
        UAV_TimeSync_Deinit();
        CHECK_TEST("UAV_TimeSync_Deinit iRet:{}",0);
        LOG_INFO("UAV_TimeSync_Deinit iRet:{}",0);
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
    UAV_Core_SetAlias("timeSync");
    // UAV_Uart_Init("/dev/pts/20",115200);
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread timesync(timesync_routine);
    timesync.detach();

    return UAV_Core_ApplicationStart();
}