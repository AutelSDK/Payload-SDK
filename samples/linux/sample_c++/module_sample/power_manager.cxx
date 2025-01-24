#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_sdk_app_info.h"
#include "uav_power_management.h"
#include "uav_fc_subscription.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


static T_UAVReturnCode PowerOffNotificationCallback(bool *powerOffPreparationFlag)
{
    LOG_INFO("Power off.");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static void myPowerOffMsgCB(void *data)
{
    T_UAVSubscriptionUAVPoweroff *posSt =  (T_UAVSubscriptionUAVPoweroff*)data;
    LOG_INFO("power off:{}", *posSt);
}
static void power_routine(void)
{
        // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    {
        // 测试 UAV_PowerManagement_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_PowerManagement_Init 函数");
        T_UAVReturnCode iRet =UAV_PowerManagement_Init();
        CHECK_TEST("UAV_PowerManagement_Init iRet:{}",iRet);
        LOG_INFO("UAV_PowerManagement_Init iRet:{}",iRet);
    }

   {
       // 测试 UAV_PowerManagement_ApplyHighPowerSync 函数的返回值
       // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       LOG_INFO("测试 UAV_PowerManagement_ApplyHighPowerSync 函数");
       T_UAVReturnCode iRet =UAV_PowerManagement_ApplyHighPowerSync();
       CHECK_TEST("UAV_PowerManagement_ApplyHighPowerSync iRet:{}",iRet);
       LOG_INFO("UAV_PowerManagement_ApplyHighPowerSync iRet:{}",iRet);
   }

    {
        // 测试 UAV_PowerManagement_RegPowerOffNotificationCallback 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_PowerManagement_RegPowerOffNotificationCallback 函数");
        T_UAVReturnCode iRet =UAV_PowerManagement_RegPowerOffNotificationCallback(PowerOffNotificationCallback);
        CHECK_TEST("UAV_PowerManagement_RegPowerOffNotificationCallback iRet:{}",iRet);
        LOG_INFO("UAV_PowerManagement_RegPowerOffNotificationCallback iRet:{}",iRet);

        iRet =UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_UAV_POWER_OFF, 0, myPowerOffMsgCB);
        CHECK_TEST("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_UAV_POWER_OFF iRet:{}",iRet);
        LOG_INFO("UAV_SubscribeTopic UAV_SUBSCRIPTION_TOPIC_UAV_POWER_OFF iRet:{}",iRet);
    }

    {
        // 测试 UAV_PowerManagement_Deinit 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_PowerManagement_Deinit 函数");
        T_UAVReturnCode iRet =UAV_PowerManagement_Deinit();
        CHECK_TEST("UAV_PowerManagement_Deinit iRet:{}",iRet);
        LOG_INFO("UAV_PowerManagement_Deinit iRet:{}",iRet);
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
    UAV_Core_SetAlias("powerManager");
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread powerManager(power_routine);
    powerManager.detach();

    return UAV_Core_ApplicationStart();
}

