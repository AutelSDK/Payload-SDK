/**
 ********************************************************************
 * @file    uav_camera_manager.h
 * @brief   This is the header file for "uav_camera_manager.c", defining the structure and
 * (exported) function prototypes.
 *
 * @copyright (c) 2021 UAV. All rights reserved.
 *
 * All information contained herein is, and remains, the property of UAV.
 * The intellectual and technical concepts contained herein are proprietary
 * to UAV and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of UAV.
 *
 * If you receive this source code without UAV’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify UAV of its removal. UAV reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UAV_CAMERA_MANAGER_H
#define UAV_CAMERA_MANAGER_H

/* Includes ------------------------------------------------------------------*/

#include "uav_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/*! @brief CameraModule work modes.
 */
typedef enum {
    /*!
     - Capture mode. In this mode, the user can capture pictures.
     */
    UAV_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO = 0,
    /*!
     - Record mode. In this mode, the user can record videos.
     */
    UAV_CAMERA_MANAGER_WORK_MODE_RECORD_VIDEO = 1,
    /*!
     * The camera work mode is unknown.
     */
    UAV_CAMERA_MANAGER_WORK_MODE_WORK_MODE_UNKNOWN = 0xFF,
} E_UAVCameraManagerWorkMode;

/*! @brief The ShootPhoto mode itself can have several modes. The default
 * value is SINGLE.
 */
typedef enum {
    /*!
     - Sets the camera to take a single photo.
     */
    UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_SINGLE = 0x01,
    /*!
     - Sets the camera to take an HDR photo. X5 camera, X5R camera, XT camera,
     Z30 camera, Phantom 4 Pro camera, X4S camera and X5S camera do not support
     HDR mode.
     */
    UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_HDR = 0x02,
    /*!
     - Set the camera to take multiple photos at once. XT camera does not
     support Burst mode.
     */
    UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_BURST = 0x04,
    /*!
     - Automatic Exposure Bracketing (AEB) capture. In this mode you can quickly
     take multiple shots (the default is 3) at different exposures without
     having to manually change any settings between frames. XT camera and Z30
     camera does not support AEB mode.
     */
    UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_AEB = 0x05,
    /*!
     - Sets the camera to take a picture (or multiple pictures) continuously at
     a set time interval. The minimum interval for JPEG format of any quality is
     2s. For all cameras except X4S, X5S and Phantom 4 Pro camera: The minimum
     interval for RAW or RAW+JPEG format is 10s. For the X4S, X5S and Phantom 4
     Pro cameras the minimum interval for RAW or RAW+JPEG dformat is 5s.
     */
    UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_INTERVAL = 0x06,
    /*!
     - The shoot photo mode is unknown.
     */
    UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_UNKNOWN = 0xFF,
} E_UAVCameraManagerShootPhotoMode;

/*! @brief the photo action of INTERVAL shooting photo mode
 */
typedef enum {
    UAV_CAMERA_MANAGER_EXPOSURE_MODE_PROGRAM_AUTO = 1,       /*!< Program mode */
    UAV_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY = 2,   /*!< Shutter priority mode */
    UAV_CAMERA_MANAGER_EXPOSURE_MODE_APERTURE_PRIORITY = 3,  /*!< Aperture priority mode */
    UAV_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL = 4,    /*!< Manual mode */
    UAV_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_UNKNOWN = 0xFF /*!< The camera exposure mode is unknown. */
} E_UAVCameraManagerExposureMode;

/*! @brief the photo action of INTERVAL shooting photo mode
 */
typedef enum {
    /*! The number of pictures to continuously take at one time in AEB mode is 3
     */
    UAV_CAMERA_MANAGER_PHOTO_AEB_COUNT_3 = 3,
    /*! The number of pictures to continuously take at one time in AEB mode is 5
     */
    UAV_CAMERA_MANAGER_PHOTO_AEB_COUNT_5 = 5,
    /*! The number of pictures to continuously take at one time in AEB mode is 7
     */
    UAV_CAMERA_MANAGER_PHOTO_AEB_COUNT_7 = 7,
    /*! The number of pictures to continuously take at one time in AEB mode is
     * unknown.
     */
    UAV_CAMERA_MANAGER_PHOTO_AEB_COUNT_KNOWN = 0xFF,
} E_UAVCameraManagerPhotoAEBCount;

/*! @breif CameraModule focus mode. If the physical AF switch on the camera is
 * set to auto.
 */
typedef enum {
    /*!
     - The camera's focus mode is set to manual. In this mode, user sets the
     focus ring value to adjust the focal distance.
     */
    UAV_CAMERA_MANAGER_FOCUS_MODE_MANUAL = 0,
    /*!
     - The camera's focus mode is set to auto. For the Z30 camera, the focus is
     calculated completely automatically. For all other cameras, a focus target
     can be set by the user, which is used to calculate focus automatically.
     */
    UAV_CAMERA_MANAGER_FOCUS_MODE_AUTO = 1,
    /*!
     - The camera's focus mode is set to Continuous AF. It is only supported by
     Mavic Pro with firmware version V01.03.0000 or above, X4S camera, Mavic 2
     Zoom camera and Mavic 2 Pro camera.
     */
    UAV_CAMERA_MANAGER_FOCUS_MODE_AFC = 2,
    /*!
     - The camera's focus mode is unknown.
     */
    UAV_CAMERA_MANAGER_FOCUS_MODE_UNKNOWN = 0xFF,
} E_UAVCameraManagerFocusMode;

/*! @breif CameraModule shutter mode.
 */
typedef enum {
    /*! The shutter mode of camera is automatical */
    UAV_CAMERA_MANAGER_SHUTTER_AUTO_MODE = 0x00,
    /*! The shutter mode of camera is manual, the shutter speed setting is
       valid. */
    UAV_CAMERA_MANAGER_SHUTTER_MANUAL_MODE = 0x01,
} E_UAVCameraManagerShutterMode;

/*! @brief CameraModule shutter speed values.
 */
typedef enum {
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_6400 = 1,     /*!< 1/6400 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_6000 = 2,     /*!< 1/6000 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_5000 = 3,     /*!< 1/5000 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_4000 = 4,     /*!< 1/4000 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_3200 = 5,     /*!< 1/3200 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_3000 = 6,     /*!< 1/3000 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_2500 = 7,     /*!< 1/2500 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_2000 = 8,     /*!< 1/2000 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_1600 = 9,     /*!< 1/1600 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_1500 = 10,    /*!< 1/1500 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_1250 = 11,    /*!< 1/1250 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_1000 = 12,    /*!< 1/1000 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_800 = 13,     /*!< 1/800 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_725 = 14,     /*!< 1/725 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_640 = 15,     /*!< 1/640 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_500 = 16,     /*!< 1/500 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_400 = 17,     /*!< 1/400 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_350 = 18,     /*!< 1/350 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_320 = 19,     /*!< 1/320 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_250 = 20,     /*!< 1/250 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_240 = 21,     /*!< 1/240 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_200 = 22,     /*!< 1/200 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_180 = 23,     /*!< 1/180 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_160 = 24,     /*!< 1/160 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_125 = 25,     /*!< 1/125 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_120 = 26,     /*!< 1/120 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_100 = 27,     /*!< 1/100 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_90 = 28,      /*!< 1/90 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_80 = 29,      /*!< 1/80 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_60 = 30,      /*!< 1/60 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_50 = 31,      /*!< 1/50 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_40 = 32,      /*!< 1/40 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_30 = 33,      /*!< 1/30 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_25 = 34,      /*!< 1/25 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_20 = 35,      /*!< 1/20 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_15 = 36,      /*!< 1/15 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_12DOT5 = 37,  /*!< 1/12.5 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_10 = 38,      /*!< 1/10 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_8 = 39,       /*!< 1/8 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_6DOT25 = 40,  /*!< 1/6.25 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_5 = 41,       /*!< 1/5 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_4 = 42,       /*!< 1/4 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_3 = 43,       /*!< 1/3 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_2DOT5 = 44,   /*!< 1/2.5 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_2 = 45,       /*!< 1/2 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_1DOT67 = 46,  /*!< 1/1.67 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1_1DOT25 = 47,  /*!< 1/1.25 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1 = 48,         /*!< 1.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1DOT3 = 49,     /*!< 1.3 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_1DOT6 = 50,     /*!< 1.6 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_2 = 51,         /*!< 2.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_2DOT5 = 52,     /*!< 2.5 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_3 = 53,         /*!< 3.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_3DOT2 = 54,     /*!< 3.2 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_4 = 55,         /*!< 4.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_5 = 56,         /*!< 5.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_6 = 57,         /*!< 6.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_7 = 58,         /*!< 7.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_8 = 59,         /*!< 8.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_9 = 60,         /*!< 9.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_10 = 61,        /*!< 10.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_13 = 62,        /*!< 13.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_15 = 63,        /*!< 15.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_20 = 64,        /*!< 20.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_25 = 65,        /*!< 25.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_30 = 66,        /*!< 30.0 s */
    UAV_CAMERA_MANAGER_SHUTTER_SPEED_UNKNOWN = 0xFF, /*!< Unknown */
} E_UAVCameraManagerShutterSpeed;

/*! @brief CameraModule ISO values.
 */
typedef enum {
    /*! The ISO value is automatically set. This cannot be used for all cameras
       when in Manual mode. */
    UAV_CAMERA_MANAGER_ISO_AUTO = 0x00,
    /*!  The ISO value is set to 100. */
    UAV_CAMERA_MANAGER_ISO_100 = 0x03,
    /*! The ISO value is set to 100. */
    UAV_CAMERA_MANAGER_ISO_200 = 0x04,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_400 = 0x05,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_800 = 0x06,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_1600 = 0x07,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_3200 = 0x08,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_6400 = 0x09,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_12800 = 0x0A,
    /*! The ISO value is set to 100.*/
    UAV_CAMERA_MANAGER_ISO_25600 = 0x0B,
    /*! ISO value is fixed by the camera firmware. When the camera color is set
     to D_LOG, camera will fix the ISO to a specific value in order to optimize
     the performance.
     */
    UAV_CAMERA_MANAGER_ISO_FIXED = 0xFF,
} E_UAVCameraManagerISO;

/*! @brief CameraModule exposure compensation.
 */
typedef enum {
    /*! The camera's exposure compensation is -5.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_5_0 = 1,
    /*! The camera's exposure compensation is -4.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_4_7 = 2,
    /*! The camera's exposure compensation is -4.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_4_3 = 3,
    /*! The camera's exposure compensation is -4.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_4_0 = 4,
    /*! The camera's exposure compensation is -3.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_3_7 = 5,
    /*! The camera's exposure compensation is -3.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_3_3 = 6,
    /*! The camera's exposure compensation is -3.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_3_0 = 7,
    /*! The camera's exposure compensation is -2.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_7 = 8,
    /*! The camera's exposure compensation is -2.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_3 = 9,
    /*! The camera's exposure compensation is -2.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_2_0 = 10,
    /*! The camera's exposure compensation is -1.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_7 = 11,
    /*! The camera's exposure compensation is -1.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_3 = 12,
    /*! The camera's exposure compensation is -1.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_1_0 = 13,
    /*! The camera's exposure compensation is -0.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_7 = 14,
    /*! The camera's exposure compensation is -0.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_3 = 15,
    /*! The camera's exposure compensation is 0.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_N_0_0 = 16,
    /*! The camera's exposure compensation is +0.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_3 = 17,
    /*! The camera's exposure compensation is +0.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_0_7 = 18,
    /*! The camera's exposure compensation is +1.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_0 = 19,
    /*! The camera's exposure compensation is +1.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_3 = 20,
    /*! The camera's exposure compensation is +1.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_7 = 21,
    /*! The camera's exposure compensation is +2.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_0 = 22,
    /*! The camera's exposure compensation is +2.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_3 = 23,
    /*! The camera's exposure compensation is +2.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_2_7 = 24,
    /*! The camera's exposure compensation is +3.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_3_0 = 25,
    /*! The camera's exposure compensation is +3.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_3_3 = 26,
    /*! The camera's exposure compensation is +3.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_3_7 = 27,
    /*! The camera's exposure compensation is +4.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_4_0 = 28,
    /*! The camera's exposure compensation is +4.3ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_4_3 = 29,
    /*! The camera's exposure compensation is +4.7ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_4_7 = 30,
    /*! The camera's exposure compensation is +5.0ev.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_5_0 = 31,
    /*! The camera's exposure compensation is fixed by the camera.*/
    UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_FIXED = 0xFF,
} E_UAVCameraManagerExposureCompensation;

/*! @brief CameraModule aperture values.
 *  @note X5, X5R, Z30, Phantom 4 Pro camera, X4S and X5S support this
 * setting.
 */
typedef enum {
    /*! 	The Aperture value is f/1.6. It is only supported by Z30
       camera.*/
    UAV_CAMERA_MANAGER_APERTURE_F_1_DOT_6 = 160,
    /*! The Aperture value is f/1.7.*/
    UAV_CAMERA_MANAGER_APERTURE_F_1_DOT_7 = 170,
    /*! The Aperture value is f/1.8.*/
    UAV_CAMERA_MANAGER_APERTURE_F_1_DOT_8 = 180,
    /*! The Aperture value is f/2.*/
    UAV_CAMERA_MANAGER_APERTURE_F_2 = 200,
    /*! The Aperture value is f/2.2.*/
    UAV_CAMERA_MANAGER_APERTURE_F_2_DOT_2 = 220,
    /*! The Aperture value is f/2.4. It is only supported by Z30 camera.*/
    UAV_CAMERA_MANAGER_APERTURE_F_2_DOT_4 = 240,
    /*! The Aperture value is f/2.5.*/
    UAV_CAMERA_MANAGER_APERTURE_F_2_DOT_5 = 250,
    /*! The Aperture value is f/2.6.*/
    UAV_CAMERA_MANAGER_APERTURE_F_2_DOT_6 = 260,
    /*! The Aperture value is f/2.8.*/
    UAV_CAMERA_MANAGER_APERTURE_F_2_DOT_8 = 280,
    /*! The Aperture value is f/3.2.*/
    UAV_CAMERA_MANAGER_APERTURE_F_3_DOT_2 = 320,
    /*! The Aperture value is f/3.4.*/
    UAV_CAMERA_MANAGER_APERTURE_F_3_DOT_4 = 340,
    /*! The Aperture value is f/3.5.*/
    UAV_CAMERA_MANAGER_APERTURE_F_3_DOT_5 = 350,
    /*! The Aperture value is f/4.*/
    UAV_CAMERA_MANAGER_APERTURE_F_4 = 400,
    /*! The Aperture value is f/4.5.*/
    UAV_CAMERA_MANAGER_APERTURE_F_4_DOT_5 = 450,
    /*! The Aperture value is f/4.8.*/
    UAV_CAMERA_MANAGER_APERTURE_F_4_DOT_8 = 480,
    /*! The Aperture value is f/5.*/
    UAV_CAMERA_MANAGER_APERTURE_F_5 = 500,
    /*! The Aperture value is f/5.6.*/
    UAV_CAMERA_MANAGER_APERTURE_F_5_DOT_6 = 560,
    /*! The Aperture value is f/6.3.*/
    UAV_CAMERA_MANAGER_APERTURE_F_6_DOT_3 = 630,
    /*! The Aperture value is f/6.8.*/
    UAV_CAMERA_MANAGER_APERTURE_F_6_DOT_8 = 680,
    /*! The Aperture value is f/7.1.*/
    UAV_CAMERA_MANAGER_APERTURE_F_7_DOT_1 = 710,
    /*! The Aperture value is f/8.*/
    UAV_CAMERA_MANAGER_APERTURE_F_8 = 800,
    /*! The Aperture value is f/9.*/
    UAV_CAMERA_MANAGER_APERTURE_F_9 = 900,
    /*! The Aperture value is f/9.6.*/
    UAV_CAMERA_MANAGER_APERTURE_F_9_DOT_6 = 960,
    /*! The Aperture value is f/10.*/
    UAV_CAMERA_MANAGER_APERTURE_F_10 = 1000,
    /*! The Aperture value is f/11.*/
    UAV_CAMERA_MANAGER_APERTURE_F_11 = 1100,
    /*! The Aperture value is f/13.*/
    UAV_CAMERA_MANAGER_APERTURE_F_13 = 1300,
    /*! The Aperture value is f/14.*/
    UAV_CAMERA_MANAGER_APERTURE_F_14 = 1400,
    /*! The Aperture value is f/16.*/
    UAV_CAMERA_MANAGER_APERTURE_F_16 = 1600,
    /*! The Aperture value is f/18.*/
    UAV_CAMERA_MANAGER_APERTURE_F_18 = 1800,
    /*! The Aperture value is f/19.*/
    UAV_CAMERA_MANAGER_APERTURE_F_19 = 1900,
    /*! The Aperture value is f/20.*/
    UAV_CAMERA_MANAGER_APERTURE_F_20 = 2000,
    /*! The Aperture value is f/22.*/
    UAV_CAMERA_MANAGER_APERTURE_F_22 = 2200,
    /*! The Aperture value is Unknown. */
    UAV_CAMERA_MANAGER_APERTURE_F_UNKNOWN = 0xFFFF,
} E_UAVCameraManagerAperture;

typedef enum {
    UAV_CAMERA_MANAGER_RECORDING_CONTROL_STOP = 0,
    UAV_CAMERA_MANAGER_RECORDING_CONTROL_BEGIN = 1,
    UAV_CAMERA_MANAGER_RECORDING_CONTROL_PAUSE = 2,
    UAV_CAMERA_MANAGER_RECORDING_CONTROL_RESUME = 3,
} E_UAVCameraManagerRecordingControl;

typedef enum {
    UAV_CAMERA_MANAGER_FILE_LIST_COUNT_60_PER_SLICE = 60,
    UAV_CAMERA_MANAGER_FILE_LIST_COUNT_120_PER_SLICE = 120,
    UAV_CAMERA_MANAGER_FILE_LIST_COUNT_ALL_PER_SLICE = 0xFFFF,
} E_UAVCameraManagerFileListCountPerSlice;

typedef enum {
    UAV_CAMERA_MANAGER_SOURCE_DEFAULT_CAM = 0x0,
    UAV_CAMERA_MANAGER_SOURCE_WIDE_CAM = 0x1,
    UAV_CAMERA_MANAGER_SOURCE_ZOOM_CAM = 0x2,
    UAV_CAMERA_MANAGER_SOURCE_IR_CAM = 0x3,
    UAV_CAMERA_MANAGER_SOURCE_VISIBLE_CAM = 0x7,
} E_UAVCameraManagerStreamSource;

typedef enum {
    UAV_CAMERA_MANAGER_NIGHT_SCENE_MODE_DISABLE = 0,
    UAV_CAMERA_MANAGER_NIGHT_SCENE_MODE_ENABLE = 1,
    UAV_CAMERA_MANAGER_NIGHT_SCENE_MODE_AUTO = 2,
} E_UAVCameraManagerNightSceneMode;

typedef enum {
    UAV_CAMERA_MANAGER_CAPTURE_OR_RECORDING_CAPTURE = 0,
    UAV_CAMERA_MANAGER_CAPTURE_OR_RECORDING_RECORDING = 1,
} E_UAVCameraManagerCaptureOrRecording;

typedef enum {
    UAV_CAMERA_MANAGER_EXPAND_NAME_TYPE_FILE = 1,
    UAV_CAMERA_MANAGER_EXPAND_NAME_TYPE_DIR = 2,
} E_UAVCameraManagerExpandNameType;

typedef enum {
    UAV_CAMERA_MANAGER_PHOTO_RATIO_4X3 = 0,
    UAV_CAMERA_MANAGER_PHOTO_RATIO_16X9 = 1,
} E_UAVCameraManagerPhotoRatio;

typedef struct {
    uint8_t firmware_version[4];
} T_UAVCameraManagerFirmwareVersion;

/*! @brief Tap zoom target point data struct, used by user.
 */
typedef T_UAVCameraPointInScreen T_UAVCameraManagerTapZoomPosData;

/*! @brief Tap focus target point data struct, used by user.
 */
typedef T_UAVCameraPointInScreen T_UAVCameraManagerFocusPosData;
typedef T_UAVCameraPointInScreen T_UAVCameraManagerMeteringPosData;


typedef struct {
    uav_f32_t currentOpticalZoomFactor;
    uav_f32_t maxOpticalZoomFactor;
} T_UAVCameraManagerOpticalZoomParam;

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} T_UAVCameraManagerFileCreateTime;

typedef struct {
    union {
        struct {
            uint32_t attributePhotoReserved: 22;
            uint32_t attributePhotoRatio: 8;
            uint32_t attributePhotoRotation: 2;
            uint8_t reserved[12];
        } photoAttribute;
        struct {
            uint32_t attributeVideoDuration: 16;
            uint32_t attributeVideoFramerate: 6;
            uint32_t attributeVideoRotation: 2;
            uint32_t attributeVideoResolution: 8;
            uint8_t reserved[12];
        } videoAttribute;
    };
} T_UAVCameraManagerFileAttributeData;



typedef struct {
    uint16_t sliceStartIndex;
    E_UAVCameraManagerFileListCountPerSlice countPerSlice;
} T_UAVCameraManagerSliceConfig;

typedef enum {
    UAV_DOWNLOAD_FILE_EVENT_START,
    UAV_DOWNLOAD_FILE_EVENT_TRANSFER,
    UAV_DOWNLOAD_FILE_EVENT_END,
} E_UAVDownloadFileEvent;

typedef enum {
    UAV_CAMERA_MANAGER_VIDEO_RESOLUTION_640X480P = 0, // 640X480P
    UAV_CAMERA_MANAGER_VIDEO_RESOLUTION_1280X640P = 2, // 1280X640P
    UAV_CAMERA_MANAGER_VIDEO_RESOLUTION_1280X720P = 4, // 1280X720P
    UAV_CAMERA_MANAGER_VIDEO_RESOLUTION_1920X1080P = 10, // 1920X1080P
    UAV_CAMERA_MANAGER_VIDEO_RESOLUTION_3840X2160P = 16, // 3840X2160P
    UAV_CAMERA_MANAGER_VIDEO_RESOLUTION_4000X3000P = 32, // 4000X3000P
} E_UAVCameraManagerVideoResolution;

typedef enum {
    UAV_CAMERA_MANAGER_VIDEO_FRAME_RATE_15FPS = 0, // 14.985
    UAV_CAMERA_MANAGER_VIDEO_FRAME_RATE_25FPS = 2, // 25.000
    UAV_CAMERA_MANAGER_VIDEO_FRAME_RATE_30FPS = 3, // 29.970
    UAV_CAMERA_MANAGER_VIDEO_FRAME_RATE_60FPS = 6, // 59.940
} E_UAVCameraManagerVideoFrameRate;

typedef enum {
    UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_RAW = 0,
    UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_JPEG = 1,
    UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_RAW_JPEG = 2,
    UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_YUV = 3, // 保存为YUV格式的图片
    UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_RJPEG = 7, // Radiometric JPEG
} E_UAVCameraManagerPhotoStorageFormat;

typedef enum {
    UAV_CAMERA_MANAGER_VIDEO_STORAGE_FORMAT_MOV = 0,
    UAV_CAMERA_MANAGER_VIDEO_STORAGE_FORMAT_MP4 = 1,
} E_UAVCameraManagerVideoStorageFormat;

typedef enum {
    UAV_CAMERA_MANAGER_METERING_MODE_CENTRAL = 0,
    UAV_CAMERA_MANAGER_METERING_MODE_AVERAGE = 1,
    UAV_CAMERA_MANAGER_METERING_MODE_SPOT = 2,
} E_UAVCameraManagerMeteringMode;

typedef enum {
    UAV_CAMERA_MANAGER_FFC_MODE_MANUAL = 0,
    UAV_CAMERA_MANAGER_FFC_MODE_AUTO = 1,
} E_UAVCameraManagerFfcMode;

typedef enum {
    UAV_CAMERA_MANAGER_IR_GAIN_MODE_AUTO = 0,
    UAV_CAMERA_MANAGER_IR_GAIN_MODE_LOW = 1,
    UAV_CAMERA_MANAGER_IR_GAIN_MODE_HIGH = 2,
} E_UAVCameraManagerIrGainMode;

typedef enum {
    /* not capturing*/
    UAV_CAMERA_MANAGER_CAPTURING_STATE_IDLE = 0,

    /* doing single capture */
    UAV_CAMERA_MANAGER_CAPTURING_STATE_SINGLE = 1,

    /* doing multi capture */
    UAV_CAMERA_MANAGER_CAPTURING_STATE_MULTI = 2,
} E_UAVCameraManagerCapturingState;

typedef enum {
    UAV_CAMERA_MANAGER_RECORDING_STATE_IDLE = 0,
    UAV_CAMERA_MANAGER_RECORDING_STATE_STARTING = 0,
    UAV_CAMERA_MANAGER_RECORDING_STATE_RECORDING = 0,
    UAV_CAMERA_MANAGER_RECORDING_STATE_STOPPING = 0,
} E_UAVCameraManagerRecordingState;

/*!< Attention: when the remote control is in split-screen mode, the coordinate range of the x-axis is 0 ~ 0.5
* */
typedef struct {
    uav_f32_t pointX;               /*! x-coordinate of point thermometry, range: 0 ~ 1 */
    uav_f32_t pointY;               /*! y-coordinate of point thermometry, range: 0 ~ 1 */
} T_UAVCameraManagerPointThermometryCoordinate;

typedef struct {
    uav_f32_t areaTempLtX;          /*! x-coordinate of the upper left corner of the area thermometry, range: 0 ~ 1 */
    uav_f32_t areaTempLtY;          /*! y-coordinate of the upper left corner of the area thermometry, range: 0 ~ 1 */
    uav_f32_t areaTempRbX;          /*! x-coordinate of the lower right corner of the area thermometry, range: 0 ~ 1 */
    uav_f32_t areaTempRbY;          /*! y-coordinate of the lower right corner of the area thermometry, range: 0 ~ 1 */
} T_UAVCameraManagerAreaThermometryCoordinate;

//result of point thermometry
typedef struct {
    uav_f32_t pointX;              /*! x-coordinate of point thermometry, range: 0 ~ 1 */
    uav_f32_t pointY;              /*! y-coordinate of point thermometry, range: 0 ~ 1 */
    uav_f32_t pointTemperature;    /*! The temperature of the current point */
} T_UAVCameraManagerPointThermometryData;

//result of area thermometry
typedef struct {
    uav_f32_t areaTempLtX;           /*! x_coordinate of the upper left corner of the current thermometry area */
    uav_f32_t areaTempLtY;           /*! y_coordinate of the upper left corner of the current thermometry area */
    uav_f32_t areaTempRbX;           /*! x_coordinate of the lower right corner of the current thermometry area */
    uav_f32_t areaTempRbY;           /*! y_coordinate of the lower right corner of the current thermometry area */
    uav_f32_t areaAveTemp;           /*! The average temperature of the current thermometry area */
    uav_f32_t areaMinTemp;           /*! The minimum temperature of the current thermometry area */
    uav_f32_t areaMaxTemp;           /*! The maximum temperature of the current thermometry area */
    uav_f32_t areaMinTempPointX;     /*! x_coordinate of the minimum temperature in the thermometry area */
    uav_f32_t areaMinTempPointY;     /*! y_coordinate of the minimum temperature in the thermometry area */
    uav_f32_t areaMaxTempPointX;     /*! x_coordinate of the maximum temperature in the thermometry area */
    uav_f32_t areaMaxTempPointY;     /*! y_coordinate of the maximum temperature in the thermometry area */
} T_UAVCameraManagerAreaThermometryData;

typedef struct {
    E_UAVDownloadFileEvent downloadFileEvent;
    uint32_t fileIndex;
    uint32_t fileSize;
    uav_f32_t progressInPercent;
} T_UAVDownloadFilePacketInfo;

typedef struct {
    uav_f64_t longitude; /*! Range: [-180,180] */
    uav_f64_t latitude; /*! Range: [-90,90] */
    int32_t altitude; /*! Unit: 0.1m */
    int32_t distance; /*! Unit: 0.1m */
    int16_t screenX; /*! Unit: 0.1% */
    int16_t screenY; /*! Unit: 0.1% */
    bool enable_lidar;
    uint8_t exception;
} T_UAVCameraManagerLaserRangingInfo;

typedef struct {
    uint32_t size;
    E_UAVCameraManagerStreamSource streamSource[4];
    E_UAVCameraManagerStreamSource streamStorage[4];
} T_UAVCameraManagerStreamList;

typedef struct {
    E_UAVCameraManagerVideoResolution videoResolution;
    E_UAVCameraManagerVideoFrameRate videoFrameRate;
} T_UAVCameraManagerVideoFormat;

typedef struct {
    uint8_t size;
    union {
        E_UAVCameraManagerPhotoStorageFormat photoStorageFormat[16];
        E_UAVCameraManagerVideoStorageFormat videoStorageFormat[16];
        E_UAVCameraManagerPhotoRatio photoRatioFormat[16];
        E_UAVCameraManagerStreamSource streamSource[16];
        E_UAVCameraManagerStreamSource streamStorage[16];
        E_UAVCameraManagerNightSceneMode nightSceneMode[16];
    };
    uint32_t minValue;
    uint32_t maxValue;
} T_UAVCameraManagerRangeList;

typedef struct {
    double lowGainTempMin;
    double lowGainTempMax;
    double highGainTempMin;
    double highGainTempMax;
} T_UAVCameraManagerIrTempMeterRange;

typedef struct {
    uint32_t totalCapacity;     /* MByte */
    uint32_t remainCapacity;    /* MByte */
} T_UAVCameraManagerStorageInfo;

typedef struct {
    uint32_t flag; /* 0xFFFFFFFF */
    uint32_t seqNum;
    uint64_t timestamp;
    uint32_t dataByte; /* actual num of bytes used for points */
} T_UAVCameraManagerPointCloudHeader;

typedef struct {
    float x; /* the x-axis of NED coordinate system */
    float y; /* the y-axis of NED coordinate system */
    float z; /* the z-axis of NED coordinate system */
    uint8_t intensity;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} T_UAVCameraManagerPointXYZRGBInfo;

typedef struct {
    T_UAVCameraManagerPointCloudHeader pointCloudHeader;
    uint32_t crc_header;
    uint32_t crc_rest;
    T_UAVCameraManagerPointXYZRGBInfo points[1];
} T_UAVCameraManagerColorPointCloud;

typedef T_UAVReturnCode (*UAVCameraManagerDownloadFileDataCallback)(T_UAVDownloadFilePacketInfo packetInfo,
                                                                    const uint8_t *data,
                                                                    uint16_t dataLen);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise camera manager module, and user should call this function
 * before using camera manager features.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_Init(void);

/**
 * @brief Deinitialise camera manager module.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_DeInit(void);

/**
 * @brief Get camera type of the selected camera mounted position.
 * @param position: camera mounted position
 * @param cameraType: see references of E_UAVCameraType.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetCameraType(E_UAVCameraType *cameraType);

/**
 * @brief Get camera firmware version of the selected camera mounted position.
 * @param position: camera mounted position
 * @param firmwareVersion: see references of T_UAVCameraManagerFirmwareVersion.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetFirmwareVersion(T_UAVCameraManagerFirmwareVersion *firmwareVersion);

/**
 * @brief Get camera connect status.
 * @param position: camera mounted position
 * @param connectStatus: returned value of connect status
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetCameraConnectStatus(bool *connectStatus);


/**
 * @brief Set camera working mode of the selected camera mounted position.
 * @note Set the camera's work mode to taking pictures, video, playback or
 * download and so on. Please note that you cannot change the mode when a certain task
 * is executing.This action will cost about 1~2s.
 * @param position: camera mounted position
 * @param workMode: see reference of E_UAVCameraManagerWorkMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetMode(E_UAVCameraManagerWorkMode workMode);

/**
 * @brief Get camera working mode of the selected camera mounted position.
 * @param position: camera mounted position
 * @param workMode: see reference of E_UAVCameraManagerWorkMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetMode(E_UAVCameraManagerWorkMode *workMode);

/**
* @brief Set camera shoot mode of the selected camera mounted position.
* @param position: camera mounted position
* @param mode: see reference of E_UAVCameraManagerShootPhotoMode.
* @return Execution result.
*/
T_UAVReturnCode UAV_CameraManager_SetShootPhotoMode(E_UAVCameraManagerShootPhotoMode mode);

/**
* @brief Get camera shoot mode of the selected camera mounted position.
* @param position: camera mounted position
* @param mode: see reference of E_UAVCameraManagerShootPhotoMode.
* @return Execution result.
*/
T_UAVReturnCode UAV_CameraManager_GetShootPhotoMode(E_UAVCameraManagerShootPhotoMode *takePhotoMode);

/**
 * @brief Start to shoot photo.
 * @note Camera must be in ShootPhoto mode. For thermal imaging camera,
 * Single photo can be taken while recording video. The SD card state should
 * be checked before this method is used to ensure sufficient space exists.
 * @param position: camera mounted position
 * @param mode: see reference of E_UAVCameraManagerShootPhotoMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StartShootPhoto(void);


/**
 * @brief Stop to shoot photo when you are in taking photo.
 * @note StartShootPhoto has been invoked and the shoot mode is either
 * Interval or Time-lapse. If the shoot mode is set to single, the camera
 * will automatically stop taking the photo once the individual photo is
 * taken.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StopShootPhoto(void);

/**
 * @brief Get camera capturing state.
 * @note L1/P1 do not support this API.
 * @param position: camera mounted position
 * @param capturingState: result of getting, see E_UAVCameraManagerCapturingState.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetCapturingState(E_UAVCameraManagerCapturingState *capturingState);

/**
 * @brief Set the burst count in the burst take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_UAVCameraBurstCount.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetPhotoBurstCount(E_UAVCameraBurstCount count);

/**
 * @brief Get the burst count in the burst take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_UAVCameraBurstCount.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoBurstCount(E_UAVCameraBurstCount *count);

/**
 * @brief Set the burst count in the AEB(Automatic Exposure Bracketing) take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_UAVCameraManagerPhotoAEBCount.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetPhotoAEBCount(E_UAVCameraManagerPhotoAEBCount count);

/**
 * @brief Get the burst count in the AEB(Automatic Exposure Bracketing) take-photo mode.
 * @param position: camera mounted position
 * @param count: see reference of E_UAVCameraManagerPhotoAEBCount.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoAEBCount(E_UAVCameraManagerPhotoAEBCount *count);

/**
 * @brief Set the parameters in the INTERVAL take-photo mode.
 * @note When in this shoot-photo mode, The camera will capture a photo, wait
 * a specified interval of time, take another photo, and continue in this
 * manner until it has taken the required number of photos. Also supported by
 * thermal imaging camera.
 * @param position: camera mounted position
 * @param intervalSetting: see reference of T_UAVCameraPhotoTimeIntervalSettings.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetPhotoTimeIntervalSettings(uint32_t interval);

/**
 * @brief Get the parameters in the INTERVAL take-photo mode.
 * @param position: camera mounted position
 * @param intervalSetting: see reference of T_UAVCameraPhotoTimeIntervalSettings.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoTimeIntervalSettings(uint32_t *interval);

/**
 * @brief Get remain time of interval shooting.
 * @note L1/P1 do not support this API.
 * @param position: camera mounted position
 * @param remainTime: time in seconds.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetIntervalShootingRemainTime(uint8_t *remainTime);

/**
 * @brief Set camera focus mode of the selected camera mounted position.
 * @note Set the lens focus mode. When the focus mode is auto, the target
 * point is the focal point. When the focus mode is manual, the target point
 * is the zoom out area if the focus assistant is enabled for the manual
 * mode.
 * @param position: camera mounted position
 * @param focusMode: see reference of E_UAVCameraManagerFocusMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetFocusMode(E_UAVCameraManagerFocusMode focusMode);
/**
 * @brief Get camera focus mode of the selected camera mounted position.
 * @param position: camera mounted position
 * @param focusMode: see reference of E_UAVCameraManagerFocusMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetFocusMode(E_UAVCameraManagerFocusMode *focusMode);

/**
 * @brief Set amera focus point of the selected camera mounted position.
 * @note  Sets the lens focus target point. When the focus mode is auto, the
 * target point is the focal point. When the focus mode is manual, the target
 * point is the zoom out area if the focus assistant is enabled for the manual
 * mode.
 * @param position: camera mounted position
 * @param focusPosData: see reference of T_UAVCameraManagerFocusPosData.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetFocusTarget(T_UAVCameraManagerFocusPosData focusPosData);

/**
 * @brief Get amera focus point of the selected camera mounted position.
 * @param position: camera mounted position
 * @param focusPosData: see reference of T_UAVCameraManagerFocusPosData.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetFocusTarget(T_UAVCameraManagerFocusPosData *tapFocusPos);

/**
 * @brief Start camera optical zooming of the selected camera mounted position.
 * @note Start changing the focal length of the lens in specified direction
 * with specified speed. Focal length change (zooming) will halt when maximum
 * or minimum focal lengths are reached, or UAV_CameraManager_StopContinuousOpticalZoom*
 * is called.
 * @param position: camera mounted position
 * @param zoomDirection: optical zoom direction, see reference of E_UAVCameraZoomDirection.
 * @param zoomSpeed: optical zoom direction, see reference of E_UAVCameraZoomSpeed.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StartContinuousOpticalZoom( E_UAVCameraZoomDirection zoomDirection,
                                                            E_UAVCameraZoomSpeed zoomSpeed);

/**
 * @brief Stop camera optical zooming of the selected camera mounted position.
 * @note Called to stop focal length changing, when it currently is from
 * calling UAV_CameraManager_StartContinuousOpticalZoom*.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StopContinuousOpticalZoom(void);


/**
 * @brief Set parameters for camera optical zooming of the selected camera mounted position.
 * @note In this interface, the zoom will set the zoom factor as the your
 * target value.
 * @param position: camera mounted position
 * @param zoomDirection: optical zoom direction, see reference of E_UAVCameraZoomDirection.
 * @param factor: target zoom factor.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetOpticalZoomParam(E_UAVCameraZoomDirection zoomDirection,
                                                     uav_f32_t factor);

/**
 * @brief Get parameters for camera optical zooming of the selected camera mounted position.
 * @param position: camera mounted position
 * @param opticalZoomParam: see reference of T_UAVCameraManagerOpticalZoomParam.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetOpticalZoomParam(T_UAVCameraManagerOpticalZoomParam *opticalZoomParam);

/**
 * @brief Set parameters for camera infrared zooming of the selected camera mounted position.
 * @param position: camera mounted position
 * @param factor: target zoom factor.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetInfraredZoomParam(uav_f32_t factor);

/**
 * @brief Enable/Disable camera's tap-zoom function of the selected camera mounted position.
 * @note TapZoomAtTarget can only be called when tap-zoom is enabled.
 * @param position: camera mounted position
 * @param param: enable/disable
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetTapZoomEnabled(bool param);

/**
 * @brief Get status of camera's tap-zoom function of the selected camera mounted position.
 * @param position: camera mounted position
 * @param param: enable/disable
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetTapZoomEnabled(bool *param);

/**
 * @brief Set camera's tap-zoom multiplier of the selected camera mounted position.
 * @note Tap-zoom uses a multiplier to change the zoom scale when called. The
 * inal zoom scale for a TapZoom will be: Current Zoom Scale x Multiplier.
 * @param position: camera mounted position
 * @param tapZoomMultiplier: The multiplier range is [1,5]. A multiplier of 1 will not change the zoom.
 * hen the multiplier is 1, the zoom scale will not change during TapZoom.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetTapZoomMultiplier(uint8_t tapZoomMultiplier);

/**
 * @brief Get camera's tap-zoom multiplier of the selected camera mounted position.
 * @param position: camera mounted position
 * @param tapZoomMultiplier: The multiplier range is [1,5]. A multiplier of 1 will not change the zoom.
 * When the multiplier is 1, the zoom scale will not change during TapZoom.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetTapZoomMultiplier(uint8_t *tapZoomMultiplier);

/**
 * @brief Set camera's tap-zoom point of the selected camera mounted position.
 * @note Tap-zoom at the target. It can be called only when TapZoom is
 * enabled. When a new target is set, the gimbal will rotate and locate the
 * target in the center of the screen. At the same time, the camera will zoom
 * by multiplying the TapZoom multiplier
 * @param position: camera mounted position
 * @param tapZoomPos: see reference of T_UAVCameraManagerTapZoomPosData.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_TapZoomAtTarget(T_UAVCameraManagerTapZoomPosData tapZoomPos);


/**
 * @brief Set camera focus ring value.
 * @param position: camera mounted position
 * @param value: focus ring value.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetFocusRingValue(uint32_t value);


/**
 * @brief Get camera focus ring value.
 * @param position: camera mounted position
 * @param value: focus ring value to be returned.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetFocusRingValue(uint32_t *value);

/**
 * @brief Get camera focus ring value range.
 * @param position: camera mounted position
 * @param rangeList: returned value of range.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetFocusRingRange(T_UAVCameraManagerRangeList *rangeList);



/**
 * @brief Set camera's exposure mode of the selected camera mounted position.
 * @note  The different exposure modes define whether aperture, shutter speed,
 * ISO can be set automatically or manually. Exposure compensation can be
 * changed in all modes except manual mode where it is not settable.
 * @param position: camera mounted position
 * @param mode: see reference of E_UAVCameraManagerExposureMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetExposureMode(E_UAVCameraManagerExposureMode mode);

/**
 * @brief Get camera's exposure mode of the selected camera mounted position.
 * @note  The different exposure modes define whether aperture, shutter speed,
 * ISO can be set automatically or manually. Exposure compensation can be
 * changed in all modes except manual mode where it is not settable.
 * @param position: camera mounted position
 * @param mode: see reference of E_UAVCameraManagerExposureMode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetExposureMode(E_UAVCameraManagerExposureMode *mode);

/**
 * @brief Set camera's iso value of the selected camera mounted position.
 * @note  ISO value can only be set when the camera exposure mode is in
 * manual mode.
 * @param position: camera mounted position
 * @param iso: see reference of E_UAVCameraManagerISO.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetISO(E_UAVCameraManagerISO iso);

/**
 * @brief Get camera's iso value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param iso: see reference of E_UAVCameraManagerISO.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetISO(E_UAVCameraManagerISO *iso);

/**
 * @brief Set camera's aperture size value of the selected camera mounted position.
 * @note The exposure mode must be in UAV_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL or
 * UAV_CAMERA_MANAGER_EXPOSURE_MODE_APERTURE_PRIORITY.
 * @param position: camera mounted position
 * @param aperture: see reference of E_UAVCameraManagerAperture.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetAperture(E_UAVCameraManagerAperture aperture);

/**
 * @brief Get camera's aperture size value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param aperture: see reference of E_UAVCameraManagerAperture.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetAperture(E_UAVCameraManagerAperture *aperture);

/**
 * @brief Set camera's shutter value of the selected camera mounted position.
 * @note Set the camera shutter speed. The shutter speed should not be set
 * slower than the video frame rate when the camera's mode is RECORD_VIDEO.
 * For example, if the video frame rate is 30fps, the shutterSpeed must be <=
 * 1/30. Precondition: The shutter speed can be set only when the camera
 * exposure mode is UAV_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL mode or
 * UAV_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY
 * @param position: camera mounted position
 * @param shutterSpeed: see reference of E_UAVCameraManagerShutterSpeed.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetShutterSpeed(E_UAVCameraManagerShutterSpeed shutterSpeed);

/**
 * @brief Get camera's shutter value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param shutterSpeed: see reference of E_UAVCameraManagerShutterSpeed.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetShutterSpeed(E_UAVCameraManagerShutterSpeed *shutterSpeed);


/**
 * @brief Set camera's EV value of the selected camera mounted position.
 * @note Set the camera's exposure compensation. In order to use this
 * function, set the camera exposure mode to shutter, program or aperture.
 * exposure mode is UAV_CAMERA_MANAGER_EXPOSURE_MODE_EXPOSURE_MANUAL mode or
 * UAV_CAMERA_MANAGER_EXPOSURE_MODE_SHUTTER_PRIORITY or
 * UAV_CAMERA_MANAGER_EXPOSURE_APERTURE_PRIORITY
 * @param position: camera mounted position
 * @param ev: see reference of E_UAVCameraManagerExposureCompensation.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetExposureCompensation(E_UAVCameraManagerExposureCompensation ev);

/**
 * @brief Get camera's EV value of the selected camera mounted position.
 * @param position: camera mounted position
 * @param ev: see reference of E_UAVCameraManagerExposureCompensation.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetExposureCompensation(E_UAVCameraManagerExposureCompensation *ev);

/**
 * @brief Set AE lock mode.
 * @param position: camera mounted position
 * @param enable: ture to enable, false to diasble.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetAELockEnabled(bool enable);

/**
 * @brief Get AE lock mode.
 * @note Camera L1/P1 don't support this API.
 * @param position: camera mounted position
 * @param enable: result of AE lock mode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetAELockEnabled(bool *enable);

/**
 * @brief Reset camera settings.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_ResetCameraSettings(void);


/**
 * @brief Start to take video of the selected camera mounted position.
 * @note Camera must be in RECORD_VIDEO mode. For thermal imaging camera,
 * user can take Single photo when recording video.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StartRecordVideo(void);

/**
 * @brief Stop to take video of the selected camera mounted position.
 * @note Precondition: The camera is recording currently.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StopRecordVideo(void);


/**
 * @brief Get camera recording state.
 * @param position: camera mounted position
 * @param recordingState: result of getting, see E_UAVCameraManagerRecordingState.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetRecordingState(E_UAVCameraManagerRecordingState *recordingState);

/**
 * @brief Get camera recording time.
 * @note L1/P1 don not support this API.
 * @param position: camera mounted position
 * @param recordingTime: result of getting, unit is seconds.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetRecordingTime(uint16_t *recordingTime);



/**
 * @brief Get camera stream source range.
 * @param position: camera mounted position
 * @param rangeList: pointer to the result.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetStreamSourceRange(T_UAVCameraManagerRangeList *rangeList);

/**
 * @brief Choose camera stream source.
 * @param position: camera mounted position
 * @param streamSource: stream source to be chose.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetStreamSource(E_UAVCameraManagerStreamSource streamSource);

/**
 * @brief Get photo storage format range.
 * @param position: camera mounted position
 * @param rangeList: range list returned value
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoFormatStorageRange(T_UAVCameraManagerRangeList *rangeList);

/**
 * @brief Set photo storage format.
 * @param position: camera mounted position
 * @param format: storage format.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetPhotoFormat(E_UAVCameraManagerPhotoStorageFormat format);

/**
 * @brief Get photo storage format.
 * @param position: camera mounted position
 * @param format: returned value of storage format.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoFormat(E_UAVCameraManagerPhotoStorageFormat *format);


/**
 * @brief Get video storage format range.
 * @param position: camera mounted position
 * @param rangeList: range list returned value
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetVideoFormatRange(T_UAVCameraManagerRangeList *rangeList);

/**
 * @brief Set video storage format.
 * @param position: camera mounted position
 * @param format: storage format.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetVideoStorageFormat(E_UAVCameraManagerVideoStorageFormat format);

/**
 * @brief Get video storage format.
 * @param position: camera mounted position
 * @param format: returned value of storage format.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetVideoFormat(E_UAVCameraManagerVideoStorageFormat *format);


/**
 * @brief Get photo ratio range
 * @param position: camera mounted position
 * @param rangeList: range list returned value
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoRatioRange(T_UAVCameraManagerRangeList *rangeList);

/**
 * @brief Set camera photo ratio
 * @param position: camera mounted position
 * @param photoRatio: ratio to be set
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetPhotoRatio(E_UAVCameraManagerPhotoRatio photoRatio);

/**
 * @brief Get camera photo ratio
 * @param position: camera mounted position
 * @param photoRatio: returned value of photo ratio
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPhotoRatio(E_UAVCameraManagerPhotoRatio *photoRatio);

/**
 * @brief Get camera video resolution and frame rate
 * @param position: camera mounted position
 * @param photoRatio: returned value of video resolution and frame rate
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetVideoResolutionFrameRate(T_UAVCameraManagerVideoFormat *videoParam);


/**
 * @brief Get night scene mode range.
 
 * @param tempRange: returned valued of night scene mode range.
 * @return Execution result.
 */
bool UAV_CameraManager_GetNightSceneModeRange(T_UAVCameraManagerRangeList *rangeList);

/**
 * @brief Set night scene mode.
 * @note Make sure that stream source is zoom or wide camera.
 * @param position: camera mounted position
 * @param nightSceneMode: night scene mode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetNightSceneMode(E_UAVCameraManagerNightSceneMode nightSceneMode);

/**
 * @brief Get night scene mode.
 * @param position: camera mounted position
 * @param nightSceneMode: pointer to night scene mode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetNightSceneMode(E_UAVCameraManagerNightSceneMode *nightSceneMode);

/**
 * @brief Get range of stream source(s) can be storaged when capturing or recording.
 
 * @param rangeList: returned value of range, in member streamStorage.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetStreamStorageRange(T_UAVCameraManagerRangeList *rangeList);

/**
 * @brief Select capture or recording stream(s) to store.
 * @note Precondition: set camera's work corresponding to streamType
 
 * @param streamType: capture mode or recording mode.
 * @param streamStorageList: Pointer to the struct that contains stream list.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetCaptureRecordingStreams(E_UAVCameraManagerCaptureOrRecording streamType,
                                                            T_UAVCameraManagerStreamList *streamStorageList);

/**
 * @brief Get the stream(s) of capture or recording mode to be storaged.
 
 * @param streamType: capture mode or recording mode.
 * @param streamSourceList: the real return value.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetCaptureRecordingStreams( E_UAVCameraManagerCaptureOrRecording streamType,
                                                            T_UAVCameraManagerStreamList *streamStorageList);

/**
 * @brief Turn on/off synchronized split screen zoom function.
 
 * @param enable: set true to turn on, false to turn off.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetSynchronizedSplitScreenZoomEnabled( bool enable);


/**
 * @brief Set suffix name of directory or file.
 * @note For file name, the setting is only valid once.
 
 * @param nameType: see E_UAVCameraManagerExpandNameType, select to set name of directory or file.
 * @param nameSize: size of name string, must be in rang of 1 ~ 239.
 * @param nameStr: Content of custom suffix name.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetCustomExpandName( E_UAVCameraManagerExpandNameType nameType,
                                                     const uint8_t *nameStr,
                                                     uint32_t nameSize);

/**
 * @brief Get custom past of lastest directory or file name
 * @param position: camera mounted position
 * @param nameType: to choose directory or file to get custom name
 * @param nameStr: name string buffer
 * @param nameSize: its tell the max size of nameStr and changed to to the actually size of
 * name string when function finished.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetCustomExpandName( E_UAVCameraManagerExpandNameType nameType,
                                                     uint8_t *nameStr,
                                                     uint32_t *nameSize);


/**
 * @brief Regsiter selected camera download file data callback,
 * @param position: the mount position of the camera
 * @param callback: the download file data callback
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_RegDownloadFileDataCallback( UAVCameraManagerDownloadFileDataCallback callback);

/**
 * @brief Download selected camera media file by file index.
 * @note Only support download one file at the same time, the new file download need wait for the previous file
 * download finished.The interface is a synchronous interface, which occupies more CPU resources when using it.
 * If the download file fails, the timeout time is 3S.
 * @param position: the mount position of the camera
 * @param fileIndex: the index of the camera media file
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_DownloadFileByIndex( uint32_t fileIndex);

/**
 * @brief Before downloading media file(s), downloader rights should be obtained.
 * @param position: the mount position of the camera
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_ObtainDownloaderRights(void);

/**
 * @brief After downloading media file(s), downloader rights should be released
 * @note If not release rights, the pilot app is probably can't access album of camera.
 * @param position: the mount position of the camera
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_ReleaseDownloaderRights(void);

/**
 * @brief Delete selected camera media file by file index.
 * @param position: the mount position of the camera
 * @param fileIndex: the index of the camera media file
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_DeleteFileByIndex( uint32_t fileIndex);


/**
 * @brief Format SD card.
 * @param position: the mount position of the camera
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_FormatStorage(void);


/**
 * @brief Get storage info of SD card.
 * @note Camera L1/P1 don't support this API.
 * @param position: the mount position of the camera
 * @param storageInfo: result of getting, storage info of SD card.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetStorageInfo( T_UAVCameraManagerStorageInfo *storageInfo);

/**
 * @brief Get the camera laser ranging info of the selected camera mounted position.
 * @note Maximum data update frequency: 5Hz.
 * @param position: the mount position of the camera
 * @param laserRangingInfo: the pointer to the camera laser ranging info
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetLaserRangingInfo( T_UAVCameraManagerLaserRangingInfo *laserRangingInfo);

/**
 * @brief Set point thermometry coordinates of the selected camera mounted position.
 * @param position: camera mounted position
 * @param pointCoordinate: point thermometry coordinates
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetPointThermometryCoordinate( T_UAVCameraManagerPointThermometryCoordinate pointCoordinate);

/**
 * @brief Get point thermometry result.
 * @note Before get point thermometry data from camera, UAV_CameraManager_SetPointThermometryCoordinate()
 * function has to be called.
 * @param position: camera mounted position
 * @param pointThermometryData: point thermometry result
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetPointThermometryData( T_UAVCameraManagerPointThermometryData *pointThermometryData);

/**
 * @brief Set area thermometry coordinates of the selected camera mounted position.
 * @param position: camera mounted position
 * @param areaCoordinate: area thermometry coordinates
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetAreaThermometryCoordinate( T_UAVCameraManagerAreaThermometryCoordinate areaCoordinate);

/**
 * @brief Get area thermometry result.
 * @note Before get area thermometry data from camera, UAV_CameraManager_SetAreaThermometryCoordinate()
 * function has to be called.
 * @param position: camera mounted position
 * @param areaThermometryData: area thermometry result
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetAreaThermometryData( T_UAVCameraManagerAreaThermometryData *areaThermometryData);

/**
 * @brief Set FFC mode.
 
 * @param ffcMode: mode to be set.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetFfcMode( E_UAVCameraManagerFfcMode ffcMode);



/**
 * @brief Trigger FFC one time.
 
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_TriggerFfc(void);



/**
 * @brief Set infrared camera gaim mode.
 
 * @param gainMode: gain mode to set.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetInfraredCameraGainMode(E_UAVCameraManagerIrGainMode gainMode);

/**
 * @brief Get temprature range of infrared camera.
 
 * @param tempRange: returned valued of temperature range.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetInfraredCameraGainModeTemperatureRange(T_UAVCameraManagerIrTempMeterRange *tempRange);

/**
 * @brief Set camera metering mode.
 * @param position: camera mounted position
 * @param meteringMode: metering mode
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetMeteringMode(E_UAVCameraManagerMeteringMode meteringMode);

/**
 * @brief Get camera metering mode.
 * @param position: camera mounted position
 * @param meteringMode: pointer to returned value of metering mode
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetMeteringMode(E_UAVCameraManagerMeteringMode *meteringMode);

/**
 * @brief Get range of metering point.
 * @param position: camera mounted position
 * @param hrzNum: returned value, horizontal range.
 * @param vtcNum: returned value, vertical range.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetMeteringPointRegionRange(uint8_t *hrzNum, uint8_t *vtcNum);
/**
 * @brief Set metrting point.
 * @param position: camera mounted position
 * @param x: Horizontal coordinate value, should be no greater than hrzNum - 1.
 * @param y: Horizontal coordinate value, should be no greater than vtcNum - 1.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_SetMeteringPoint(T_UAVCameraManagerMeteringPosData meteringPosData);

/**
 * @brief Get camera metering mode.
 * @param position: camera mounted position
 * @param x: returned valued, current metering point in horizontal coordinate.
 * @param y: returned valued, current metering point in vertical coordinate.
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_GetMeteringPoint(T_UAVCameraManagerMeteringPosData *meteringPosData);

/**
 * @brief Start to record point cloud of the selected camera mounted position.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StartRecordPointCloud(void);

/**
 * @brief Stop to record point cloud of the selected camera mounted position.
 * @note Precondition: The camera is recording currently.
 * @param position: camera mounted position
 * @return Execution result.
 */
T_UAVReturnCode UAV_CameraManager_StopRecordPointCloud(void);

#ifdef __cplusplus
}
#endif

#endif // UAV_CAMERA_MANAGER_H
/************************ (C) COPYRIGHT UAV Innovations *******END OF FILE******/
