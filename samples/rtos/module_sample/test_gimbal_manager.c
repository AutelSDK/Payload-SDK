
#include "uav_gimbal_manager.h"
#include "uav_core.h"
#include "uav_logger.h"

#define CHECK(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                UAV_LOG_ERROR("%s failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                UAV_LOG_INFO("%s success",str);                 \
                            }

T_UAVReturnCode UAV_Test_GimalManagerStartService(void)
{
    T_UAVReturnCode returnCode;
    T_UAVReturnCode iRet;
    T_UAVOsalHandler *os_handler = nullptr;
    UAV_LOG_INFO("==========gimbal_manager_routine==========");
    returnCode = UAV_GimbalManager_Init();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("gimbal manager init failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    returnCode = UAV_GimbalManager_Reset(UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD);
    CHECK("reset pitch downward upward", returnCode);

    returnCode = UAV_GimbalManager_SetMode(UAV_GIMBAL_MODE_FREE);
    CHECK("set free mode", returnCode);

    returnCode = UAV_GimbalManager_SetMode(UAV_GIMBAL_MODE_YAW_FOLLOW);
    CHECK("set yaw follow mode", returnCode);

    returnCode = UAV_GimbalManager_SetMode(UAV_GIMBAL_MODE_FPV);
    CHECK("set fpv mode", returnCode);

    returnCode = UAV_GimbalManager_SetPitchRangeExtensionEnabled(true);
    CHECK("set pitch range extension enabled", returnCode);

    returnCode = UAV_GimbalManager_SetControllerMaxSpeedPercentage(UAV_GIMBAL_AXIS_PITCH, 44);
    CHECK("set controller max speed percentage", returnCode);

    returnCode = UAV_GimbalManager_SetControllerMaxSpeedPercentage(UAV_GIMBAL_AXIS_YAW, 50);
    CHECK("set controller max speed percentage", returnCode);


    returnCode = UAV_GimbalManager_SetControllerMaxSpeedPercentage(UAV_GIMBAL_AXIS_ROLL, 75);
    CHECK("set controller max speed percentage", returnCode);

    returnCode = UAV_GimbalManager_RestoreFactorySettings();
    CHECK("restore factory settings", returnCode);


    T_UAVGimbalManagerRotation rotation;
    rotation.rotationMode = UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE;
    rotation.pitch = 2.1f;
    rotation.roll = 0.7f;
    rotation.yaw = 4.6f;
    rotation.time = 0;
    returnCode = UAV_GimbalManager_Rotate(rotation);
    CHECK("rotate gimbal", returnCode);


    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

