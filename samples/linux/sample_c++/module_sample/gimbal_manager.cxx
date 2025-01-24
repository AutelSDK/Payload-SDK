#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_gimbal_manager.h"
#include "uav_sdk_app_info.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


static void hgaimbal_manager_routine(void)
{
    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }
    {
        // 测试 UAV_GimbalManager_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_GimbalManager_Init 函数");
        T_UAVReturnCode iRet = UAV_GimbalManager_Init();
        CHECK_TEST("UAV_GimbalManager_Init iRet:{}",iRet);
    }

    {
        LOG_INFO("测试 UAV_GimbalManager_SetMode UAV_GIMBAL_MODE_FREE函数");
        // 测试 UAV_GimbalManager_SetMode 函数的不同模式设置
        // 期望：当设置为 UAV_GIMBAL_MODE_FREE 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        T_UAVReturnCode iRet = UAV_GimbalManager_SetMode(UAV_GIMBAL_MODE_FREE);
        CHECK_TEST("UAV_GimbalManager_SetMode UAV_GIMBAL_MODE_FREE iRet:{}",iRet);

        // 期望：当设置为 UAV_GIMBAL_MODE_FPV 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试  UAV_GimbalManager_SetMode  UAV_GIMBAL_MODE_FPV函数");
        iRet = UAV_GimbalManager_SetMode(UAV_GIMBAL_MODE_FPV);
        CHECK_TEST("UAV_GimbalManager_SetMode UAV_GIMBAL_MODE_FPV iRet:{}",iRet);

        // 期望：当设置为 UAV_GIMBAL_MODE_YAW_FOLLOW 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试  UAV_GimbalManager_SetMode  UAV_GIMBAL_MODE_YAW_FOLLOW函数");
        iRet = UAV_GimbalManager_SetMode(UAV_GIMBAL_MODE_YAW_FOLLOW);
        CHECK_TEST("UAV_GimbalManager_SetMode UAV_GIMBAL_MODE_YAW_FOLLOW iRet:{}",iRet);
    }

    {
        LOG_INFO("测试 UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_PITCH_AND_YAW 函数");
        // 测试 UAV_GimbalManager_Reset 函数的不同重置模式
        // 期望：当设置为 UAV_GIMBAL_RESET_MODE_PITCH_AND_YAW 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       T_UAVReturnCode iRet = UAV_GimbalManager_Reset(UAV_GIMBAL_RESET_MODE_PITCH_AND_YAW);
       CHECK_TEST("UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_PITCH_AND_YAW iRet:{}",iRet);

       //期望：当设置为 UAV_GIMBAL_RESET_MODE_YAW 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       LOG_INFO("测试 UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_YAW 函数");
       iRet = UAV_GimbalManager_Reset(UAV_GIMBAL_RESET_MODE_YAW);
       CHECK_TEST("UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_YAW iRet:{}",iRet);

       //期望：当设置为 UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD_AND_YAW 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       LOG_INFO("测试 UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD_AND_YAW 函数");
       iRet = UAV_GimbalManager_Reset(UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD_AND_YAW);
       CHECK_TEST("UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD_AND_YAW iRet:{}",iRet);

        // 期望：当设置为 UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_GimbalManager_Reset UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD 函数");
        iRet = UAV_GimbalManager_Reset(UAV_GIMBAL_RESET_MODE_PITCH_DOWNWARD_UPWARD);
        CHECK_TEST("UAV_GimbalManager_Reset iRet:{}",iRet);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    {
        LOG_INFO("测试 UAV_GimbalManager_Rotate UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE 函数");
        // 测试 UAV_GimbalManager_Rotate 函数的不同旋转模式
        // 期望：当设置为 UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       T_UAVGimbalManagerRotation rotation;
       rotation.rotationMode = UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE;
       rotation.pitch = 2.1f;
       rotation.roll = 0.7f;
       rotation.yaw = 4.6f;
       rotation.time = 0;
       T_UAVReturnCode iRet = UAV_GimbalManager_Rotate(rotation);
       CHECK_TEST("UAV_GimbalManager_Rotate UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE iRet:{}",iRet);
       std::this_thread::sleep_for(std::chrono::seconds(2));

       //期望：当设置为 UAV_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE 模式时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       LOG_INFO("测试 UAV_GimbalManager_Rotate UAV_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE 函数");
       rotation.rotationMode = UAV_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE;
       rotation.pitch = 20.1f;
       rotation.roll = 7.7f;
       rotation.yaw = -46.0f;
       rotation.time = 0;
       iRet = UAV_GimbalManager_Rotate(rotation);
       CHECK_TEST("UAV_GimbalManager_Rotate UAV_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE iRet:{}",iRet);
       std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    {
        LOG_INFO("测试 UAV_GimbalManager_SetPitchRangeExtensionEnabled 1 函数");
        // 测试 UAV_GimbalManager_SetPitchRangeExtensionEnabled 函数的不同设置
        // 期望：当设置为 true 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        T_UAVReturnCode iRet = UAV_GimbalManager_SetPitchRangeExtensionEnabled(true);
        CHECK_TEST("UAV_GimbalManager_SetPitchRangeExtensionEnabled iRet:{}",iRet);

        // 期望：当设置为 false 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_GimbalManager_SetPitchRangeExtensionEnabled 0 函数");
        iRet = UAV_GimbalManager_SetPitchRangeExtensionEnabled(false);
        CHECK_TEST("UAV_GimbalManager_SetPitchRangeExtensionEnabled iRet:{}",iRet);
    }

    {
        // 测试 UAV_GimbalManager_SetControllerMaxSpeedPercentage 函数的不同设置
        // 入参：E_UAVMountPosition mountPosition, E_UavGimbalAxis axis, uint8_t maxSpeedPercentage
        // 返回值：T_UAVReturnCode
        LOG_INFO("测试 UAV_GimbalManager_SetControllerMaxSpeedPercentage 44 函数");
        // 期望：当设置为 UAV_GIMBAL_AXIS_PITCH 以及速度百分比为 44 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        T_UAVReturnCode iRet = UAV_GimbalManager_SetControllerMaxSpeedPercentage(UAV_GIMBAL_AXIS_PITCH, 44) ;
        CHECK_TEST("UAV_GimbalManager_SetControllerMaxSpeedPercentage iRet:{}",iRet);


        // 期望：当设置为 UAV_GIMBAL_AXIS_YAW 以及速度百分比为 50 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_GimbalManager_SetControllerMaxSpeedPercentage 50 函数");
        iRet = UAV_GimbalManager_SetControllerMaxSpeedPercentage(UAV_GIMBAL_AXIS_YAW, 50) ;
        CHECK_TEST("UAV_GimbalManager_SetControllerMaxSpeedPercentage iRet:{}",iRet);

        // 期望：当设置为 UAV_GIMBAL_AXIS_ROLL 以及速度百分比为 75 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_GimbalManager_SetControllerMaxSpeedPercentage 75 函数");
        iRet = UAV_GimbalManager_SetControllerMaxSpeedPercentage(UAV_GIMBAL_AXIS_ROLL, 75) ;
        CHECK_TEST("UAV_GimbalManager_SetControllerMaxSpeedPercentage iRet:{}",iRet);

    }

    {
       LOG_INFO("测试 UAV_GimbalManager_SetControllerSmoothFactor 10 函数");
       // 期望：当设置为 UAV_GIMBAL_AXIS_YAW 以及平滑因子为 10 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       T_UAVReturnCode iRet = UAV_GimbalManager_SetControllerSmoothFactor(UAV_GIMBAL_AXIS_YAW, 10) ;
       CHECK_TEST("UAV_GimbalManager_SetControllerSmoothFactor UAV_GIMBAL_AXIS_YAW iRet:{}",iRet);
//
//
       // 期望：当设置为 UAV_GIMBAL_AXIS_PITCH 以及平滑因子为 20 时，函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
       LOG_INFO("测试 UAV_GimbalManager_SetControllerSmoothFactor 20 函数");
       iRet = UAV_GimbalManager_SetControllerSmoothFactor(UAV_GIMBAL_AXIS_PITCH, 20) ;
       CHECK_TEST("UAV_GimbalManager_SetControllerSmoothFactor UAV_GIMBAL_AXIS_PITCH iRet:{}",iRet);

    }

    {
        // 测试 UAV_GimbalManager_RestoreFactorySettings 函数的执行情况
        // 期望：当设置为 函数调用成功返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_GimbalManager_RestoreFactorySettings 函数");
        T_UAVReturnCode iRet = UAV_GimbalManager_RestoreFactorySettings()  ;
        CHECK_TEST("UAV_GimbalManager_RestoreFactorySettings iRet:{}",iRet);

    }

    {
        // 测试 UAV_GimbalManager_Deinit 函数的执行情况
        // 期望：函数可以成功执行，但由于没有返回值，主要验证不抛出异常
        LOG_INFO("测试 UAV_GimbalManager_Deinit 函数");
        UAV_GimbalManager_Deinit();
        CHECK_TEST("Deinit: ",0); // 验证函数执行完成
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
    UAV_Core_SetAlias("Gimbal");
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread gaimbal_manager(hgaimbal_manager_routine);
    gaimbal_manager.detach();

    return UAV_Core_ApplicationStart();
}