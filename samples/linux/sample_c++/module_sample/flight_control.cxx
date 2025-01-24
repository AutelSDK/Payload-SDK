#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_sdk_app_info.h"
#include "uav_flight_control.h"


#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }

static void flightControlThreadFunc(void)
{
    // T_UAVFlightControllerRidInfo ridInfo;
    // UAV_FlightControl_Init(ridInfo);

    //wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    // 测试 UAV_FlightControl_Init 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_Init 函数");
        T_UAVFlightControllerRidInfo ridInfo;
        T_UAVReturnCode iRet =UAV_FlightControl_Init(ridInfo);
        CHECK_TEST("UAV_FlightControl_Init iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_SetControlMode 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_SetControlMode 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_SetControlMode(UAV_FLIGHTCONTROL_MODE_POS_CTL);
        CHECK_TEST("UAV_FlightControl_SetControlMode iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_GetControlMode 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_GetControlMode 函数");
        E_UAVFlithtControl_Mode mode;
        T_UAVReturnCode iRet =UAV_FlightControl_GetControlMode(mode);
        CHECK_TEST("UAV_FlightControl_GetControlMode iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_SetRCLostAction 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_SetRCLostAction 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_SetRCLostAction(UAV_FLIGHTCONTROL_RC_LOST_ACTION_GOHOME);
        CHECK_TEST("UAV_FlightControl_SetRCLostAction iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_GetRCLostAction 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_GetRCLostAction 函数");
        E_UAVFlightControl_RCLost_Action action;
        T_UAVReturnCode iRet =UAV_FlightControl_GetRCLostAction(action);
        CHECK_TEST("UAV_FlightControl_GetRCLostAction iRet:{}",iRet);
    }

//    TEST_CASE("测试 UAV_FlightControl_SetRtkPositionEnableStatus 函数") {
//        // 测试 UAV_FlightControl_SetRtkPositionEnableStatus 函数的返回值
//        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
//        LOG_INFO("测试 UAV_FlightControl_SetRtkPositionEnableStatus 函数");
//        T_UAVReturnCode iRet =UAV_FlightControl_SetRtkPositionEnableStatus(true);
//        CHECK_TEST(iRet== UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS);
//        LOG_INFO("UAV_FlightControl_SetRtkPositionEnableStatus iRet:{}",iRet);
//    }
//
//    TEST_CASE("测试 UAV_FlightControl_GetRtkPositionEnableStatus 函数") {
//        // 测试 UAV_FlightControl_GetRtkPositionEnableStatus 函数的返回值
//        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
//        LOG_INFO("测试 UAV_FlightControl_GetRtkPositionEnableStatus 函数");
//        bool status;
//        T_UAVReturnCode iRet =UAV_FlightControl_GetRtkPositionEnableStatus(status);
//        CHECK_TEST(iRet== UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS);
//        LOG_INFO("UAV_FlightControl_GetRtkPositionEnableStatus iRet:{}",iRet);
//    }

    // 测试 UAV_FlightControl_SpeedControl 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_SpeedControl 函数");
        T_UAVFlightControlSpeed speed;
        speed.x = 0;
        speed.y = 0;
        speed.z = 0;
        speed.yaw = 0;
        speed.heading_mode = UAV_HEADING_MODE_ALONG;
        speed.obstacle_mode = UAV_OBSTACLE_MODE_LOITER;
        T_UAVReturnCode iRet =UAV_FlightControl_SpeedControl(speed);
        CHECK_TEST("UAV_FlightControl_SpeedControl iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_POSControl 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_POSControl 函数");
        T_UAVFlightControlPos position;
        position.latitude = 123;
        position.longitude = 234;
        position.altitude = 345;
        position.obstacle_mode = UAV_OBSTACLE_MODE_OBS;
        T_UAVReturnCode iRet =UAV_FlightControl_POSControl(position);
        CHECK_TEST("UAV_FlightControl_POSControl iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_TurnOnMotors 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_TurnOnMotors 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_TurnOnMotors();
        CHECK_TEST("UAV_FlightControl_TurnOnMotors iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_TurnOffMotors 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_TurnOffMotors 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_TurnOffMotors();
        CHECK_TEST("UAV_FlightControl_TurnOffMotors iRet:{}",iRet);
    }

    {
        // 测试 UAV_FlightControl_EmergencyStopMotor 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_FlightControl_EmergencyStopMotor 函数");
        E_UAVFlightControl_Emergency_Stop_Motor stopMotor = UAV_FLIGHTCONTROL_ENABLE_EMERGENCY_STOP_MOTOR;
        char debugMsg[32]={0};
        T_UAVReturnCode iRet = UAV_FlightControl_EmergencyStopMotor(stopMotor,debugMsg);
        CHECK_TEST("UAV_FlightControl_EmergencyStopMotor iRet:{}",iRet);
        LOG_INFO("UAV_FlightControl_EmergencyStopMotor iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_StartTakeoff 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_StartTakeoff 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_StartTakeoff();
        CHECK_TEST("UAV_FlightControl_StartTakeoff iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_StartLanding 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_StartLanding 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_StartLanding();
        CHECK_TEST("UAV_FlightControl_StartLanding iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_CancelLanding 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_CancelLanding 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_CancelLanding();
        CHECK_TEST("UAV_FlightControl_CancelLanding iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_StartForceLanding 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_StartForceLanding 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_StartForceLanding();
        CHECK_TEST("UAV_FlightControl_StartForceLanding iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_SetHomeLocationUsingGPSCoordinates 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_SetHomeLocationUsingGPSCoordinates 函数");
        T_UavFlightControllerHomeLocation homeLocation;
        homeLocation.latitude =144.4444;
        homeLocation.longitude =133.3333;
        T_UAVReturnCode iRet =UAV_FlightControl_SetHomeLocationUsingGPSCoordinates(homeLocation);
        CHECK_TEST("UAV_FlightControl_SetHomeLocationUsingGPSCoordinates iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_SetHomeLocationUsingCurrentAircraftLocation 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_SetHomeLocationUsingCurrentAircraftLocation 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_SetHomeLocationUsingCurrentAircraftLocation();
        CHECK_TEST("UAV_FlightControl_SetHomeLocationUsingCurrentAircraftLocation iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_SetGoHomeAltitude 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_SetGoHomeAltitude 函数");
        E_UavFlightControllerGoHomeAltitude homeAltitude;
        homeAltitude =144;
        T_UAVReturnCode iRet =UAV_FlightControl_SetGoHomeAltitude(homeAltitude);
        CHECK_TEST("UAV_FlightControl_SetGoHomeAltitude iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_GetGoHomeAltitude 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_GetGoHomeAltitude 函数");
        E_UavFlightControllerGoHomeAltitude homeAltitude;
        T_UAVReturnCode iRet =UAV_FlightControl_GetGoHomeAltitude(&homeAltitude);
        LOG_INFO("GetGoHomeAltitude value:{}",homeAltitude);
        CHECK_TEST("UAV_FlightControl_GetGoHomeAltitude iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_StartGoHome 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_StartGoHome 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_StartGoHome();
        CHECK_TEST("UAV_FlightControl_StartGoHome iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_CancelGoHome 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_CancelGoHome 函数");
        T_UAVReturnCode iRet =UAV_FlightControl_CancelGoHome();
        CHECK_TEST("UAV_FlightControl_CancelGoHome iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_GetGeneralInfo 函数的返回值
    // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    {
        LOG_INFO("测试 UAV_FlightControl_GetGeneralInfo 函数");
        T_UavFlightControllerGeneralInfo generalInfo;
        T_UAVReturnCode iRet =UAV_FlightControl_GetGeneralInfo(&generalInfo);
        std::string serialNumStr(generalInfo.serialNum); // 自动计算长度直到空字符
        LOG_INFO("GetGeneralInfo value:{}",serialNumStr);
        CHECK_TEST("UAV_FlightControl_GetGeneralInfo iRet:{}",iRet);
    }

    // 测试 UAV_FlightControl_Deinit 函数的返回值
    {
        LOG_INFO("测试 UAV_FlightControl_Deinit 函数");
        UAV_FlightControl_Deinit();
        CHECK_TEST("UAV_FlightControl_Deinit iRet:{}",0);
    }

    while(1)
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    UAV_Core_SetAlias("flight_controlV1");
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread flightControlThread(flightControlThreadFunc);
    flightControlThread.detach();
    return UAV_Core_ApplicationStart();
}
