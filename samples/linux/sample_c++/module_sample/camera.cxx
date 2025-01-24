#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_sdk_app_info.h"
#include "uav_camera_manager.h"
#include "uav_positioning.h"
static void cameraManagerThreadFunc()
{
    //wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }
    int result = UAVCameraManager_Init();
    LOG_INFO("UAVCameraManager_Init : {}", result);
    {
        int ret = 0;
        ret = UAVCameraManager_StartShootPhoto();
        LOG_INFO("UAVCameraManager_StartShootPhoto: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_StartRecordVideo();
        LOG_INFO("UAVCameraManager_StartRecordVideo: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        ret = UAVCameraManager_StopRecordVideo();
        LOG_INFO("UAVCameraManager_StopRecordVideo: {}", ret);
    }
        std::this_thread::sleep_for(std::chrono::seconds(4));
    {
        int ret = 0;
        ret = UAVCameraManager_SetMode(UAV_CAMERA_MODE_SHOOT_PHOTO);
        LOG_INFO("camera SetMode: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    
        ret = UAVCameraManager_SetShootPhotoMode(UAV_CAMERA_SHOOT_PHOTO_MODE_SINGLE);
        LOG_INFO("camera SetShootPhotoMode: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetPhotoBurstCount(UAV_CAMERA_BURST_COUNT_3);
        LOG_INFO("camera SetPhotoBurstCount: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetPhotoAEBCount(UAV_CAMERA_MANAGER_PHOTO_AEB_COUNT_3);
        LOG_INFO("camera SetPhotoAEBCount: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetPhotoTimeIntervalSettings(2);
        LOG_INFO("camera SetPhotoTimeIntervalSettings: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetFocusMode(UAV_CAMERA_FOCUS_MODE_AUTO);
        LOG_INFO("camera SetFocusMode: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        T_UAVCameraPointInScreen focusPosSetData;
        focusPosSetData.focusX = 0.5f;
        focusPosSetData.focusY = 0.5f;
        ret = UAVCameraManager_SetFocusTarget(focusPosSetData);
        LOG_INFO("camera SetFocusTarget: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        ret = UAVCameraManager_SetFocusRingValue(20);
        LOG_INFO("camera SetFocusRingValue: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetExposureMode(UAV_CAMERA_MANAGER_EXPOSURE_MODE_PROGRAM_AUTO);
        LOG_INFO("camera SetExposureMode: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetISO(UAV_CAMERA_MANAGER_ISO_400);
        LOG_INFO("camera SetISO: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetAperture(UAV_CAMERA_MANAGER_APERTURE_F_2);
        LOG_INFO("camera SetAperture: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetShutterSpeed(UAV_CAMERA_MANAGER_SHUTTER_SPEED_1);
        LOG_INFO("camera SetShutterSpeed: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetExposureCompensation(UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_0);
        LOG_INFO("camera SetExposureCompensation: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetAELockEnabled(false);
        LOG_INFO("camera SetAELockEnabled: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetPhotoFormat(UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_JPEG);
        LOG_INFO("camera SetPhotoFormat: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetVideoStorageFormat(UAV_CAMERA_MANAGER_VIDEO_STORAGE_FORMAT_MP4);
        LOG_INFO("camera SetVideoStorageFormat: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetPhotoRatio(UAV_CAMERA_MANAGER_PHOTO_RATIO_4X3);
        LOG_INFO("camera SetPhotoRatio: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        ret = UAVCameraManager_SetInfraredCameraGainMode(UAV_CAMERA_MANAGER_IR_GAIN_MODE_LOW);
        LOG_INFO("camera SetInfraredCameraGainMode: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));    
        
        ret = UAVCameraManager_SetMeteringMode(UAV_CAMERA_MANAGER_METERING_MODE_CENTRAL);
        LOG_INFO("camera SetMeteringMode: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        T_UAVCameraManagerRangeList rangeList;
        ret = UAVCameraManager_GetStreamSourceRange(
                    rangeList);
        LOG_INFO("camera GetStreamSourceRange: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));



        T_UAVCameraManagerRangeList rangeList1;
        ret = UAVCameraManager_GetPhotoFormatStorageRange(
                                                        rangeList1);
        LOG_INFO("camera GetPhotoFormatStorageRange: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));


        T_UAVCameraManagerRangeList rangeList2;
        ret = UAVCameraManager_GetVideoFormatRange(
                                                                  rangeList2);
        LOG_INFO("camera GetVideoFormatRange: {}", ret);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        T_UAVPositioningEventInfo eventInfo[2];
        T_UAVPositioningPositionInfo positionInfo[2];
        eventInfo[0].eventIndex =0;
        eventInfo[0].timestamp.year =2024;
        eventInfo[0].timestamp.month =5;
        eventInfo[0].timestamp.day =29;
        eventInfo[0].timestamp.hour =9;
        eventInfo[0].timestamp.minute =20;
        eventInfo[0].timestamp.second =59;
        eventInfo[0].timestamp.milliseconds =500;
        eventInfo[1].eventIndex =1;
        eventInfo[1].timestamp.year =2024;
        eventInfo[1].timestamp.month =5;
        eventInfo[1].timestamp.day =29;
        eventInfo[1].timestamp.hour =9;
        eventInfo[1].timestamp.minute =20;
        eventInfo[1].timestamp.second =60;
        eventInfo[1].timestamp.milliseconds =500;
        UAV_Positioning_GetPositioning_Sync(2, (T_UAVPositioningEventInfo *)&eventInfo,(T_UAVPositioningPositionInfo *)&positionInfo);
    }
    std::this_thread::sleep_for(std::chrono::seconds(4));
    {
        E_UAVCameraMode mode;
        UAVCameraManager_GetMode(mode);
        LOG_INFO("camera GetMode: {}", (int)mode);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraShootPhotoMode takePhotoMode;
        UAVCameraManager_GetShootPhotoMode(takePhotoMode);
        LOG_INFO("camera GetShootPhotoMode: {}", (int)takePhotoMode);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraBurstCount count;
        UAVCameraManager_GetPhotoBurstCount(count);
        LOG_INFO("camera GetPhotoBurstCount: {}", (int)count);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraManagerPhotoAEBCount aebCount;
        UAVCameraManager_GetPhotoAEBCount(aebCount);
        LOG_INFO("camera GetPhotoAEBCount: {}", (int)count);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        uint32_t interval;
        UAVCameraManager_GetPhotoTimeIntervalSettings(interval);
        LOG_INFO("camera GetPhotoTimeIntervalSettings: {}", (int)interval);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraFocusMode focusMode;
        UAVCameraManager_GetFocusMode(focusMode);
        LOG_INFO("camera GetFocusMode: {}", (int)focusMode);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        T_UAVCameraPointInScreen focusPosData;
        UAVCameraManager_GetFocusTarget(focusPosData);
        LOG_INFO("camera GetFocusTarget: {} {}", focusPosData.focusX,focusPosData.focusY);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        uint32_t value;
        UAVCameraManager_GetFocusRingValue(value);
        LOG_INFO("camera GetFocusRingValue: {}", value);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraManagerExposureMode exposureMode;
        UAVCameraManager_GetExposureMode(exposureMode);
        LOG_INFO("camera GetExposureMode: {}", (int)exposureMode);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraManagerISO iso;
        UAVCameraManager_GetISO(iso);
        LOG_INFO("camera GetISO: {}", (int)iso);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        E_UAVCameraManagerAperture aperture;
        UAVCameraManager_GetAperture(aperture);
        LOG_INFO("camera GetAperture: {}", (int)aperture);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraManagerShutterSpeed shutterSpeed;
        UAVCameraManager_GetShutterSpeed(shutterSpeed);
        LOG_INFO("camera GetShutterSpeed: {}", (int)shutterSpeed);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        E_UAVCameraManagerExposureCompensation ev;
        UAVCameraManager_GetExposureCompensation(ev);
        LOG_INFO("camera GetExposureCompensation: {}", (int)ev);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        bool enable;
        UAVCameraManager_GetAELockEnabled(enable);
        LOG_INFO("camera GetAELockEnabled: {}", enable);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        E_UAVCameraManagerPhotoStorageFormat format;
        UAVCameraManager_GetPhotoFormat(format);
        LOG_INFO("camera GetPhotoFormat: {}", (int)format);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        E_UAVCameraManagerPhotoRatio photoRatio;
        UAVCameraManager_GetPhotoRatio(photoRatio);
        LOG_INFO("camera GetPhotoRatio: {}", (int)photoRatio);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        E_UAVCameraManagerMeteringMode meteringMode;
        UAVCameraManager_GetMeteringMode(meteringMode);
        LOG_INFO("camera GetMeteringMode: {}", (int)meteringMode);
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    UAV_Core_SetAlias("cameraV1");
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread cameraManagerThread(cameraManagerThreadFunc);
    cameraManagerThread.detach();
    return UAV_Core_ApplicationStart();
}
