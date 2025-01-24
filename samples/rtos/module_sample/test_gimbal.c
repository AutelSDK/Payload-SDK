#include "uav_gimbal.h"
#include "uav_core.h"
#include "uav_logger.h"
static T_UAVReturnCode GetSystemState(T_UAVGimbalSystemState *systemState);
static T_UAVReturnCode GetAttitudeInformation(T_UAVGimbalAttitudeInformation *attitudeInformation);
static T_UAVReturnCode GetCalibrationState(T_UAVGimbalCalibrationState *calibrationState);
static T_UAVReturnCode GetRotationSpeed(T_UAVAttitude3d *rotationSpeed);
static T_UAVReturnCode GetJointAngle(T_UAVAttitude3d *jointAngle);
static T_UAVReturnCode StartCalibrate(void);
static T_UAVReturnCode SetControllerSmoothFactor(uint8_t smoothingFactor, E_UAVGimbalAxis axis);
static T_UAVReturnCode SetPitchRangeExtensionEnabled(bool enabledFlag);
static T_UAVReturnCode SetControllerMaxSpeedPercentage(uint8_t maxSpeedPercentage, E_UAVGimbalAxis axis);
static T_UAVReturnCode RestoreFactorySettings(void);
static T_UAVReturnCode SetMode(E_UAVGimbalMode mode);
static T_UAVReturnCode Reset(E_UAVGimbalResetMode mode);
static T_UAVReturnCode FineTuneAngle(T_UAVAttitude3d fineTuneAngle);

T_UAVGimbalCommonHandler gimbalCommonHandler =
{
    .GetSystemState = GetSystemState,
    .GetAttitudeInformation = GetAttitudeInformation,
    .GetCalibrationState = GetCalibrationState,
    .GetRotationSpeed = GetRotationSpeed,
    .GetJointAngle = GetJointAngle,
    .StartCalibrate = StartCalibrate,
    .SetControllerSmoothFactor = SetControllerSmoothFactor,
    .SetPitchRangeExtensionEnabled = SetPitchRangeExtensionEnabled,
    .SetControllerMaxSpeedPercentage = SetControllerMaxSpeedPercentage,
    .RestoreFactorySettings = RestoreFactorySettings,
    .SetMode = SetMode,
    .Reset = Reset,
    .FineTuneAngle = FineTuneAngle,
};


T_UAVReturnCode UAV_Test_GimbalStartService(void)
{
    T_UAVReturnCode iRet;
    UAV_LOG_INFO("==========gimbal_routine==========");
    iRet = UAV_Gimbal_Init();
    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("UAV_Gimbal_Init failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    iRet = UAV_Gimbal_RegCommonHandler(&gimbalCommonHandler);
    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("UAV_Gimbal_RegCommonHandler failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    UAV_LOG_INFO("UAV_Gimbal_RegCommonHandler success");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode GetSystemState(T_UAVGimbalSystemState *systemState)
{
    UAV_LOG_INFO("GetSystemState");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetAttitudeInformation(T_UAVGimbalAttitudeInformation *attitudeInformation)
{
    UAV_LOG_INFO("GetAttitudeInformation");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode GetCalibrationState(T_UAVGimbalCalibrationState *calibrationState)
{
    UAV_LOG_INFO("GetCalibrationState");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetRotationSpeed(T_UAVAttitude3d *rotationSpeed)
{
    UAV_LOG_INFO("GetRotationSpeed");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetJointAngle(T_UAVAttitude3d *jointAngle)
{
    UAV_LOG_INFO("GetJointAngle");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode StartCalibrate(void)
{
    UAV_LOG_INFO("StartCalibrate");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode SetControllerSmoothFactor(uint8_t smoothingFactor, E_UAVGimbalAxis axis)
{
    UAV_LOG_INFO("SetControllerSmoothFactor");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetPitchRangeExtensionEnabled(bool enabledFlag)
{
    UAV_LOG_INFO("SetPitchRangeExtensionEnabled");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetControllerMaxSpeedPercentage(uint8_t maxSpeedPercentage, E_UAVGimbalAxis axis)
{
    UAV_LOG_INFO("SetControllerMaxSpeedPercentage");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode RestoreFactorySettings(void)
{
    UAV_LOG_INFO("RestoreFactorySettings");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetMode(E_UAVGimbalMode mode)
{
    UAV_LOG_INFO("SetMode");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode Reset(E_UAVGimbalResetMode mode)
{
    UAV_LOG_INFO("Reset");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode FineTuneAngle(T_UAVAttitude3d fineTuneAngle)
{
    UAV_LOG_INFO("FineTuneAngle");
    T_UAVReturnCode uavReturnCode = UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    return uavReturnCode;
}
