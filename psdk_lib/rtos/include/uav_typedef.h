#ifndef __UAV_TYPEDEF_H__
#define __UAV_TYPEDEF_H__


#ifdef __cplusplus
extern "C" {

#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define UAV_PI                     (3.14159265358979323846f)

/**
 * @brief Type define double as uav_f64_t.
 */
typedef double uav_f64_t;
/**
 * @brief Type define float as uav_f32_t.
 */
typedef float uav_f32_t;
/**
 * @brief Type define uint64 as T_UAVReturnCode.
 * @details The type can be any value of ::T_UAVReturnCode.
 */
typedef uint64_t T_UAVReturnCode;

#ifndef nulllptr
#define nullptr NULL
#endif

/**
 * @brief Subscription frequency type
 */
typedef enum {
    UAV_DATA_SUBSCRIPTION_TOPIC_1_HZ   = 1,   /*!< 1Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_5_HZ   = 5,   /*!< 5Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_10_HZ  = 10,  /*!< 10Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_20_HZ  = 20,  /*!< 20Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_50_HZ  = 50,  /*!< 50Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_100_HZ = 100, /*!< 100Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_200_HZ = 200, /*!< 200Hz */
    UAV_DATA_SUBSCRIPTION_TOPIC_400_HZ = 400, /*!< 400Hz */
}E_UAVDataSubscriptionTopicFrequency;

typedef enum {
    UAV_MOUNT_POSITION_UNKNOWN = 0,
    UAV_MOUNT_POSITION_PAYLOAD_PORT_NO1 = 1,
    UAV_MOUNT_POSITION_PAYLOAD_PORT_NO2 = 2,
    UAV_MOUNT_POSITION_PAYLOAD_PORT_NO3 = 3,
} E_UAVMountPosition;

/**
 * @brief Gimbal work mode, specifies how gimbal follow aircraft movement.
 */
typedef enum {
    UAV_GIMBAL_MODE_FREE = 0, /*!< Free mode, fix gimbal attitude in the ground coordinate, ignoring movement of aircraft. */
    UAV_GIMBAL_MODE_FPV = 1, /*!< FPV (First Person View) mode, only control roll and yaw angle of gimbal in the ground coordinate to follow aircraft. */
    UAV_GIMBAL_MODE_YAW_FOLLOW = 2, /*!< Yaw follow mode, only control yaw angle of gimbal in the ground coordinate to follow aircraft. */
} E_UAVGimbalMode;

/**
 * @brief Gimbal rotation mode, specifies control style.
 */
typedef enum {
    UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE = 0, /*!< Relative angle rotation mode, represents rotating gimbal specified angles based on current angles. */
    UAV_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE = 1, /*!< Absolute angle rotation mode, represents rotating gimbal to specified angles in the ground coordinate. */
} E_UAVGimbalRotationMode;

#pragma pack(1)

typedef struct Vector3d {
    int32_t x; /*!< Specifies int32 value of x for vector. */
    int32_t y; /*!< Specifies int32 value of y for vector. */
    int32_t z; /*!< Specifies int32 value of z for vector. */
}T_UAVVector3d;

typedef struct Vector3f {
    float x; /*!< Specifies float value of x for vector. */
    float y; /*!< Specifies float value of y for vector. */
    float z; /*!< Specifies float value of z for vector. */
}T_UAVVector3f;

typedef struct {
    int32_t pitch; /*!< Specifies int32 value of pitch for attitude. */
    int32_t roll;  /*!< Specifies int32 value of roll for attitude. */
    int32_t yaw;   /*!< Specifies int32 value of yaw for attitude. */
}T_UAVAttitude3d;

typedef struct {
    float pitch; /*!< Specifies float value of pitch for attitude. */
    float roll;  /*!< Specifies float value of roll for attitude. */
    float yaw;   /*!< Specifies float value of yaw for attitude. */
}T_UAVAttitude3f;

typedef struct {
    float q0; /*!< w, when converted to a rotation matrix or Euler angles. */
    float q1; /*!< x, when converted to a rotation matrix or Euler angles. */
    float q2; /*!< y, when converted to a rotation matrix or Euler angles. */
    float q3; /*!< z, when converted to a rotation matrix or Euler angles. */
}T_UAVQuaternion4f;

/**
 * @brief Timestamp data struct
 */
typedef struct {
    int64_t millisecond; /*!< Specifies the milli seconds part of the timestamp. */
    int32_t microsecond;   /*!< Specifies the micro seconds part of the timestamp. */
}T_UAVDataTimestamp;

typedef enum {
    UAV_MOUNT_POSITION_TYPE_UNKNOWN = 0,
    UAV_MOUNT_POSITION_TYPE_PAYLOAD_PORT = 1,
    UAV_MOUNT_POSITION_TYPE_EXTENSION_PORT = 2,
    UAV_MOUNT_POSITION_TYPE_EXTENSION_LITE_PORT = 3
} E_UAVMountPositionType;
/**
 * @brief Camera work mode.
 */
typedef enum {
    UAV_CAMERA_MODE_SHOOT_PHOTO = 0, /*!< Shoot photo work mode. */
    UAV_CAMERA_MODE_RECORD_VIDEO = 1, /*!< Record video work mode. */
    UAV_CAMERA_MODE_PLAYBACK = 2, /*!< Media playback work mode. */
} E_UAVCameraMode;

/**
 * @brief Camera focus mode.
 */
typedef enum {
    UAV_CAMERA_FOCUS_MODE_MANUAL = 0, /*!< Manual focus mode. */
    UAV_CAMERA_FOCUS_MODE_AUTO = 1, /*!< Auto focus mode. */
} E_UAVCameraFocusMode;

/**
 * @brief Camera focus target point when in focus mode.
 */
typedef struct {
    uav_f32_t focusX; /*!< Specifies horizontal zone coordinate. This parameter is between 0 and 1.
                            The point [0.0, 0.0] represents the top-left angle of the screen.*/
    uav_f32_t focusY; /*!< Specifies vertical zone coordinate. This parameter is between 0 and 1. */
} T_UAVCameraPointInScreen;

/**
 * @brief Camera time interval settings when in interval shootPhoto mode.
 */
typedef struct {
    uint8_t captureCount; /*!< Specifies the total capture count of interval settings.
 *                             0:reserve 1~254:number 255:keep capturing till stop */
    uint16_t timeIntervalSeconds; /*!< Specifies the interval time between two captures, unit: s*/
} T_UAVCameraPhotoTimeIntervalSettings;

/**
 * @brief Camera zoom speed.
 */
typedef enum {
    UAV_CAMERA_ZOOM_SPEED_SLOWEST = 72, /*!< Lens zooms in slowest speed. */
    UAV_CAMERA_ZOOM_SPEED_SLOW = 73, /*!< Lens zooms in slow speed. */
    UAV_CAMERA_ZOOM_SPEED_MODERATELY_SLOW = 74, /*!< Lens zooms in speed slightly slower than normal speed. */
    UAV_CAMERA_ZOOM_SPEED_NORMAL = 75, /*!< Lens zooms in normal speed. */
    UAV_CAMERA_ZOOM_SPEED_MODERATELY_FAST = 76, /*!< Lens zooms very in speed slightly faster than normal speed. */
    UAV_CAMERA_ZOOM_SPEED_FAST = 77, /*!< Lens zooms very in fast speed. */
    UAV_CAMERA_ZOOM_SPEED_FASTEST = 78, /*!< Lens zooms very in fastest speed. */
} E_UAVCameraZoomSpeed;

typedef enum {
    /*! The number of pictures to continuously take each time in BURST mode is 2
     */
    UAV_CAMERA_BURST_COUNT_2 = 2,
    /*! The number of pictures to continuously take each time in BURST mode is 3
     */
    UAV_CAMERA_BURST_COUNT_3 = 3,
    /*! The number of pictures to continuously take each time in BURST mode is 5
     */
    UAV_CAMERA_BURST_COUNT_5 = 5,
    /*! The number of pictures to continuously take each time in BURST mode is 7
     */
    UAV_CAMERA_BURST_COUNT_7 = 7,
    /*! The number of pictures to continuously take at one time in BURST mode is
     * 10, Only supported by X4S camera, X5S camera and Phantom 4 Pro camera.
     */
    UAV_CAMERA_BURST_COUNT_10 = 10,
    /*! The number of pictures to continuously take at one time in BURST mode is
     * 14, Only supported by X4S camera, X5S camera and Phantom 4 Pro camera.
     */
    UAV_CAMERA_BURST_COUNT_14 = 14,
    /*!	The camera burst shoot count value is unknown.
     */
    UAV_CAMERA_BURST_COUNT_KNOWN = 0xFF,
} E_UAVCameraBurstCount;

/**
 * @brief Camera zoom direction.
 */
typedef enum {
    UAV_CAMERA_ZOOM_DIRECTION_OUT = 0, /*!< The lens moves in the far direction, the zoom factor becomes smaller. */
    UAV_CAMERA_ZOOM_DIRECTION_IN = 1, /*!< The lens moves in the near direction, the zoom factor becomes larger. */
} E_UAVCameraZoomDirection;

/**
 * @brief Camera type.
 */
typedef enum {
    UAV_CAMERA_TYPE_UNKNOWN = 0, /*!< Camera type is unknown. */
    UAV_CAMERA_TYPE_Z30 = 20, /*!< Camera type is Z30. */
    UAV_CAMERA_TYPE_XT2 = 26, /*!< Camera type is XT2. */
    UAV_CAMERA_TYPE_PSDK = 31, /*!< Camera type is third party camera based on Payload SDK. */
    UAV_CAMERA_TYPE_XTS = 41, /*!< Camera type is XT S. */
    UAV_CAMERA_TYPE_H20 = 42, /*!< Camera type is H20. */
    UAV_CAMERA_TYPE_H20T = 43, /*!< Camera type is H20T. */
    UAV_CAMERA_TYPE_H20N = 61, /*!< Camera type is H20N. */
    UAV_CAMERA_TYPE_P1 = 50, /*!< Camera type is P1. */
    UAV_CAMERA_TYPE_L1, /*!< Camera type is L1. */
    UAV_CAMERA_TYPE_L2, /*!< Camera type is L2. */
    UAV_CAMERA_TYPE_M30, /*!< Camera type is M30. */
    UAV_CAMERA_TYPE_M30T, /*!< Camera type is M30T. */
    UAV_CAMERA_TYPE_M3E, /*!< Camera type is M3E. */
    UAV_CAMERA_TYPE_M3T, /*!< Camera type is M3T. */
    UAV_CAMERA_TYPE_M3D, /*!< Camera type is Matrice 3D. */
    UAV_CAMERA_TYPE_M3TD, /*!< Camera type is Matrice 3TD. */
} E_UAVCameraType;

/**
 * @brief Camera supported media file type.
 */
typedef enum {
    UAV_CAMERA_FILE_TYPE_JPEG = 0, /*!< Media file JPEG type. */
    UAV_CAMERA_FILE_TYPE_DNG = 1, /*!< Media file DNG type. */
    UAV_CAMERA_FILE_TYPE_MOV = 2, /*!< Media file MOV type. */
    UAV_CAMERA_FILE_TYPE_MP4 = 3, /*!< Media file MP4 type. */
    UAV_CAMERA_FILE_TYPE_TIFF = 5, /*!< Media file TIFF type. */
    UAV_CAMERA_FILE_TYPE_PCD = 24, /*!< Media file point cloud type. */
    UAV_CAMERA_FILE_TYPE_UNKNOWN = 255, /*!< Media file unknown type. */
} E_UAVCameraMediaFileType;

/**
 * @brief Camera optical zoom specifies.
 */
typedef struct {
    uint16_t maxFocalLength; /*!< The maximum focal length of the lens, unit: 0.1mm. */
    uint16_t minFocalLength; /*!< The minimum focal length of the lens, unit: 0.1mm. */
    uint16_t focalLengthStep; /*!< The minimum interval of focal length change, unit: 0.1mm. */
} T_UAVCameraOpticalZoomSpec;

/**
 * @brief Data channel state.
 */
typedef struct {
    /*! Realtime bandwidth limitation, varying with link status of aircraft system for some channels, such as data
     * stream, video stream and download stream. Must ensure actual bandwidth of data transmission is less than
     * realtime bandwidth limitation, unit: byte/s. */
    int32_t realtimeBandwidthLimit;

    /*! Realtime actual transmission bandwidth of data transmission channel calculated before flow controller, unit: byte/s. */
    int32_t realtimeBandwidthBeforeFlowController;

    /*! Realtime actual transmission bandwidth of data transmission channel calculated after flow controller, unit:
     * byte/s. If specified channel without flow controller. the value is equal to
     * ::realtimeBandwidthBeforeFlowController. */
    int32_t realtimeBandwidthAfterFlowController;

    /*! State specified whether the channel is busy or not. When data can not be sent to the endpoint directly, instead be
     * sent to buffer of flow controller or discarded, the busy state will be set. At this time, the user should stop
     * transmitting data or decrease the amount of data to be transmitted via this channel. When data bandwidth restore to
     * normal, this state will be clear again. */
    bool busyState;
} T_UAVDataChannelState;


typedef struct {
    uint8_t majorVersion; /*!< The major version of firmware, the range is 0 ~ 99. */
    uint8_t minorVersion; /*!< The minor version of firmware, the range is 0 ~ 99. */
    uint8_t modifyVersion; /*!< The modify version of firmware, the range is 0 ~ 99. */
    uint8_t debugVersion; /*!< The debug version of firmware, the range is 0 ~ 99. */
    char *boot_version;
    char *hw_version;
} T_UAVFirmwareVersion;

#pragma pack()


#ifdef __cplusplus
}
#endif

#endif //__UAV_TYPEDEF_H__