#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_sdk_app_info.h"

static void remote_log_routine(void)
{
    //wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    RLOG_DEBUG("waitHandShakeRegister success");
    RLOG_TRACE("waitHandShakeRegister success");
    RLOG_INFO("waitHandShakeRegister success");
    RLOG_WARN("waitHandShakeRegister success");
    RLOG_ERROR("waitHandShakeRegister success");

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        RLOG_INFO("remote logger test");
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
    UAV_Core_SetAlias("remoteLogger");
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread remoteLog(remote_log_routine);
    remoteLog.detach();

    return UAV_Core_ApplicationStart();
}

