/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef UAV_LIVEVIEW_H
#define UAV_LIVEVIEW_H

/* Includes ------------------------------------------------------------------*/
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef enum {
    UAV_LIVEVIEW_CAMERA_SOURCE_DEFAULT = 0,
    UAV_LIVEVIEW_CAMERA_SOURCE_4T_IR = 1,
    UAV_LIVEVIEW_CAMERA_SOURCE_4T_VIS = 2,
    UAV_LIVEVIEW_CAMERA_SOURCE_4TPRO_IR = 3,
    UAV_LIVEVIEW_CAMERA_SOURCE_4TPRO_VIS = 4,
    UAV_LIVEVIEW_CAMERA_SOURCE_4N_IR = 5,
    UAV_LIVEVIEW_CAMERA_SOURCE_4N_WIDE = 6,
    UAV_LIVEVIEW_CAMERA_SOURCE_4N_NIGHT = 7,
    UAV_LIVEVIEW_CAMERA_SOURCE_L35T_IR = 8,
    UAV_LIVEVIEW_CAMERA_SOURCE_L35T_WIDE = 9,
    UAV_LIVEVIEW_CAMERA_SOURCE_L35T_ZOOM = 10,
    UAV_LIVEVIEW_CAMERA_SOURCE_4TH_IR = 11,
    UAV_LIVEVIEW_CAMERA_SOURCE_4TH_WIDE = 12,
    UAV_LIVEVIEW_CAMERA_SOURCE_4TH_ZOOM = 13,
}E_UAVLiveViewCameraSource;

/**
 * @brief Liveview camera h264 stream callback.
 */
typedef void (*UAVLiveView_H264Callback)(int32_t payload_id, const uint8_t *buf, uint32_t len);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the liveview module.
 */
T_UAVReturnCode UAV_LiveView_Init(void);

/**
 * @brief Deinitialize the liveview module.
 */
T_UAVReturnCode UAV_LiveView_Deinit(void);

/**
 * @brief Start the FPV or camera H264 stream from the specified position.
 * @param position: Camera position for the H264 stream output.
 * @param source: sub-camera source for the H264 stream output.
 * @param callback: Callback function in a callback thread when a new h264 frame is received
 */
T_UAVReturnCode UAV_LiveView_StartH264Stream(int32_t payload_id, E_UAVLiveViewCameraSource source, UAVLiveView_H264Callback callback);

/**
 * @brief Stop the FPV or Camera H264 Stream from the specified position.
 * @param position: Camera position for the H264 stream output.
 * @param source: sub-camera source for the H264 stream output.
 */
T_UAVReturnCode UAV_LiveView_StopH264Stream(int32_t payload_id, E_UAVLiveViewCameraSource source);

/**
 * @brief Request the intraframe Frame of camera H264 stream from the specified position.
 * @param position: Camera position for the H264 stream output.
 * @param source: sub-camera source for the H264 stream output.
 */
T_UAVReturnCode UAV_LiveView_RequestIntraFrameData(int32_t payload_id, E_UAVLiveViewCameraSource source);

#ifdef __cplusplus
}
#endif

#endif // UAV_LIVEVIEW_H
