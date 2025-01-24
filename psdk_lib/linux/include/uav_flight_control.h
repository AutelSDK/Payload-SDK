#ifndef __UAV_FLIGHT_CONTROL_HPP__
#define __UAV_FLIGHT_CONTROL_HPP__
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UAV_FLIGHTCONTROL_MODE_UNKNOWN = 0,
    UAV_FLIGHTCONTROL_MODE_POS_CTL = 1,
    UAV_FLIGHTCONTROL_MODE_SPEED_CTL = 2,
}E_UAVFlithtControl_Mode;

typedef enum {
    UAV_FLIGHTCONTROL_RC_LOST_ACTION_HOVER   = 0,  /*!< Aircraft will execute hover cation when rc is lost. */
    UAV_FLIGHTCONTROL_RC_LOST_ACTION_LANDING = 1,  /*!< Aircraft will execute land cation when rc is lost. */
    UAV_FLIGHTCONTROL_RC_LOST_ACTION_GOHOME  = 2,  /*!< Aircraft will execute go-home cation when rc is lost. */
}E_UAVFlightControl_RCLost_Action;

/*
 * @note Attention:Enable emergency-stop-motor function is very dangerous in the air.it will make the aircraft crash!!!
 */
typedef enum {
    UAV_FLIGHTCONTROL_ENABLE_EMERGENCY_STOP_MOTOR = 1, /*!< Execute emergency-stop-motor action */
}E_UAVFlightControl_Emergency_Stop_Motor;

typedef struct {
    double latitude;    /*!< Specifies latitude, unit: rad */
    double longitude;   /*!< Specifies longitude, unit: rad */
    float altitude;     /*!< Specifies altitude, unit: m */
}T_UAVFlightControllerRidInfo;

extern T_UAVReturnCode UAV_FlightControl_Init(T_UAVFlightControllerRidInfo ridInfo);
extern void UAV_FlightControl_Deinit(void);

/**
 * @brief Set flight control mode.
 * @param mode: see reference of E_UAVFlithtControl_Mode.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_SetControlMode(E_UAVFlithtControl_Mode mode);

/**
 * @brief Get flight control mode.
 * @param mode: see reference of E_UAVFlithtControl_Mode.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_GetControlMode(E_UAVFlithtControl_Mode &mode);

typedef enum {
    UAV_HEADING_MODE_NONE = 0,   /*!< No heading mode. */
    UAV_HEADING_MODE_ALONG = 1,  /*!< Along the waypoint line direction. */
    UAV_HEADING_MODE_BY_PSDK = 2,/*!< Control the heading by psdk from user. */
}E_HEADING_MODE;

typedef enum {
    UAV_OBSTACLE_MODE_NONE = 0,   /*!< No obstacle avoidance. */
    UAV_OBSTACLE_MODE_LOITER = 1, /*!< Loiter mode. */
    UAV_OBSTACLE_MODE_OBS = 2,    /*!< Obstacle avoidance mode. */
}E_OBSTACLE_MODE;

typedef struct {
    float yaw;          /*!< Specifies yaw angle. */
    double latitude;    /*!< Specifies latitude. */
    double longitude;   /*!< Specifies longitude. */
    double altitude;    /*!< Specifies altitude. */
    int32_t obstacle_mode; /*!< Specifies obstacle mode, see E_OBSTACLE_MODE. */
}T_UAVFlightControlPos;

/**
 * @brief use position to control UAV.
 * @param pos: see reference of T_UAVFlightControlPos.
 * @return Execution result.
*/
extern T_UAVReturnCode UAV_FlightControl_POSControl(T_UAVFlightControlPos pos);

typedef struct {
    float x; /*!< Specifies x coordinate. */
    float y; /*!< Specifies y coordinate. */
    float z; /*!< Specifies z coordinate. */
    float yaw; /*!< Specifies yaw angle. */
    int32_t heading_mode;  /*!< Specifies heading mode, see E_HEADING_MODE. */
    int32_t obstacle_mode; /*!< Specifies obstacle mode, see E_OBSTACLE_MODE. */
}T_UAVFlightControlSpeed;

/**
 * @brief use speed to control UAV.
 * @param speed: see reference of T_UAVFlightControlSpeed.
 * @return Execution result.
*/
extern T_UAVReturnCode UAV_FlightControl_SpeedControl(T_UAVFlightControlSpeed speed);

/**
 * @brief Enable/Disable RTK position function.
 * @details Enabling RTK means that RTK data will be used instead of GPS during flight.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_SetRtkPositionEnableStatus(bool enable);

/**
 * @brief Get RTK enable status.
 * @note Enabling RTK means that RTK data will be used during intelligent flight.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_GetRtkPositionEnableStatus(bool &enable);

/**
 * @brief Set rc lost action.
 * @note It will be valid when rc and osdk is both lost.
 * @param rcLostAction: actions when rc is lost.(hover/landing/go home).
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_SetRCLostAction(E_UAVFlightControl_RCLost_Action rcLostAction);

/**
 * @brief Get rc lost action(hover/landing/gohome).
 * @note It will be valid when rc and osdk is both lost.
 * @param rcLostAction: see reference of E_UAVFlightControl_RCLost_Action.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_GetRCLostAction(E_UAVFlightControl_RCLost_Action &rcLostAction);

/**
 * @brief Turn on motors when the UAV is on the ground.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_TurnOnMotors(void);

/**
 * @brief Turn off motors when the UAV is on the ground.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_TurnOffMotors(void);

/**
 * @brief Emergency stop motor in any case.
 * @note If you want to turn on motor after emergency stopping motor, you need to use the interface to send disable
 * command to quit lock-motor status.
 * @param cmd: see reference of E_UAVFlightControl_Emergency_Stop_Motor
 * @param debugMsg:inject debug message to flight control FW for logging, size limit: 32 bytes
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_EmergencyStopMotor(E_UAVFlightControl_Emergency_Stop_Motor command, const char debugMsg[32]);

/**
 * @brief Request take off action when the UAV is on the ground.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_StartTakeoff(void);


/**
 * @brief Request landing action when the UAV is in the air.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_StartLanding(void);

/**
 * @brief Request cancel landing action when the UAV is landing
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_CancelLanding(void);

/**
 * @brief Force landing in any case.
 * @note This api will ignore the smart landing function,.When using this pi, it will landing directly (would not stop
 * at 0.7m and wait user's  command). Attention:it may make the aircraft crash!!!
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_StartForceLanding(void);

typedef struct {
    double latitude;  /*!< unit: rad */
    double longitude; /*!< unit: rad */
} T_UavFlightControllerHomeLocation; // pack(1)
/**
 * @brief Set customized GPS(not RTK) home location.
 * @note Set customized home location failed reason may as follows:
 * 1. The distance between new home location and last home location is larger than MAX_FLY_RADIUS(20km).
 * 2. Record initial home location failed after start aircraft.
 * @param homeLocation: homeLocation include latitude and longitude
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_SetHomeLocationUsingGPSCoordinates(T_UavFlightControllerHomeLocation homeLocation);


/**
 * @brief Set home location using current aircraft GPS(not RTK) location.
 * @note Set home location failed reasons may as follows:
 * 1. Aircraft's gps level can't reach the condition of recording home location.
 * 2. Record initial home location failed after start aircraft.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_SetHomeLocationUsingCurrentAircraftLocation(void);


typedef uint16_t E_UavFlightControllerGoHomeAltitude; /*!< Unit:meter, range 20~500 */
/**
 * @brief Set go home altitude.
 * @note If aircraft's current altitude is higher than the setting value of go home altitude, aircraft will go home
 * using current altitude. Otherwise, it will climb to setting of go home altitude ,and then execute go home action.
 * Go home altitude setting is 20m ~ 1500m.
 * @param altitude: go home altitude, unit: meter
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_SetGoHomeAltitude(E_UavFlightControllerGoHomeAltitude altitude);

/**
 * @brief Get go home altitude.
 * @param altitude: go home altitude, unit: meter
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_GetGoHomeAltitude(E_UavFlightControllerGoHomeAltitude *altitude);


/**
 * @brief Request go home action when the UAV is in the air
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_StartGoHome(void);

/**
 * @brief Request cancel go home action when the UAV is going home
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_CancelGoHome(void);

typedef struct {
    char serialNum[32];
} T_UavFlightControllerGeneralInfo;

/**
 * @brief Get general info of the aircraft.
 * @param generalInfo: the struct stored the serial num which contains a array of chars var in case the user gives an
 * illegal length character pointer
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_FlightControl_GetGeneralInfo(T_UavFlightControllerGeneralInfo *generalInfo);
#ifdef __cplusplus
}
#endif

#endif // __UAV_FLIGHT_CONTROL_HPP__
