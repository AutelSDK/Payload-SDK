#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"
#include "uav_flight_control.h"
#define CHECK(str, ret) \
    if (ret != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) { \
        UAV_LOG_ERROR("%s failed", str); \
    } \
    else { \
        UAV_LOG_INFO("%s success", str); \
    }
T_UAVFlightControllerRidInfo ridInfo = {0};
T_UAVReturnCode UAV_Test_flightControlStartService(void)
{
    T_UAVReturnCode returnCode;
    T_UAVReturnCode iRet;
    T_UAVOsalHandler *os_handler = nullptr;
    UAV_LOG_INFO("==========flight_ctrl_routine==========");
    returnCode = UAV_FlightControl_Init(ridInfo);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("flight control init failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    iRet =  UAV_FlightControl_EmergencyStopMotor(UAV_FLIGHTCONTROL_ENABLE_EMERGENCY_STOP_MOTOR, "stop motor" );
    CHECK("emergency stop motor", iRet);

    iRet = UAV_FlightControl_SetControlMode(UAV_FLIGHTCONTROL_MODE_POS_CTL);
    CHECK("set control mode", iRet);

    E_FLIGHTCONTROL_MODE mode;
    iRet = UAV_FlightControl_GetControlMode(&mode);
    CHECK("get control mode", iRet);

    iRet =UAV_FlightControl_SetRCLostAction(UAV_FLIGHTCONTROL_RC_LOST_ACTION_GOHOME);
    CHECK("set rc lost action", iRet);

    E_FLIGHTCONTROL_LCLOST_ACTION action;
    iRet =UAV_FlightControl_GetRCLostAction(&action);
    CHECK("get rc lost action", iRet);

    T_UAVFlightControlPos pos;
    pos.altitude = 0;
    pos.latitude = 0;
    pos.longitude = 0;
    pos.obstacle_mode = UAV_OBSTACLE_MODE_LOITER;
    iRet = UAV_FlightControl_POSControl(pos);
    CHECK("pos control", iRet);

    T_UAVFlightControlSpeed speed;
    speed.x = 1;
    speed.y = 1;
    speed.z = 1;
    speed.yaw = 0;
    speed.heading_mode = UAV_HEADING_MODE_ALONG;
    speed.obstacle_mode = UAV_OBSTACLE_MODE_LOITER;
    iRet = UAV_FlightControl_SpeedControl(speed);
    CHECK("speed control", iRet);

    iRet =UAV_FlightControl_TurnOnMotors();
    CHECK("turn on motors", iRet);

    iRet =UAV_FlightControl_TurnOffMotors();
    CHECK("turn off motors", iRet);

    iRet = UAV_FlightControl_StartTakeoff();
    CHECK("start takeoff", iRet);

    iRet = UAV_FlightControl_StartLanding();
    CHECK("start landing", iRet);

    iRet = UAV_FlightControl_CancelLanding();
    CHECK("cancel landing", iRet);

    iRet = UAV_FlightControl_StartForceLanding();
    CHECK("start force landing", iRet);

    T_UAVFlightControllerHomeLocation homeLocation;
    homeLocation.latitude =144.4444;
    homeLocation.longitude =133.3333;
    iRet = UAV_FlightControl_SetHomeLocationUsingGPSCoordinates(homeLocation);
    CHECK("set home location using gps coordinates", iRet);

    iRet =UAV_FlightControl_SetHomeLocationUsingCurrentAircraftLocation();
    CHECK("set home location using current aircraft location", iRet);

    E_UAVFlightControllerGoHomeAltitude homeAltitude;
    homeAltitude =144;
    iRet =UAV_FlightControl_SetGoHomeAltitude(homeAltitude);
    CHECK("set go home altitude", iRet);

    homeAltitude =0;
    iRet =UAV_FlightControl_GetGoHomeAltitude(&homeAltitude);
    CHECK("get go home altitude", iRet);


    iRet =UAV_FlightControl_StartGoHome();
    CHECK("start go home", iRet);

    iRet =UAV_FlightControl_CancelGoHome();
    CHECK("cancel go home", iRet);

    T_UAVFlightControllerGeneralInfo generalInfo;
    iRet =UAV_FlightControl_GetGeneralInfo(&generalInfo);
    CHECK("get general info", iRet);

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;

}
