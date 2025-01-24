#include "test_waypoint.h"
#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"

#define CHECK(str, ret) \
    if (ret != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) \
    { \
        UAV_LOG_ERROR("%s failed", str); \
    } \
    else \
    { \
        UAV_LOG_INFO("%s success", str); \
    }

T_UAVReturnCode UAV_Test_WaypointStartService(void)
{
    T_UAVReturnCode UavStat, iRet;
    uint8_t file_data[16] = {0};
    UAV_LOG_INFO("==========waypoint_routine==========");
    UavStat = UAV_Waypoint_Init();
    if(UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS != UavStat)
    {
        UAV_LOG_ERROR("UAV_Waypoint_Init failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    iRet =UAV_Waypoint_Action(UAV_WAYPOINT_ACTION_START, 3000);
    CHECK("waypoint action", iRet);

    iRet = UAV_Waypoint_RegisterMissionStateCallback(nullptr);
    CHECK("register mission state callback", iRet);

    iRet = UAV_Waypoint_RegisterActionStateCallback(nullptr);
    CHECK("register action state callback", iRet);

    for(int i=0; i<sizeof(file_data); i++)
    {
        file_data[i] = i;
    }
    // iRet = UAV_Waypoint_Upload_kmz(file_data, sizeof(file_data));
    // CHECK("upload kmz", iRet);

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}