#ifndef __UAV_FC_SUBSCRIBE_HPP__
#define __UAV_FC_SUBSCRIBE_HPP__
#include "uav_platform.h"
#ifdef __cplusplus
extern "C" {
#endif
/* *******************************************************************订阅消息类型枚举*****************************************************************************/
//订阅消息类型枚举

#define UAV_DATA_SUBSCRIPTION_MODULE               10000
#define UAV_DATA_SUBSCRIPTION_NUMBER               11000
#define UAV_DATA_SUBSCRIPTION_TOPIC(FC, num)       (FC + num)

typedef enum {
    /**
     * @brief Quaternion of aircraft topic name. Quaternion topic provides aircraft body frame (FRD) to ground frame
     * (NED) rotation. Please refer to ::T_UAVSubscriptionQuaternion for information about data structure.
     * @details The Autel quaternion follows Hamilton convention (q0 = w, q1 = x, q2 = y, q3 = z).
     * | Angle        | Unit | Accuracy   | Notes                                           |
       |--------------|------|------------|-------------------------------------------------|
       | pitch, roll  | deg  | <1         | in NON-AHRS mode                                |
       | yaw          | deg  | <3         | in well-calibrated compass with fine aligned    |
       | yaw with rtk | deg  | around 1.2 | in RTK heading fixed mode with 1 meter baseline |
     * @datastruct \ref T_UAVSubscriptionQuaternion
     */
    UAV_SUBSCRIPTION_TOPIC_QUATERNION = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 0),

    /**
     * @brief Provides aircraft's acceleration w.r.t a ground-fixed \b NEU frame @ up to 200Hz
     * @warning Please note that this data is not in a conventional right-handed frame of reference.
     * @details This is a fusion output from the flight control system. The output is in a right-handed NED frame, but the
     * sign of the Z-axis acceleration is flipped before publishing to this topic. So if you are looking to get acceleration
     * in an NED frame, simply flip the sign of the z-axis value. Beyond that, you can convert using rotations to
     * any right-handed frame of reference.
     * @units m/s<SUP>2</SUP>
     * @datastruct \ref T_UAVSubscriptionAccelerationGround
    */
    UAV_SUBSCRIPTION_TOPIC_ACCELERATION_GROUND = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 1),

    /**
     * @brief Provides aircraft's acceleration w.r.t a body-fixed \b FRU frame @ up to 200Hz
     * @warning Please note that this data is not in a conventional right-handed frame of reference.
     * @details This is a fusion output from the flight control system.
     * @units m/s<SUP>2</SUP>
     * @datastruct \ref T_UAVSubscriptionAccelerationBody
    */
    UAV_SUBSCRIPTION_TOPIC_ACCELERATION_BODY = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 2),

    /**
     * @brief Provides aircraft's acceleration in an IMU-centered, body-fixed \b FRD frame @ up to 400Hz
     * @details This is a filtered output from the IMU on board the flight control system.
     * @sensors IMU
     * @units m/s<SUP>2</SUP>
     * @datastruct \ref T_UAVSubscriptionAccelerationRaw
    */
    UAV_SUBSCRIPTION_TOPIC_ACCELERATION_RAW = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 3),

    /**
     * @brief Velocity of aircraft topic name. Velocity topic provides aircraft's velocity in a ground-fixed NEU frame.
     * @warning Please note that this data is not in a conventional right-handed frame of reference.
     * @details This velocity data is a fusion output from the aircraft. Original output is in a right-handed NED frame, but the
     * sign of the Z-axis velocity is flipped before publishing to this topic. So if you are looking to get velocity
     * in an NED frame, simply flip the sign of the z-axis value. Beyond that, you can convert using rotations to
     * any right-handed frame of reference.
     * | Axis     | Unit | Accuracy                                                                                    |
       |----------|------|---------------------------------------------------------------------------------------------|
       | vgx, vgy | m/s  | Around 5cm/s for GNSS navigation. Around 3cm/s with VO at 1 meter height                    |
       | vgz      | m/s  | 10cm/s only with barometer in steady air. 3cm/s with VO at 1 meter height with 8cm baseline |
    * @datastruct \ref T_UAVSubscriptionVelocity
    */
    UAV_SUBSCRIPTION_TOPIC_VELOCITY = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 4),

    /**
     * @brief Provides aircraft's angular velocity in a ground-fixed \b NED frame. up to 200Hz.
     * @details This is fusion output from the flight control system.
     * @units rad/s
     * @datastruct \ref T_UAVSubscriptionAngularRateFusioned
     */
    UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_FUSIONED = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 5),

    /**
     * @brief Provides aircraft's angular velocity in an IMU-centered, body-fixed \b FRD frame @ up to 400Hz
     * @details This is a filtered output from the IMU on board the flight control system.
     * @sensors IMU
     * @units rad/s
     * @datastruct \ref T_UAVSubscriptionAngularRateRaw
    */
    UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 6),

    /**
     * @brief Fused altitude of aircraft topic name. Fused altitude topic provides aircraft's fused altitude from sea level.
     * @units m
     * @datastruct \ref T_UAVSubscriptionAltitudeFused
    */
    UAV_SUBSCRIPTION_TOPIC_ALTITUDE_FUSED = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 7),


    UAV_SUBSCRIPTION_TOPIC_ALTITUDE_BAROMETER = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 8),

    UAV_SUBSCRIPTION_TOPIC_ALTITUDE_OF_HOMEPOINT = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 9),

    UAV_SUBSCRIPTION_TOPIC_HEIGHT_FUSION =  UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 10),
    UAV_SUBSCRIPTION_TOPIC_HEIGHT_RELATIVE      =  UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 11),

    /**
     * @brief Fused position of aircraft topic name. Please refer to ::T_UAVSubscriptionPositionFused for information
     * about data structure.
     * @warning Please note that if GPS signal is weak (low visibleSatelliteNumber, see below), the
     * latitude/longitude values won't be updated but the altitude might still be. There is currently no way to know if
     * the lat/lon update is healthy.
     * @details The most important component of this topic is the T_UAVSubscriptionPositionFused::visibleSatelliteNumber.
     * Use this to track your GPS satellite coverage and build some heuristics for when you might expect to lose GPS updates.
     *   | Axis | Unit | Position Sensor | Accuracy                                         |
         |------|------|-----------------|--------------------------------------------------|
         | x, y | m    | GPS             | <3m with open sky without multipath              |
         | z    | m    | GPS             | <5m with open sky without multipath              |
     * 数据结构： T_UAVSubscriptionPositionFused
     */
    UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 12),

    /**
     * @brief GPS date topic name. Please refer to ::T_UAVSubscriptionGpsDate for information about data structure.
    */
    UAV_SUBSCRIPTION_TOPIC_GPS_DATE = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 13),

    /**
     * @brief GPS time topic name. Please refer to ::T_UAVSubscriptionGpsTime for information about data structure.
    */
    UAV_SUBSCRIPTION_TOPIC_GPS_TIME = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 14),

    /**
     * @brief GPS position topic name.
     * @details
     *   | Axis | Accuracy                                         |
         |------|--------------------------------------------------|
         | x, y | <3m with open sky without multipath              |
         | z    | <5m with open sky without multipath              |
     * @datastruct \ref T_UAVSubscriptionGpsPosition
    */
    UAV_SUBSCRIPTION_TOPIC_GPS_POSITION = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 15),

    /**
     * @brief GPS velocity topic name.
     * @datastruct \ref T_UAVSubscriptionGpsVelocity
    */
    UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 16),

    UAV_SUBSCRIPTION_TOPIC_GPS_DETAILS = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 17),
    /**
     * @brief GPS signal level topic name. This topic provides a measure of the quality of GPS signal.
     * @datastruct \ref T_UAVSubscriptionGpsSignalLevel
    */
    UAV_SUBSCRIPTION_TOPIC_GPS_SIGNAL_LEVEL = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 18),

    /**
     * @brief RTK position topic name.
     * @details
     *   | Axis | Accuracy                                         |
     *   |------|--------------------------------------------------|
     *   | x, y | ~2cm with fine alignment and fix condition       |
     *   | z    | ~3cm with fine alignment and fix condition       |
     * @datastruct \ref T_UAVSubscriptionRTKPosition
    */
    UAV_SUBSCRIPTION_TOPIC_RTK_POSITION = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 19),

    /**
     * @brief RTK velocity topic name.
     * @datastruct \ref T_UAVSubscriptionRTKVelocity
    */
    UAV_SUBSCRIPTION_TOPIC_RTK_VELOCITY = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 20),

    /**
     * @brief RTK yaw topic name.
     * @details The RTK yaw will provide the vector from ANT1 to ANT2 as configured in UAV Assistant 2. This
     * means that the value of RTK yaw will be 90deg offset from the yaw of the aircraft.
     * @datastruct \ref T_UAVSubscriptionRTKYaw
    */
    UAV_SUBSCRIPTION_TOPIC_RTK_YAW = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 21),

    UAV_SUBSCRIPTION_TOPIC_RTK_POSITION_INFO = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 22),
    UAV_SUBSCRIPTION_TOPIC_RTK_YAW_INFO = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 23),
    UAV_SUBSCRIPTION_TOPIC_COMPASS = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 24),
    UAV_SUBSCRIPTION_TOPIC_RC = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 25),


    /*
     * @brief Provides gimbal pitch, roll, yaw @ up to 50Hz
     * @details The reference frame for gimbal angles is a NED frame attached to the gimbal.
     * This topic uses a data structure, Vector3f, that is too generic for the topic. The order of angles is :
     * |Data Structure Element| Meaning|
     * |----------------------|--------|
     * |Vector3f.x            |pitch   |
     * |Vector3f.y            |roll    |
     * |Vector3f.z            |yaw     |
     * 
     * @ units deg
     * @perf 0.1 deg accuracy in all axes
     * @datastruct \ref T_UAVSubscriptionGimbalAngle
    */
   UAV_SUBSCRIPTION_TOPIC_GIMBAL_ANGLE = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 26),
   UAV_SUBSCRIPTION_TOPIC_GIMBAL_STATUS = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 27),
    /**
     * @brief Flight status topic name.
     * @datastruct \ref T_UAVSubscriptionFlightStatus
    */
    UAV_SUBSCRIPTION_TOPIC_STATUS_FLIGHT = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 28),

    /***
     * @brief Provides a granular state representation for various tasks/flight modes @ up to 50Hz
     * @details Typically, use this topic together with \ref UAV_SUBSCRIPTION_TOPIC_STATUS_FLIGHT 
     * to get a better understanding of the overall status of the aircraft.
     * @datastruct \ref T_UAVSubscriptionDisplaymode
    */
    UAV_SUBSCRIPTION_TOPIC_STATUS_DISPLAYMODE = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 29),

    /**
     * @brief Provides status for the landing gear state @ up to 50Hz
     * @datastruct \ref T_UAVSubscriptionLandinggear
    */
    UAV_SUBSCRIPTION_TOPIC_STATUS_LANDINGGEAR = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 30),
    UAV_SUBSCRIPTION_TOPIC_STATUS_MOTOR_START_ERROR = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 31),
    /**
     * @brief Battery information topic name.
     * @datastruct \ref T_UAVSubscriptionWholeBatteryInfo
    */
    UAV_SUBSCRIPTION_TOPIC_BATTERY_INFO = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 32),


    UAV_SUBSCRIPTION_TOPIC_HARD_SYNC = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 34),
    UAV_SUBSCRIPTION_TOPIC_GPS_CONTROL_LEVEL = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 35),
    UAV_SUBSCRIPTION_TOPIC_RC_WITH_FLAG_DATA = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 36),
    UAV_SUBSCRIPTION_TOPIC_ESC_DATA          = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 37),
    UAV_SUBSCRIPTION_TOPIC_RTK_CONNECT_STATUS   = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 38),

    /*
     * @brief Provides the mode in which the gimbal will interpret control commands @ up to 50Hz
     * @details This topic will report the current control mode which can be set in the
     * @datastruct \ref T_UAVSubscriptionGimbalControlMode
    */
    UAV_SUBSCRIPTION_TOPIC_GIMBAL_CONTROL_MODE = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 39),
    UAV_SUBSCRIPTION_TOPIC_POSITION_VO = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 41),
    UAV_SUBSCRIPTION_TOPIC_AVOID_DATA = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 42),
    UAV_SUBSCRIPTION_TOPIC_HOME_POINT_SET_STATUS  = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 43),
    UAV_SUBSCRIPTION_TOPIC_HOME_POINT_INFO = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 44),
    UAV_SUBSCRIPTION_TOPIC_GIMBAL_THREE_DATA = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 45),
    UAV_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INDEX1 = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 46),
    UAV_SUBSCRIPTION_TOPIC_BATTERY_SINGLE_INDEX2 = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 47),
    UAV_SUBSCRIPTION_TOPIC_IMU_ATTI_NAVI_WITH_TIMESTAMP = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_MODULE, 48),

    /**
     * @brief Flight control mode topic name.
     * @datastruct \ref T_UAVSubscriptionFlightControlMode
    */
    UAV_SUBSCRIPTION_TOPIC_FLIGHT_CTL_MODE = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_NUMBER, 0),

    /**
     * @brief Euler Angular information topic name. Please refer to ::T_UAVSubscriptionEulerAngular for information about data structure.
     * @datastruct \ref T_UAVSubscriptionEulerAngular
    */
    UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_NUMBER, 1),

    /**
     * @brief notify payload that the aircraft will power off soon.
     * @datastruct \ref T_UAVSubscriptionUAVPoweroff
    */
    UAV_SUBSCRIPTION_TOPIC_UAV_POWER_OFF = UAV_DATA_SUBSCRIPTION_TOPIC(UAV_DATA_SUBSCRIPTION_NUMBER, 2),

    /** Total number of topics that can be subscribed。 */
    UAV_SUBSCRIPTION_TOPIC_TOTAL_NUMBER,
} E_UAVSubscriptionTopic;


/* *******************************************************************订阅消息结构体定义*****************************************************************************/
/// @brief UAV_SUBSCRIPTION_TOPIC_QUATERNION topic data structure.
typedef struct Quaternion {
    float q0;           /*!< w, rad (when converted to a rotation matrix or Euler angles). */
    float q1;           /*!< x, rad (when converted to a rotation matrix or Euler angles). */
    float q2;           /*!< y, rad (when converted to a rotation matrix or Euler angles). */
    float q3;           /*!< z, rad (when converted to a rotation matrix or Euler angles). */
} T_UAVSubscriptionQuaternion;

/// @brief UAV_SUBSCRIPTION_TOPIC_VELOCITY topic data structure.
typedef enum {
    UAV_SUBSCRIPTION_DATA_NOT_HEALTHY = 0, /*!< Data subscribed is healthy and can be used. */
    UAV_SUBSCRIPTION_DATA_HEALTHY = 1,     /*!< Data subscribed is not healthy and recommend not to use it. */
}E_UAVSubscriptionDataHealthFlag;

typedef struct Velocity {
    T_UAVVector3f data; /*!< Specifies the velocity vector. */
    uint16_t health: 1; /*!< Specifies the health of the velocity data. see E_UAVSubscriptionDataHealthFlag */
    uint16_t Reserved: 15; /*!< Reserved. */
}T_UAVSubscriptionVelocity;

/// @brief UAV_SUBSCRIPTION_TOPIC_ACCELERATION_GROUND topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionAccelerationGround;
/// @brief UAV_SUBSCRIPTION_TOPIC_ACCELERATION_BODY topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionAccelerationBody;
/// @brief UAV_SUBSCRIPTION_TOPIC_ACCELERATION_RAW topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionAccelerationRaw;
/// @brief UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_FUSIONED topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionAngularRateFusioned;
/// @brief UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionAngularRateRaw;
/// @brief UAV_SUBSCRIPTION_TOPIC_ALTITUDE_FUSED topic data structure.
typedef float T_UAVSubscriptionAltitudeFused;

typedef float T_UAVFcSubscriptionAltitudeBarometer;
typedef float T_UAVFcSubscriptionHeightFusion;
typedef float T_UAVFcSubscriptionHeightRelative;
typedef uint8_t  T_UAVFcSubscriptionRtkPositionInfo;

typedef struct GpsDetail {
    float hdop; /*!< Horizontal dilution of precision, unit: 0.01, eg: 100 = 1.00, <1: ideal, 1-2: excellent, 2-5: good, 5-10: moderate, 10-20: fair, >20: poor. */
    float pdop; /*!< Position dilution of precision, unit: 0.01, eg: 100 = 1.00, <1: ideal, 1-2: excellent, 2-5: good, 5-10: moderate, 10-20: fair, >20: poor. */
    float fixState; /*!< GPS fix state, and can be any value of ::E_UAV_FcSubscriptionGpsFixState. Value other than ::E_UAV_FcSubscriptionGpsFixState is invalid. */
    float vacc; /*!< Vertical position accuracy (mm), the smaller, the better. */
    float hacc; /*!< Horizontal position accuracy (mm), the smaller, the better. */
    float sacc; /*!< Speed accuracy (cm/s), the smaller, the better. */
    uint32_t gpsSatelliteNumberUsed; /*!< Number of GPS satellites used for fixing position. */
    uint32_t glonassSatelliteNumberUsed; /*!< Number of GLONASS satellites used for fixing position. */
    uint16_t totalSatelliteNumberUsed; /*!< Total number of satellites used for fixing position. */
    uint16_t gpsCounter; /*!< Accumulated times of sending GPS data. */
} T_UAVFcSubscriptionGpsDetails;


typedef struct RC {
    int16_t roll;     /*!< [-10000,10000] */
    int16_t pitch;    /*!< [-10000,10000] */
    int16_t yaw;      /*!< [-10000,10000] */
    int16_t throttle; /*!< [-10000,10000] */
    int16_t mode;     /*!< [-10000,10000] */
    /*!< M100 [P: -8000, A: 0, F: 8000] */
    int16_t gear;     /*!< [-10000,10000] */
    /*!< M100 [Up: -10000, Down: -4545] */
} T_UAVFcSubscriptionRC;

typedef uint16_t T_UAVFcSubscriptionMotorStartError;

typedef struct SyncTimestamp {
    uint32_t time2p5ms; /*!< clock time in multiples of 2.5ms. Sync timer runs at 400Hz, this field increments in integer steps */
    uint32_t time1ns;   /*!< nanosecond time offset from the 2.5ms pulse */
    uint32_t resetTime2p5ms; /*!< clock time in multiple of 2.5ms elapsed since the hardware sync started */
    uint16_t index;   /*!< This is the tag field you filled out when using the
                       setSyncFreq API above; use it to identify the packets that
                       have sync data. This is useful when you call the
                       setSyncFreq API with freqInHz = 0, so you get a single
                       pulse that can be uniquely identified with a tag - allowing
                       you to create your own pulse train with uniquely
                       identifiable pulses. */
    uint8_t flag;     /*!< This is true when the packet corresponds to a hardware
                       pulse and false otherwise. This is useful because you can
                       request the software packet to be sent at a higher frequency
                       that the hardware line.*/
} SyncTimestamp;    // pack(1)

typedef struct HardSyncData {
    SyncTimestamp ts;  /*!< time stamp for the incoming data */
    struct Quaternion q;  /*!< quaternion */
    T_UAVVector3f a;  /*!< accelerometer reading unit: g */
    T_UAVVector3f w;  /*!< gyro reading unit: rad/sec */
} T_UAVFcSubscriptionHardSync;

typedef uint8_t T_UAVFcSubscriptionGpsControlLevel;

typedef struct RCWithFlagData {
    float pitch;       /*!< down = -0.999, middle = 0.000, up   =0.999 */
    float roll;        /*!< left = -0.999, middle = 0.000, right=0.999 */
    float yaw;         /*!< left = -0.999, middle = 0.000, right=0.999 */
    float throttle;    /*!< down = -0.999, middle = 0.000, up   =0.999 */
    struct {
        uint8_t logicConnected: 1;  /*!< 0 if sky or ground side is disconnected for 3 seconds   */
        uint8_t skyConnected: 1;  /*!< Sky side is connected, i.e., receiver is connected to FC */
        uint8_t groundConnected: 1;  /*!< Ground side is connected, i.e., RC is on and connected to FC */
        uint8_t appConnected: 1;  /*!< Mobile App is connected to RC */
        uint8_t reserved: 4;
    } flag;
} T_UAVFcSubscriptionRCWithFlagData;

typedef struct ESCStatusIndividual {
    int16_t current;              /*!< ESC current, unit: mA */
    int16_t speed;                /*!< ESC speed, unit: rpm */
    uint16_t voltage;              /*!< Input power from battery to ESC, unit: mV */
    int16_t temperature;          /*!< ESC temperature, unit: degree C */
    uint16_t stall: 1; /*!< Motor is stall */
    uint16_t empty: 1; /*!< Motor has no load */
    uint16_t unbalanced: 1; /*!< Motor speed is unbalanced */
    uint16_t escDisconnected: 1; /*!< ESC is disconnected */
    uint16_t temperatureHigh: 1; /*!< Temperature is high */
    uint16_t reserved: 11;
} T_UAVESCStatusIndividual;

typedef struct EscData {
    T_UAVESCStatusIndividual esc[8];
} T_UAVFcSubscriptionEscData;



typedef struct PositionVO {
    float x;              /*!< North (best effort), unit: m */
    float y;              /*!< East (best effort),  unit: m */
    float z;              /*!< Down,  unit: m */
    uint8_t xHealth: 1;
    uint8_t yHealth: 1;
    uint8_t zHealth: 1;
    uint8_t reserved: 5;
} T_UAVFcSubscriptionPositionVO;

typedef struct RelativePosition {
    float down;            /*!< distance from obstacle (m) */
    float front;           /*!< distance from obstacle (m) */
    float right;           /*!< distance from obstacle (m) */
    float back;            /*!< distance from obstacle (m) */
    float left;            /*!< distance from obstacle (m) */
    float up;              /*!< distance from obstacle (m) */
    uint8_t downHealth: 1;  /*!< Down sensor flag: 0 - not working, 1 - working */
    uint8_t frontHealth: 1; /*!< Front sensor flag: 0 - not working, 1 - working */
    uint8_t rightHealth: 1; /*!< Right sensor flag: 0 - not working, 1 - working */
    uint8_t backHealth: 1;  /*!< Back sensor flag: 0 - not working, 1 - working */
    uint8_t leftHealth: 1;  /*!< Left sensor flag: 0 - not working, 1 - working */
    uint8_t upHealth: 1;    /*!< Up sensor health flag: 0 - not working, 1 - working */
    uint8_t reserved: 2;    /*!< Reserved sensor health flag*/
} T_UAVFcSubscriptionAvoidData;

typedef struct {
    uint32_t reserved: 12;
    uint32_t cellBreak: 5;            /*!< 0: normal; other: under voltage core index(0x01-0x1F). */
    uint32_t selfCheckError: 3;       /*!< enum-type: E_UAVFcSubscriptionBatterySelfCheck. */
    uint32_t reserved1: 7;
    uint32_t batteryClosedReason: 5;  /*!< enum-type: E_UAVFcSubscriptionBatteryClosedReason. */
    uint8_t reserved2: 6;             /*!< 0: CHG state; 1: DSG state; 2: ORING state. */
    uint8_t batSOHState: 2;           /*!< enum-type: E_UAVFcSubscriptionBatterySohState. */
    uint8_t maxCycleLimit: 6;         /*!< APP: cycle_limit*10. */
    uint8_t reserved3: 2;
    uint16_t lessBattery: 1;
    uint16_t batteryCommunicationAbnormal: 1;
    uint16_t reserved4: 3;
    uint16_t hasCellBreak: 1;
    uint16_t reserved5: 4;
    uint16_t isBatteryEmbed: 1;       /*!< 0:embed; 1:not embed. */
    uint16_t heatState: 2;            /*!< enum-type: E_UAVFcSubscriptionBatteryHeatState. */
    uint16_t socState: 3;             /*!< enum-type: E_UAVFcSubscriptionBatterySocState. */
} T_UAVFcSubscriptionSingleBatteryState;

typedef struct BatterySingleInfo {
    uint8_t reserve;
    uint8_t batteryIndex;
    int32_t currentVoltage;          /*!< uint: mV. */
    int32_t currentElectric;         /*!< uint: mA. */
    uint32_t fullCapacity;           /*!< uint: mAh. */
    uint32_t remainedCapacity;       /*!< uint: mAh. */
    int16_t batteryTemperature;      /*!< uint: 0.1℃. */
    uint8_t cellCount;
    uint8_t batteryCapacityPercent;  /*!< uint: %. */
    T_UAVFcSubscriptionSingleBatteryState batteryState;
    uint8_t reserve1;
    uint8_t reserve2;
    uint8_t SOP;                     /*!< Relative power percentage. */
} T_UAVFcSubscriptionSingleBatteryInfo;

typedef struct ImuAttiNaviDataWithTimestamp {
    uint16_t version;
    uint16_t flag;
    float pn_x;
    float pn_y;
    float pn_z;
    float vn_x;
    float vn_y;
    float vn_z;
    float an_x;
    float an_y;
    float an_z;
    float q[4];
    uint16_t resv;
    uint16_t cnt;
    uint32_t timestamp;
} T_UAVFcSubscriptionImuAttiNaviDataWithTimestamp;

typedef float T_UAVFcSubscriptionAltitudeOfHomePoint;
typedef uint8_t T_UAVFcSubscriptionRtkYawInfo;

typedef struct Mag {
    int16_t x;
    int16_t y;
    int16_t z;
} T_UAVFcSubscriptionCompass;

typedef struct RTKConnectStatus {
    uint16_t rtkConnected: 1;
    uint16_t reserve: 15;
} T_UAVFcSubscriptionRTKConnectStatus;
typedef uint8_t T_UAVFcSubscriptionHomePointSetStatus;

typedef struct HomeLocationData {
    float latitude;  /*!< unit: rad */
    float longitude; /*!< unit: rad */
} T_UAVFcSubscriptionHomePointInfo;

/// @brief UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED topic data structure.
typedef struct PositionFused {
    double longitude; /*!< Longitude, unit: rad. */
    double latitude;  /*!< Latitude, unit: rad. */
    float  altitude;  /*!< Altitude, WGS 84 reference ellipsoid, unit: m. */
    uint16_t visibleSatelliteNumber; /*!< Number of visible satellites. */
} T_UAVSubscriptionPositionFused;

/// @brief UAV_SUBSCRIPTION_TOPIC_GPS_DATE topic data structure.
typedef uint32_t T_UAVSubscriptionGpsDate;
/// @brief UAV_SUBSCRIPTION_TOPIC_GPS_TIME topic data structure.
typedef uint32_t T_UAVSubscriptionGpsTime;
/// @brief UAV_SUBSCRIPTION_TOPIC_GPS_POSITION topic data structure.
typedef T_UAVVector3d T_UAVSubscriptionGpsPosition;
/// @brief UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionGpsVelocity;
/// @brief UAV_SUBSCRIPTION_TOPIC_GPS_SIGNAL_LEVEL topic data structure.
typedef uint8_t T_UAVSubscriptionGpsSignalLevel;
/// @brief UAV_SUBSCRIPTION_TOPIC_RTK_POSITION topic data structure.
typedef struct {
    double longitude; /*!< Longitude, unit: deg. */
    double latitude;  /*!< Latitude, unit: deg. */
    float  altitude;  /*!< Height above mean sea level, unit: m. */
}T_UAVSubscriptionRTKPosition;
/// @brief UAV_SUBSCRIPTION_TOPIC_RTK_VELOCITY topic data structure. unit: cm/s
typedef T_UAVVector3f T_UAVSubscriptionRTKVelocity;
/// @brief UAV_SUBSCRIPTION_TOPIC_RTK_YAW topic data structure, unit: deg.
typedef int16_t T_UAVSubscriptionRTKYaw;
typedef enum {
    UAV_SUBSCRIPTION_FLIGHT_STATUS_STOPED    = 0, /*!< Aircraft is on ground and motors are still. */
    UAV_SUBSCRIPTION_FLIGHT_STATUS_ON_GROUND = 1, /*!< Aircraft is on ground and motors are running. */
    UAV_SUBSCRIPTION_FLIGHT_STATUS_IN_AIR    = 2, /*!< Aircraft is in the air. */
}E_UAVSubscriptionFlightStatus;
/// @brief UAV_SUBSCRIPTION_TOPIC_GIMBAL_ANGLE topic data structure.
typedef T_UAVVector3f T_UAVSubscriptionGimbalAngle;
/// @brief UAV_SUBSCRIPTION_TOPIC_STATUS_FLIGHT topic data structure.
typedef uint8_t T_UAVSubscriptionFlightStatus;  ///< E_UAVSubscriptionFlightStatus
/// @brief UAV_SUBSCRIPTION_TOPIC_STATUS_DISPLAYMODE topic data structure.
typedef uint8_t T_UAVSubscriptionDisplaymode;
/// @brief UAV_SUBSCRIPTION_TOPIC_STATUS_LANDINGGEAR topic data structure.
typedef uint8_t T_UAVSubscriptionLandinggear;

/// @brief UAV_SUBSCRIPTION_TOPIC_BATTERY_INFO topic data structure.
typedef struct {
    int32_t capacity;   /*!< Specifies the battery capacity in mAh. */
    int32_t voltage;    /*!< Specifies the battery voltage in mV. */
    int32_t current;    /*!< Specifies the battery current in mA. */
    uint8_t percentage; /*!< Specifies the battery percentage, unit: 1% */
}T_UAVSubscriptionWholeBatteryInfo;

/// @brief UAV_SUBSCRIPTION_TOPIC_GIMBAL_CONTROL_MODE topic data structure.
typedef uint8_t T_UAVSubscriptionGimbalControlMode;

typedef enum {
    UAV_SUBSCRIPTION_FLIGHT_CONTROL_MODE_UNKNOWN = 0, /*!< unknown control mode. */
    UAV_SUBSCRIPTION_FLIGHT_CONTROL_MODE_APP = 1,     /*!< remote control control mode. */
    UAV_SUBSCRIPTION_FLIGHT_CONTROL_MODE_CLOUD = 2,   /*!< cloud control mode. */
    UAV_SUBSCRIPTION_FLIGTH_CONTROL_MODE_PAYLOAD = 3, /*!< payload control mode. */
}E_UAVSubscriptionFlightControlMode;
///< @brief UAV_SUBSCRIPTION_TOPIC_FLIGHT_CTL_MODE topic data structure.
typedef uint8_t T_UAVSubscriptionFlightControlMode;  ///< E_UAVSubscriptionFlightControlMode
///< @brief UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO topic data structure.
typedef T_UAVAttitude3f T_UAVSubscriptionEulerAngular;
///< @brief UAV_SUBSCRIPTION_TOPIC_UAV_POWER_OFF topic data structure.
typedef uint32_t T_UAVSubscriptionUAVPoweroff;

/* *******************************************************************订阅消息函数接口*****************************************************************************/
typedef void (*UavReceiveDataOfTopicCallback)(void *data, int len);
T_UAVReturnCode UAV_Subscription_Init(void);
T_UAVReturnCode UAV_Subscription_DeInit(void);
T_UAVReturnCode UAV_SubscribeTopic(E_UAVSubscriptionTopic topicType, uint32_t frequency, UavReceiveDataOfTopicCallback usrCb);
T_UAVReturnCode UAV_unSubscribeTopic(E_UAVSubscriptionTopic topicType);


#ifdef __cplusplus
}
#endif

#endif // __UAV_SUBSCRIBE_HPP__
