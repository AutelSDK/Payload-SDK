#ifndef __UAV_TYPEDEF_HPP__
#define __UAV_TYPEDEF_HPP__
#include <stdint.h>
#include <stdbool.h>
/**
 * @brief Type define double as autel_f64_t.
 */
typedef double autel_f64_t;
/**
 * @brief Type define float as autel_f32_t.
 */
typedef float autel_f32_t;
/**
 * @brief Type define uint64 as T_UAVReturnCode.
 * @details The type can be any value of ::UAVErrorCode.
 */
typedef uint64_t T_UAVReturnCode;
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
    UAV_GIMBAL_MODE_FREE                        = 0, /*!< Free mode, fix gimbal attitude in the ground coordinate, ignoring movement of aircraft. */
    UAV_GIMBAL_MODE_FPV                         = 1, /*!< FPV (First Person View) mode, only control roll and yaw angle of gimbal in the ground coordinate to follow aircraft. */
    UAV_GIMBAL_MODE_YAW_FOLLOW = 2, /*!< Yaw follow mode, only control yaw angle of gimbal in the ground coordinate to follow aircraft. */
} E_UAVGimbalMode;

/**
 * @brief Gimbal rotation mode, specifies control style.
 */
typedef enum {
    UAV_GIMBAL_ROTATION_MODE_RELATIVE_ANGLE = 0, /*!< Relative angle rotation mode, represents rotating gimbal specified angles based on current angles. */
    UAV_GIMBAL_ROTATION_MODE_ABSOLUTE_ANGLE = 1, /*!< Absolute angle rotation mode, represents rotating gimbal to specified angles in the ground coordinate. */
    UAV_GIMBAL_ROTATION_MODE_SPEED = 2, /*!< Speed rotation mode, specifies rotation speed of gimbal in the ground coordinate. */
} E_UAVGimbalRotationMode;

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
    int64_t seconds; /*!< Specifies the seconds part of the timestamp. */
    int32_t nanos;   /*!< Specifies the nanoseconds part of the timestamp. */
}T_UAVDataTimestamp;

typedef struct {
    uint8_t majorVersion;  /*!< The major version of firmware, the range is 0 ~ 99. */
    uint8_t minorVersion;  /*!< The minor version of firmware, the range is 0 ~ 99. */
    uint8_t patchVersion;  /*!< The modify version of firmware, the range is 0 ~ 99. */
    uint8_t debugVersion;  /*!< The debug version of firmware, the range is 0 ~ 99. */
}T_UAVFirmwareVersion;

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
    /*!
     - Capture mode. In this mode, the user can capture pictures.
     */
    UAV_CAMERA_MODE_SHOOT_PHOTO = 0,
    /*!
     - Record mode. In this mode, the user can record videos.
     */
    UAV_CAMERA_MODE_RECORD_VIDEO = 1,
    /*!
     - Media playback work mode.
     */
    UAV_CAMERA_MODE_PLAYBACK = 2,
    /*!
     * The camera work mode is unknown.
     */
    UAV_CAMERA_MODE_WORK_MODE_UNKNOWN = 0xFF,
} E_UAVCameraMode;

/*! @brief The ShootPhoto mode itself can have several modes. The default
 * value is SINGLE.
 */
typedef enum {
    /*!
     - Sets the camera to take a single photo.
     */
    UAV_CAMERA_SHOOT_PHOTO_MODE_SINGLE = 0x01,
    /*!
     - Sets the camera to take an HDR photo. X5 camera, X5R camera, XT camera,
     Z30 camera, Phantom 4 Pro camera, X4S camera and X5S camera do not support
     HDR mode.
     */
    UAV_CAMERA_SHOOT_PHOTO_MODE_HDR = 0x02,
    /*!
     - Set the camera to take multiple photos at once. XT camera does not
     support Burst mode.
     */
    UAV_CAMERA_SHOOT_PHOTO_MODE_BURST = 0x04,
    /*!
     - Automatic Exposure Bracketing (AEB) capture. In this mode you can quickly
     take multiple shots (the default is 3) at different exposures without
     having to manually change any settings between frames. XT camera and Z30
     camera does not support AEB mode.
     */
    UAV_CAMERA_SHOOT_PHOTO_MODE_AEB = 0x05,
    /*!
     - Sets the camera to take a picture (or multiple pictures) continuously at
     a set time interval. The minimum interval for JPEG format of any quality is
     2s. For all cameras except X4S, X5S and Phantom 4 Pro camera: The minimum
     interval for RAW or RAW+JPEG format is 10s. For the X4S, X5S and Phantom 4
     Pro cameras the minimum interval for RAW or RAW+JPEG dformat is 5s.
     */
    UAV_CAMERA_SHOOT_PHOTO_MODE_INTERVAL = 0x06,
    /*!
     - The shoot photo mode is unknown.
     */
    UAV_CAMERA_SHOOT_PHOTO_MODE_UNKNOWN = 0xFF,
} E_UAVCameraShootPhotoMode;

/*! @breif CameraModule focus mode. If the physical AF switch on the camera is
 * set to auto.
 */
typedef enum {
    /*!
     - The camera's focus mode is set to manual. In this mode, user sets the
     focus ring value to adjust the focal distance.
     */
    UAV_CAMERA_FOCUS_MODE_MANUAL = 0,
    /*!
     - The camera's focus mode is set to auto. For the Z30 camera, the focus is
     calculated completely automatically. For all other cameras, a focus target
     can be set by the user, which is used to calculate focus automatically.
     */
    UAV_CAMERA_FOCUS_MODE_AUTO = 1,
    /*!
     - The camera's focus mode is set to Continuous AF. It is only supported by
     Mavic Pro with firmware version V01.03.0000 or above, X4S camera, Mavic 2
     Zoom camera and Mavic 2 Pro camera.
     */
    UAV_CAMERA_FOCUS_MODE_AFC = 2,
    /*!
     - The camera's focus mode is unknown.
     */
    UAV_CAMERA_FOCUS_MODE_UNKNOWN = 0xFF,
} E_UAVCameraFocusMode;

/**
 * @brief Camera focus target point when in focus mode.
 */
typedef struct {
    autel_f32_t focusX; /*!< Specifies horizontal zone coordinate. This parameter is between 0 and 1.
                            The point [0.0, 0.0] represents the top-left angle of the screen.*/
    autel_f32_t focusY; /*!< Specifies vertical zone coordinate. This parameter is between 0 and 1. */
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
 * @brief Camera supported media file subtype.
 */
typedef enum
{
    UAV_DOWNLOAD_FILE_ORG                       = 0, /*!< Media sub file origin data type. */
    UAV_DOWNLOAD_FILE_LDR                       = 21, /*!< Media sub file cloud point raw data type. */
    UAV_DOWNLOAD_FILE_SIG                       = 22, /*!< Media sub file point cloud signature type. */
    UAV_DOWNLOAD_FILE_RTK                       = 23, /*!< Media sub file point cloud real-time kinematic type. */
    UAV_DOWNLOAD_FILE_CLC                       = 25, /*!< Media sub file radar-camera external reference type. */
    UAV_DOWNLOAD_FILE_CLI                       = 26, /*!< Media sub file radar-IMU external reference type. */
    UAV_DOWNLOAD_FILE_IMU                       = 27, /*!< Media sub file IMU data type. */
    UAV_DOWNLOAD_FILE_RTL                       = 28, /*!< Media sub file RTK boom data type. */
    UAV_DOWNLOAD_FILE_RTB                       = 29, /*!< Media sub file RTK base station data type. */
    UAV_DOWNLOAD_FILE_RTS                       = 30, /*!< Media sub file RTK secondary antenna data type. */
    UAV_DOWNLOAD_FILE_RPOS                      = 31, /*!< Media sub file real-time fusion of attitude and position data type. */
} E_UAVCameraMediaFileSubType;

/**
 * @brief Camera type.
 */
typedef enum {
    UAV_CAMERA_TYPE_UNKNOWN = 0, /*!< Camera type is unknown. */
    UAV_CAMERA_TYPE_XL801 = 1, /*!< Camera type is XL801. */
    UAV_CAMERA_TYPE_XL802 = 2, /*!< Camera type is XL802. */
    UAV_CAMERA_TYPE_XL806 = 6, /*!< Camera type is XL806. */
    UAV_CAMERA_TYPE_XL807 = 7, /*!< Camera type is XL807. */
    UAV_CAMERA_TYPE_XL809 = 9, /*!< Camera type is XL809. */
    UAV_CAMERA_TYPE_XL811 = 11, /*!< Camera type is XL811. */
    UAV_CAMERA_TYPE_XL813 = 13, /*!< Camera type is XL813. */
    UAV_CAMERA_TYPE_XL865 = 65, /*!< Camera type is XL865. */
    UAV_CAMERA_TYPE_XL866 = 66, /*!< Camera type is XL866. */
    UAV_CAMERA_TYPE_DH901 = 101, /*!< Camera type is DH901. */
    UAV_CAMERA_TYPE_XT701 = 201, /*!< Camera type is XT701. */
    UAV_CAMERA_TYPE_XT705 = 205, /*!< Camera type is XT705. */
    UAV_CAMERA_TYPE_XT706 = 206, /*!< Camera type is XT706. */
    UAV_CAMERA_TYPE_XT709 = 209, /*!< Camera type is XT709. */
    UAV_CAMERA_TYPE_XT729 = 229, /*!< Camera type is XT729. */
    UAV_CAMERA_TYPE_XL719 = 319, /*!< Camera type is XL719. */
    UAV_CAMERA_TYPE_XL720 = 320, /*!< Camera type is XL720. */
    UAV_CAMERA_TYPE_XL723 = 323, /*!< Camera type is XL723. */
    UAV_CAMERA_TYPE_XL724 = 324, /*!< Camera type is XL724. */
    UAV_CAMERA_TYPE_XL709 = 409, /*!< Camera type is XL709. */
    UAV_CAMERA_TYPE_XL705 = 405, /*!< Camera type is XL705. */
    UAV_CAMERA_TYPE_XL725 = 425, /*!< Camera type is XL725. */
    UAV_CAMERA_TYPE_XL726 = 426, /*!< Camera type is XL726. */
    UAV_CAMERA_TYPE_XL729 = 429, /*!< Camera type is XL729. */
    UAV_CAMERA_TYPE_XL730 = 530, /*!< Camera type is XL730. */
    UAV_CAMERA_TYPE_XL735 = 535, /*!< Camera type is XL735. */
    UAV_CAMERA_TYPE_XL750 = 650, /*!< Camera type is XL750. */
    UAV_CAMERA_TYPE_XL751 = 651, /*!< Camera type is XL751. */
    UAV_CAMERA_TYPE_XL752 = 652, /*!< Camera type is XL752. */
    UAV_CAMERA_TYPE_XL753 = 653, /*!< Camera type is XL753. */
    UAV_CAMERA_TYPE_XL715 = 715, /*!< Camera type is XL715. */
    UAV_CAMERA_TYPE_XL731 = 731, /*!< Camera type is XL731. */
    UAV_CAMERA_TYPE_XL732 = 832, /*!< Camera type is XL732. */
    UAV_CAMERA_TYPE_XL736 = 836, /*!< Camera type is XL736. */

    UAV_CAMERA_TYPE_XT708 = 908, /*!< Camera type is XL708. */
    UAV_CAMERA_TYPE_XT711 = 911, /*!< Camera type is XL711. */
    UAV_CAMERA_TYPE_XT713 = 913, /*!< Camera type is XL713. */
    UAV_CAMERA_TYPE_XT714 = 914, /*!< Camera type is XL714. */
    UAV_CAMERA_TYPE_XT717 = 917, /*!< Camera type is XL717. */
    UAV_CAMERA_TYPE_XL718 = 918, /*!< Camera type is XL718. */

    UAV_CAMERA_TYPE_DF725 = 1025, /*!< Camera type is DF725. */
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

#endif // __UAV_TYPEDEF_HPP__