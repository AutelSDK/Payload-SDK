#include "test_camera_manager.h"
#include "uav_core.h"
#include "uav_logger.h"
#include "FreeRTOS.h"
#define CHECK(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                UAV_LOG_ERROR("%s failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                UAV_LOG_INFO("%s success",str);                 \
                            }

#define UAV_CAMERA_MANAGER_TASK_STACK_SIZE   (1024*2)
#define UAV_CAMERA_MANAGER_TASK_PRIORITY     1
static T_UAVOsalHandler *os_handler = nullptr;
static void test_camera_manager_task(void *param);
static void camera_manager_test(void);
static T_UAVTaskHandle s_cameraManagerTaskHandle = nullptr;
T_UAVReturnCode UAV_Test_CameraManagerStartService(void)
{
    T_UAVReturnCode iRet;

    UAV_LOG_INFO("==========camera_manager_routine==========");
    iRet = UAV_CameraManager_Init();
    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("camera manager init failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    os_handler = UAV_Platform_GetOsalHandler();
    if(os_handler == nullptr)
    {
        UAV_LOG_ERROR("os_handler is nullptr");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    // camera_manager_test();

    os_handler->TaskCreate("test_camera_manager_task",
                            test_camera_manager_task,
                            UAV_CAMERA_MANAGER_TASK_STACK_SIZE,
                            nullptr,
                            UAV_CAMERA_MANAGER_TASK_PRIORITY,
                            &s_cameraManagerTaskHandle);
    if(s_cameraManagerTaskHandle == nullptr)
    {
        UAV_LOG_ERROR("task create failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
void camera_manager_task_stack_size_printf(void)
{
    UBaseType_t stacksize = 0;
    if(s_cameraManagerTaskHandle != nullptr) stacksize = uxTaskGetStackHighWaterMark(s_cameraManagerTaskHandle );
    UAV_LOG_INFO("camera manager task stack size is [%d] ", stacksize);
}

static void camera_manager_test(void)
{
    T_UAVReturnCode iRet;
    camera_manager_task_stack_size_printf();
    iRet = UAV_CameraManager_SetMode(UAV_CAMERA_MANAGER_WORK_MODE_SHOOT_PHOTO);
    CHECK("set shoot photo mode", iRet);
	
    iRet = UAV_CameraManager_SetShootPhotoMode(UAV_CAMERA_MANAGER_SHOOT_PHOTO_MODE_SINGLE);
    CHECK("set shoot photo mode single", iRet);


    iRet = UAV_CameraManager_SetPhotoBurstCount(UAV_CAMERA_BURST_COUNT_3);
    CHECK("set photo burst count", iRet);

    iRet = UAV_CameraManager_SetPhotoAEBCount(UAV_CAMERA_MANAGER_PHOTO_AEB_COUNT_3);
    CHECK("set photo aeb count", iRet);

    iRet = UAV_CameraManager_SetPhotoTimeIntervalSettings(2);
    CHECK("set photo time interval settings", iRet);

    iRet =UAV_CameraManager_SetFocusMode(UAV_CAMERA_MANAGER_FOCUS_MODE_AUTO);
    CHECK("set focus mode", iRet);

    T_UAVCameraManagerFocusPosData focusPosSetData;
    focusPosSetData.focusX = 0.5f;
    focusPosSetData.focusY = 0.5f;
    iRet = UAV_CameraManager_SetFocusTarget(focusPosSetData);
    CHECK("set focus target", iRet);

    iRet = UAV_CameraManager_SetFocusRingValue(20);
    CHECK("set focus ring value", iRet);

    iRet = UAV_CameraManager_SetExposureMode(UAV_CAMERA_MANAGER_EXPOSURE_MODE_PROGRAM_AUTO);
    CHECK("set exposure mode", iRet);


    iRet = UAV_CameraManager_SetISO(UAV_CAMERA_MANAGER_ISO_400);
    CHECK("set iso", iRet);

    iRet =  UAV_CameraManager_SetAperture(UAV_CAMERA_MANAGER_APERTURE_F_2);
    CHECK("set aperture", iRet);

    iRet =  UAV_CameraManager_SetShutterSpeed(UAV_CAMERA_MANAGER_SHUTTER_SPEED_1);
    CHECK("set shutter speed", iRet);

    iRet =  UAV_CameraManager_SetExposureCompensation(UAV_CAMERA_MANAGER_EXPOSURE_COMPENSATION_P_1_0);
    CHECK("set exposure compensation", iRet);

    iRet =  UAV_CameraManager_SetAELockEnabled(false);
    CHECK("set ae lock enabled", iRet);

    iRet =  UAV_CameraManager_SetPhotoFormat(UAV_CAMERA_MANAGER_PHOTO_STORAGE_FORMAT_JPEG);
    CHECK("set photo format", iRet);

    iRet =  UAV_CameraManager_SetVideoStorageFormat(UAV_CAMERA_MANAGER_VIDEO_STORAGE_FORMAT_MP4);
    CHECK("set video storage format", iRet);

    iRet =  UAV_CameraManager_SetPhotoRatio(UAV_CAMERA_MANAGER_PHOTO_RATIO_4X3);
    CHECK("set photo ratio", iRet);
    
    iRet =  UAV_CameraManager_SetInfraredCameraGainMode(UAV_CAMERA_MANAGER_IR_GAIN_MODE_LOW);
    CHECK("set infrared camera gain mode", iRet);

    iRet =  UAV_CameraManager_SetMeteringMode(UAV_CAMERA_MANAGER_METERING_MODE_CENTRAL);
    CHECK("set metering mode", iRet);

    T_UAVCameraManagerRangeList rangeList;
    iRet = UAV_CameraManager_GetStreamSourceRange(&rangeList);
    CHECK("get stream source range", iRet);

    iRet = UAV_CameraManager_GetPhotoFormatStorageRange(&rangeList);
    CHECK("get photo format storage range", iRet);
#if 1
    iRet = UAV_CameraManager_GetVideoFormatRange(&rangeList);
    CHECK("get video format range", iRet);

    E_UAVCameraManagerWorkMode mode;
    iRet =UAV_CameraManager_GetMode(&mode);
    CHECK("get mode", iRet);

    E_UAVCameraManagerShootPhotoMode takePhotoMode;
    iRet =UAV_CameraManager_GetShootPhotoMode(&takePhotoMode);
    CHECK("get take photo mode", iRet);

    E_UAVCameraBurstCount count;
    iRet =UAV_CameraManager_GetPhotoBurstCount(&count);
    CHECK("get photo burst count", iRet);

    E_UAVCameraManagerPhotoAEBCount aebCount;
    iRet =UAV_CameraManager_GetPhotoAEBCount(&aebCount);
    CHECK("get photo aeb count", iRet);

    uint32_t interval;
    iRet =UAV_CameraManager_GetPhotoTimeIntervalSettings(&interval);
    CHECK("get photo time interval settings", iRet);

    E_UAVCameraManagerFocusMode focusMode;
    iRet =UAV_CameraManager_GetFocusMode(&focusMode);
    CHECK("get focus mode", iRet);

    T_UAVCameraManagerFocusPosData focusPosData;
    iRet =UAV_CameraManager_GetFocusTarget(&focusPosData);
    CHECK("get focus target", iRet);

    uint32_t value;
    iRet =UAV_CameraManager_GetFocusRingValue(&value);
    CHECK("get focus ring value", iRet);


    E_UAVCameraManagerISO iso;
    iRet =UAV_CameraManager_GetISO(&iso);
    CHECK("get iso", iRet);

    E_UAVCameraManagerAperture aperture;
    iRet =UAV_CameraManager_GetAperture(&aperture);
    CHECK("get aperture", iRet);

    E_UAVCameraManagerShutterSpeed shutterSpeed;
    iRet =UAV_CameraManager_GetShutterSpeed(&shutterSpeed);
    CHECK("get shutter speed", iRet);

    E_UAVCameraManagerExposureCompensation ev;
    iRet =UAV_CameraManager_GetExposureCompensation(&ev);
    CHECK("get exposure compensation", iRet);

    bool en;
    iRet =UAV_CameraManager_GetAELockEnabled(&en);
    CHECK("get ae lock enabled", iRet);

    E_UAVCameraManagerPhotoStorageFormat format;
    iRet =UAV_CameraManager_GetPhotoFormat(&format);
    CHECK("get photo format", iRet);

    E_UAVCameraManagerPhotoRatio photoRatio;
    iRet =UAV_CameraManager_GetPhotoRatio(&photoRatio);
    CHECK("get photo ratio", iRet);

    E_UAVCameraManagerMeteringMode meteringMode;
    iRet =UAV_CameraManager_GetMeteringMode(&meteringMode);
    CHECK("get metering mode", iRet);

    E_UAVCameraType cameraType;
    iRet =UAV_CameraManager_GetCameraType(&cameraType);
    CHECK("get camera type", iRet);

    T_UAVCameraManagerFirmwareVersion firmwareVersion;
    iRet =UAV_CameraManager_GetFirmwareVersion(&firmwareVersion);    
    CHECK("get firmware version", iRet);

    bool bret;
    iRet =UAV_CameraManager_GetCameraConnectStatus(&bret);
    CHECK("get camera connect status", iRet);

    E_UAVCameraManagerCapturingState capturingState;
    iRet =UAV_CameraManager_GetCapturingState(&capturingState);
    CHECK("get capturing state", iRet);

    E_UAVCameraZoomSpeed zoomSpeed = UAV_CAMERA_ZOOM_SPEED_SLOWEST;
    E_UAVCameraZoomDirection zoomDirection = UAV_CAMERA_ZOOM_DIRECTION_OUT;
    iRet =UAV_CameraManager_StartContinuousOpticalZoom(zoomDirection,zoomSpeed);
    CHECK("start continuous optical zoom", iRet);

    iRet =UAV_CameraManager_StopContinuousOpticalZoom();
    CHECK("stop continuous optical zoom", iRet);


    zoomDirection = UAV_CAMERA_ZOOM_DIRECTION_OUT;
    uav_f32_t factor = 2.0;
    iRet =UAV_CameraManager_SetOpticalZoomParam(zoomDirection, factor);
    CHECK("set optical zoom param", iRet);

    T_UAVCameraManagerOpticalZoomParam opticalZoomParam;
    iRet =UAV_CameraManager_GetOpticalZoomParam(&opticalZoomParam);
    CHECK("get optical zoom param", iRet);

    factor = 2.0;
    iRet =UAV_CameraManager_SetInfraredZoomParam(factor);
    CHECK("set infrared zoom param", iRet);


    E_UAVCameraManagerRecordingState recordingState;
    iRet =UAV_CameraManager_GetRecordingState(&recordingState);
    CHECK("get recording state", iRet);

    T_UAVCameraManagerMeteringPosData meteringPosData;
    meteringPosData.focusX = 0.5f;
    meteringPosData.focusY = 0.5f;
    iRet =UAV_CameraManager_SetMeteringPoint(meteringPosData);
    CHECK("set metering point", iRet);

    iRet =UAV_CameraManager_GetMeteringPoint(&meteringPosData);
    CHECK("get metering point", iRet);

    T_UAVCameraManagerVideoFormat videoParam;
    iRet =UAV_CameraManager_GetVideoResolutionFrameRate(&videoParam);
    CHECK("get video resolution frame rate", iRet);

    en= true;
    iRet =UAV_CameraManager_SetSynchronizedSplitScreenZoomEnabled(en);
    CHECK("set synchronized split screen zoom enabled", iRet);

    T_UAVCameraManagerStorageInfo storageInfo;
    iRet =UAV_CameraManager_GetStorageInfo(&storageInfo);
    CHECK("get storage info", iRet);

    E_UAVCameraManagerFfcMode ffcMod = UAV_CAMERA_MANAGER_FFC_MODE_MANUAL;
    iRet =UAV_CameraManager_SetFfcMode(ffcMod);
    CHECK("set ffc mode", iRet);

    iRet =UAV_CameraManager_FormatStorage();
    CHECK("format storage", iRet);

    iRet =UAV_CameraManager_ResetCameraSettings();
    CHECK("reset camera settings", iRet);


    iRet = UAV_CameraManager_StartShootPhoto();
    CHECK("start shoot photo", iRet);

    iRet = UAV_CameraManager_StopShootPhoto();
    CHECK("stop shoot photo", iRet);
#endif
}

static void test_camera_manager_task(void *param)
{
    camera_manager_test();
    while(1)
    {
        os_handler->TaskSleepMs(1000);
        break;
    }
    if(s_cameraManagerTaskHandle != nullptr)
        os_handler->TaskDestroy(s_cameraManagerTaskHandle);
    else 
        UAV_LOG_ERROR("task handle is nullptr");
}