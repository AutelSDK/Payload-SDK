#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_hms_manager.h"
#include "uav_sdk_app_info.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }

static void hmsInfoCallbackFunc(T_UAVHmsInfoTable hmsInfoTable)
{
    for (int i = 0; i < hmsInfoTable.hmsInfoNum; i++)
    {
        LOG_INFO("index {}: errorCode {}, componentIndex {}, errorLevel {}", i,
            hmsInfoTable.hmsInfo[i].errorCode, hmsInfoTable.hmsInfo[i].componentIndex, hmsInfoTable.hmsInfo[i].errorLevel);
    }
}

static void hms_manager_routine(void)
{
    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }
    
    {
        // 测试 UAV_HmsManager_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_HmsManager_Init 函数");
        T_UAVReturnCode iRet =UAV_HmsManager_Init();
        CHECK_TEST("UAV_HmsManager_Init iRet:{}",iRet);
        LOG_INFO("UAV_HmsManager_Init iRet:{}",iRet);
    }

    {
        // 测试 UAV_HmsManager_RegHmsInfoCallback 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_HmsManager_RegHmsInfoCallback 函数");
        T_UAVReturnCode iRet =UAV_HmsManager_RegHmsInfoCallback(hmsInfoCallbackFunc);
        CHECK_TEST("UAV_HmsManager_RegHmsInfoCallback iRet:{}",iRet);
        LOG_INFO("UAV_HmsManager_RegHmsInfoCallback iRet:{}",iRet);
    }

    {
        // 测试 UAV_HmsManager_PayloadSelfCHECK_TESTResult 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_HmsManager_PayloadSelfCHECK_TESTResult 函数");
        T_UAVReturnCode iRet =UAV_HmsManager_PayloadSelfCheckResult(1,"success");
        CHECK_TEST("UAV_HmsManager_PayloadSelfCHECK_TESTResult iRet:{}",iRet);
        LOG_INFO("UAV_HmsManager_PayloadSelfCHECK_TESTResult iRet:{}",iRet);
    }

    {
        // 测试 UAV_HmsManager_DeInit 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_HmsManager_DeInit 函数");
        UAV_HmsManager_DeInit();
        CHECK_TEST("UAV_HmsManager_DeInit iRet:{}",0);
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
    UAV_Core_SetAlias("tracerV1");
    // UAV_Uart_Init("/dev/pts/20",115200);
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread hms_manager(hms_manager_routine);
    hms_manager.detach();

    return UAV_Core_ApplicationStart();
}