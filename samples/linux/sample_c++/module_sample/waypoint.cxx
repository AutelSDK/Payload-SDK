#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_waypoint.h"
#include "uav_sdk_app_info.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


static int WaypointMissionStateHandler(T_UAV_WAYPOINT_MISSION_STATE missionState){
    LOG_INFO("recv waypoint missionState.");
    return 0;
}

static int WaypointActionStateHandler(T_UAV_WAYPOINT_ACTION_STATE actionState) {
    LOG_INFO("recv waypoint ActionState.");
    return 0;
}

static void waypoint_routine(const std::string &kmz_file)
{
    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    {
        // 测试 UAV_Waypoint_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Waypoint_Init 函数");
        T_UAVReturnCode iRet =UAV_Waypoint_Init();
        CHECK_TEST("UAV_Waypoint_Init iRet:{}",iRet);
        LOG_INFO("UAV_Waypoint_Init iRet:{}",iRet);
    }

    {
        // 测试 UAV_Waypoint_UploadFile_kmz 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Waypoint_UploadFile_kmz 函数");
        T_UAVReturnCode iRet =UAV_Waypoint_UploadFile_kmz(kmz_file.c_str());
        CHECK_TEST("UAV_Waypoint_UploadFile_kmz iRet:{}",iRet);
        LOG_INFO("UAV_Waypoint_UploadFile_kmz iRet:{}",iRet);
        std::this_thread::sleep_for(std::chrono::seconds(3)); ///< wait a while for kmz file upload success
    }

    {
        // 测试 UAV_Waypoint_Action 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Waypoint_Action 函数");
        T_UAVReturnCode iRet =UAV_Waypoint_Action(UAV_WAYPOINT_ACTION_START, 3000);
        CHECK_TEST("UAV_Waypoint_Action iRet:{}",iRet);
        LOG_INFO("UAV_Waypoint_Action iRet:{}",iRet);
    }

    {
        // 测试 UAV_Waypoint_RegisterMissionStateCallback 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Waypoint_RegisterMissionStateCallback 函数");
        T_UAVReturnCode iRet = UAV_Waypoint_RegisterMissionStateCallback(WaypointMissionStateHandler);
        CHECK_TEST("UAV_Waypoint_RegisterMissionStateCallback iRet:{}",iRet);
        LOG_INFO("UAV_Waypoint_RegisterMissionStateCallback iRet:{}",iRet);
        
    }

    {
        // 测试 UAV_Waypoint_RegisterActionStateCallback 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_Waypoint_RegisterActionStateCallback 函数");
        T_UAVReturnCode iRet = UAV_Waypoint_RegisterActionStateCallback(WaypointActionStateHandler);
        CHECK_TEST("UAV_Waypoint_RegisterActionStateCallback iRet:{}",iRet);
        LOG_INFO("UAV_Waypoint_RegisterActionStateCallback iRet:{}",iRet);
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
    UAV_Core_SetAlias("waypoint");
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::string kmz_file = std::string(argv[3]);
    std::thread waypoint(waypoint_routine, kmz_file);
    waypoint.detach();

    return UAV_Core_ApplicationStart();
}

