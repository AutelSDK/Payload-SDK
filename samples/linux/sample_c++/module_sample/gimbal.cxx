#include <string>
#include <thread>
#include <sys/time.h>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_upgrade.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_sdk_app_info.h"
#include "uav_fc_subscription.h"
#include "uav_low_speed_data_channel.h"
#include "uav_high_speed_data_channel.h"
#include "uav_gimbal.h"
#include "proto/tracerMessage.pb.h"
#include "uav_error.h"

static T_UAVReturnCode GetSystemState(T_UavGimbalSystemState *systemState);
static T_UAVReturnCode GetAttitudeInformation(T_UavGimbalAttitudeInformation *attitudeInformation);
static T_UAVReturnCode GetCalibrationState(T_UavGimbalCalibrationState *calibrationState);
static T_UAVReturnCode GetRotationSpeed(T_UAVAttitude3d *rotationSpeed);
static T_UAVReturnCode GetJointAngle(T_UAVAttitude3d *jointAngle);
static T_UAVReturnCode StartCalibrate(void);
static T_UAVReturnCode SetControllerSmoothFactor(uint8_t smoothingFactor, E_UavGimbalAxis axis);
static T_UAVReturnCode SetPitchRangeExtensionEnabled(bool enabledFlag);
static T_UAVReturnCode SetControllerMaxSpeedPercentage(uint8_t maxSpeedPercentage, E_UavGimbalAxis axis);
static T_UAVReturnCode RestoreFactorySettings(void);
static T_UAVReturnCode SetMode(E_UAVGimbalMode mode);
static T_UAVReturnCode Reset(E_UAVGimbalResetMode mode);
static T_UAVReturnCode FineTuneAngle(T_UAVAttitude3d fineTuneAngle);

void registThreadFunc(void);

int main(int argc, char *argv[])
{
    T_AUserInfo usrInfo;

    logger_init(std::string(argv[1]));
    if(false == uav_sdk_app_info_init(&usrInfo)) {
        LOG_ERROR("uav_sdk_app_info_init failed");
        return -1;
    }
    UAV_Core_Init(&usrInfo);
    UAV_Core_SetAlias("gimbalV1");
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);
    
    LOG_INFO(" gimbal init ");

    std::thread registerThread(registThreadFunc);
    registerThread.detach();

    return UAV_Core_ApplicationStart();
}

void registThreadFunc(void)
{
    int ret = 0;
    //wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }
    ret = UavGimbal_Init();
    LOG_INFO(" UavGimbal_Init :{}", ret);

    //订阅Speaker消息,进行对应的speaker操作
    ret =UAV_LowSpeedDataChannel_Init();
    LOG_INFO(" channel init ret :{}", ret);
    T_UavGimbalCommonHandler *commonHandler =new T_UavGimbalCommonHandler();
    commonHandler->GetSystemState = GetSystemState;
    commonHandler->GetAttitudeInformation = GetAttitudeInformation;
    commonHandler->GetCalibrationState = GetCalibrationState;
    commonHandler->GetRotationSpeed = GetRotationSpeed;
    commonHandler->GetJointAngle = GetJointAngle;
    //   s_commonHandler.Rotate = commonHandler->UavTest_GimbalRotate;
    commonHandler->StartCalibrate = StartCalibrate;
    commonHandler->SetControllerSmoothFactor = SetControllerSmoothFactor;
    commonHandler->SetPitchRangeExtensionEnabled = SetPitchRangeExtensionEnabled;
    commonHandler->SetControllerMaxSpeedPercentage = SetControllerMaxSpeedPercentage;
    commonHandler->RestoreFactorySettings = RestoreFactorySettings;
    commonHandler->SetMode = SetMode;
    commonHandler->Reset = Reset;
    commonHandler->FineTuneAngle = FineTuneAngle;
    ret = UavGimbal_RegCommonHandler(commonHandler);
    LOG_INFO(" UavGimbal_RegCommonHandler :{}", ret);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    ret = UavGimbal_DeInit();
    LOG_INFO(" UavGimbal_DeInit :{}", ret);
}



static T_UAVReturnCode GetSystemState(T_UavGimbalSystemState *systemState)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetAttitudeInformation(T_UavGimbalAttitudeInformation *attitudeInformation)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode GetCalibrationState(T_UavGimbalCalibrationState *calibrationState)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetRotationSpeed(T_UAVAttitude3d *rotationSpeed)
{

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetJointAngle(T_UAVAttitude3d *jointAngle)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode StartCalibrate(void)
{

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode SetControllerSmoothFactor(uint8_t smoothingFactor, E_UavGimbalAxis axis)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetPitchRangeExtensionEnabled(bool enabledFlag)
{

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetControllerMaxSpeedPercentage(uint8_t maxSpeedPercentage, E_UavGimbalAxis axis)
{

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode RestoreFactorySettings(void)
{

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetMode(E_UAVGimbalMode mode)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode Reset(E_UAVGimbalResetMode mode)
{
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode FineTuneAngle(T_UAVAttitude3d fineTuneAngle)
{
    T_UAVReturnCode uavReturnCode = UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    return uavReturnCode;
}
