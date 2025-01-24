#include "test_camera.h"
#include "uav_core.h"
#include "uav_logger.h"

static T_UAVOsalHandler *os_handler = nullptr;

#define CHECK(condition)   if(condition)                                        \
                            {                                                   \
                                UAV_LOG_INFO("success");                        \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                UAV_LOG_ERROR("failed");                        \
                            }

static T_UAVReturnCode GetSystemState(T_UAVCameraSystemState *systemState) {
    UAV_LOG_INFO("测试 camera GetSystemState功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetMode(E_UAVCameraMode mode) {
    UAV_LOG_INFO("测试 camera SetMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMode(E_UAVCameraMode *mode){
    UAV_LOG_INFO("测试 camera GetMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StartRecordVideo(void){
    UAV_LOG_INFO("测试 camera StartRecordVideo功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StopRecordVideo(void){
    UAV_LOG_INFO("测试 camera StopRecordVideo功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StartShootPhoto(void){
    UAV_LOG_INFO("测试 camera StartShootPhoto功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StopShootPhoto(void){
    UAV_LOG_INFO("测试 camera StopShootPhoto功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetShootPhotoMode(E_UAVCameraShootPhotoMode mode){
    UAV_LOG_INFO("测试 camera SetShootPhotoMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetShootPhotoMode(E_UAVCameraShootPhotoMode *mode){
    UAV_LOG_INFO("测试 camera GetShootPhotoMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetPhotoBurstCount(E_UAVCameraBurstCount burstCount){
    UAV_LOG_INFO("测试 camera SetPhotoBurstCount功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetPhotoBurstCount(E_UAVCameraBurstCount *burstCount){
    UAV_LOG_INFO("测试 camera GetPhotoBurstCount功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetPhotoTimeIntervalSettings(uint32_t timeIntervalSeconds){
    UAV_LOG_INFO("测试 camera SetPhotoTimeIntervalSettings功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetPhotoTimeIntervalSettings(uint32_t *timeIntervalSeconds){
    UAV_LOG_INFO("测试 camera GetPhotoTimeIntervalSettings功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetSDCardState(T_UAVCameraSDCardState *sdCardState){
    UAV_LOG_INFO("测试 camera GetSDCardState功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode FormatSDCard(void){
    UAV_LOG_INFO("测试 camera FormatSDCard功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetMeteringMode(E_UAVCameraMeteringMode mode){
    UAV_LOG_INFO("测试 camera SetMeteringMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
    }
static T_UAVReturnCode GetMeteringMode(E_UAVCameraMeteringMode *mode){
    UAV_LOG_INFO("测试 camera GetMeteringMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetSpotMeteringTarget(T_UAVCameraPointInScreen target){
    UAV_LOG_INFO("测试 camera SetSpotMeteringTarget功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode GetSpotMeteringTarget(T_UAVCameraPointInScreen *target)
{
    UAV_LOG_INFO("测试 camera GetSpotMeteringTarget功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


static T_UAVReturnCode SetFocusMode(E_UAVCameraFocusMode mode){
    UAV_LOG_INFO("测试 camera SetFocusMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetFocusMode(E_UAVCameraFocusMode *mode){
    UAV_LOG_INFO("测试 camera GetFocusMode功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetFocusTarget(T_UAVCameraPointInScreen target){
    UAV_LOG_INFO("测试 camera SetFocusTarget功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetFocusTarget(T_UAVCameraPointInScreen *target){
    UAV_LOG_INFO("测试 camera GetFocusTarget功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetFocusAssistantSettings(T_UAVCameraFocusAssistantSettings settings){
    UAV_LOG_INFO("测试 camera SetFocusAssistantSettings功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetFocusAssistantSettings(T_UAVCameraFocusAssistantSettings *settings){
    UAV_LOG_INFO("测试 camera GetFocusAssistantSettings功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetFocusRingValue(uint32_t value){
    UAV_LOG_INFO("测试 camera SetFocusRingValue功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetFocusRingValue(uint32_t *value){
    UAV_LOG_INFO("测试 camera GetFocusRingValue功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetFocusRingValueUpperBound(uint32_t *value){
    UAV_LOG_INFO("测试 camera GetFocusRingValueUpperBound功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetDigitalZoomFactor(uav_f32_t factor){
    UAV_LOG_INFO("测试 camera SetDigitalZoomFactor功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetDigitalZoomFactor(uav_f32_t *factor){
    UAV_LOG_INFO("测试 camera GetDigitalZoomFactor功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetOpticalZoomFocalLength(uint32_t focalLength){
    UAV_LOG_INFO("测试 camera SetOpticalZoomFocalLength功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetOpticalZoomFocalLength(uint32_t *focalLength){
    UAV_LOG_INFO("测试 camera GetOpticalZoomFocalLength功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetOpticalZoomFactor(uav_f32_t *factor){
    UAV_LOG_INFO("测试 camera GetOpticalZoomFactor功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetOpticalZoomSpec(T_UAVCameraOpticalZoomSpec *spec){
    UAV_LOG_INFO("测试 camera GetOpticalZoomSpec功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StartContinuousOpticalZoom(E_UAVCameraZoomDirection direction, E_UAVCameraZoomSpeed speed){
    UAV_LOG_INFO("测试 camera StartContinuousOpticalZoom功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StopContinuousOpticalZoom(void){
    UAV_LOG_INFO("测试 camera StopContinuousOpticalZoom功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}



static T_UAVReturnCode GetMediaFileDir(char *dirPath){
    UAV_LOG_INFO("测试 camera GetMediaFileDir功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaFileOriginInfo(const char *filePath, T_UAVCameraMediaFileInfo *fileInfo){
    UAV_LOG_INFO("测试 camera GetMediaFileOriginInfo功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaFileOriginData(const char *filePath, uint32_t offset, uint32_t length, uint8_t *data){
    UAV_LOG_INFO("测试 camera GetMediaFileOriginData功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode CreateMediaFileThumbNail(const char *filePath){
    UAV_LOG_INFO("测试 camera CreateMediaFileThumbNail功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaFileThumbNailInfo(const char *filePath, T_UAVCameraMediaFileInfo *fileInfo){
    UAV_LOG_INFO("测试 camera GetMediaFileThumbNailInfo功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaFileThumbNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                    uint8_t *data){
    UAV_LOG_INFO("测试 camera GetMediaFileThumbNailData功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode DestroyMediaFileThumbNail(const char *filePath){
    UAV_LOG_INFO("测试 camera DestroyMediaFileThumbNail功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode CreateMediaFileScreenNail(const char *filePath){
    UAV_LOG_INFO("测试 camera CreateMediaFileScreenNail功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaFileScreenNailData(const char *filePath, uint32_t offset, uint32_t length,
                                                    uint8_t *data)
{
    UAV_LOG_INFO("测试 camera GetMediaFileScreenNailData功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaFileScreenNailInfo(const char *filePath, T_UAVCameraMediaFileInfo *fileInfo)
{
    UAV_LOG_INFO("测试 camera GetMediaFileScreenNailInfo功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode DestroyMediaFileScreenNail(const char *filePath){
    UAV_LOG_INFO("测试 camera DestroyMediaFileScreenNail功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StartDownloadNotification(void){
    UAV_LOG_INFO("测试 camera StartDownloadNotification功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StopDownloadNotification(void){
    UAV_LOG_INFO("测试 camera StopDownloadNotification功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode DeleteMediaFile(char *filePath){
    UAV_LOG_INFO("测试 camera DeleteMediaFile功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetMediaPlaybackStatus(T_UAVCameraPlaybackStatus *status){
    UAV_LOG_INFO("测试 camera GetMediaPlaybackStatus功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetMediaPlaybackFile(const char *filePath){
    UAV_LOG_INFO("测试 camera SetMediaPlaybackFile功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StartMediaPlayback(void){
    UAV_LOG_INFO("测试 camera StartMediaPlayback功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode StopMediaPlayback(void){
    UAV_LOG_INFO("测试 camera StopMediaPlayback功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode PauseMediaPlayback(void){
    UAV_LOG_INFO("测试 camera PauseMediaPlayback功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SeekMediaPlayback(uint32_t playbackPosition){
    UAV_LOG_INFO("测试 camera SeekMediaPlayback功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}



static T_UAVReturnCode GetTapZoomState(T_UAVCameraTapZoomState *state){
    UAV_LOG_INFO("测试 camera GetTapZoomState功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetTapZoomEnabled(bool enabledFlag){
    UAV_LOG_INFO("测试 camera SetTapZoomEnabled功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetTapZoomEnabled(bool *enabledFlag){
    UAV_LOG_INFO("测试 camera GetTapZoomEnabled功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode SetTapZoomMultiplier(uint8_t multiplier){
    UAV_LOG_INFO("测试 camera SetTapZoomMultiplier功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode GetTapZoomMultiplier(uint8_t *multiplier){
    UAV_LOG_INFO("测试 camera GetTapZoomMultiplier功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode TapZoomAtTarget(T_UAVCameraPointInScreen target){
    UAV_LOG_INFO("测试 camera TapZoomAtTarget功能函数 done");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static void Test_Item_RegCommonHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegCommonHandler 函数");
    T_UAVCameraCommonHandler *commonHandler =  nullptr;
    commonHandler = (T_UAVCameraCommonHandler *)os_handler->Malloc(sizeof(T_UAVCameraCommonHandler));
    if(commonHandler == nullptr)
    {
        UAV_LOG_ERROR("commonHandler 0");
        return;
    }
    commonHandler->GetSystemState = GetSystemState;
    commonHandler->SetMode = SetMode;
    commonHandler->GetMode = GetMode;
    commonHandler->StartRecordVideo = StartRecordVideo;
    commonHandler->StopRecordVideo = StopRecordVideo;
    commonHandler->StartShootPhoto = StartShootPhoto;
    commonHandler->StopShootPhoto = StopShootPhoto;
    commonHandler->SetShootPhotoMode = SetShootPhotoMode;
    commonHandler->GetShootPhotoMode = GetShootPhotoMode;
    commonHandler->SetPhotoBurstCount = SetPhotoBurstCount;
    commonHandler->GetPhotoBurstCount = GetPhotoBurstCount;
    commonHandler->SetPhotoTimeIntervalSettings = SetPhotoTimeIntervalSettings;
    commonHandler->GetPhotoTimeIntervalSettings = GetPhotoTimeIntervalSettings;
    commonHandler->GetSDCardState = GetSDCardState;
    commonHandler->FormatSDCard = FormatSDCard;
    CHECK(UAV_PayloadCamera_RegCommonHandler(commonHandler)==0);
}

static void Test_Item_RegExposureMeteringHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegExposureMeteringHandler 函数");
    T_UAVCameraExposureMeteringHandler *commonHandler = nullptr;
    commonHandler = (T_UAVCameraExposureMeteringHandler *)os_handler->Malloc(sizeof(T_UAVCameraExposureMeteringHandler));
    commonHandler->SetMeteringMode = SetMeteringMode;
    commonHandler->GetMeteringMode = GetMeteringMode;
    commonHandler->SetSpotMeteringTarget = SetSpotMeteringTarget;
    commonHandler->GetSpotMeteringTarget = GetSpotMeteringTarget;
    CHECK(UAV_PayloadCamera_RegExposureMeteringHandler(commonHandler)==0);
}

static void Test_Item_RegFocusHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegFocusHandler 函数");
    T_UAVCameraFocusHandler *commonHandler = nullptr;
    commonHandler = (T_UAVCameraFocusHandler *)os_handler->Malloc(sizeof(T_UAVCameraFocusHandler));
    commonHandler->SetFocusMode = SetFocusMode;
    commonHandler->GetFocusMode = GetFocusMode;
    commonHandler->SetFocusTarget = SetFocusTarget;
    commonHandler->GetFocusTarget = GetFocusTarget;
    commonHandler->SetFocusAssistantSettings = SetFocusAssistantSettings;
    commonHandler->GetFocusAssistantSettings = GetFocusAssistantSettings;
    commonHandler->SetFocusRingValue = SetFocusRingValue;
    commonHandler->GetFocusRingValue = GetFocusRingValue;
    commonHandler->GetFocusRingValueUpperBound = GetFocusRingValueUpperBound;
    CHECK(UAV_PayloadCamera_RegFocusHandler(commonHandler)==0);
}

static void Test_Item_RegDigitalZoomHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegDigitalZoomHandler 函数");
    T_UAVCameraDigitalZoomHandler *commonHandler = nullptr;
    commonHandler = (T_UAVCameraDigitalZoomHandler *)os_handler->Malloc(sizeof(T_UAVCameraDigitalZoomHandler));
    commonHandler->SetDigitalZoomFactor = SetDigitalZoomFactor;
    commonHandler->GetDigitalZoomFactor = GetDigitalZoomFactor;
    CHECK(UAV_PayloadCamera_RegDigitalZoomHandler(commonHandler)==0);
}

static void Test_Item_RegOpticalZoomHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegOpticalZoomHandler 函数");
    T_UAVCameraOpticalZoomHandler *commonHandler = nullptr;

    commonHandler = (T_UAVCameraOpticalZoomHandler *)os_handler->Malloc(sizeof(T_UAVCameraOpticalZoomHandler));
    commonHandler->SetOpticalZoomFocalLength = SetOpticalZoomFocalLength;
    commonHandler->GetOpticalZoomFocalLength = GetOpticalZoomFocalLength;
    commonHandler->GetOpticalZoomFactor = GetOpticalZoomFactor;
    commonHandler->GetOpticalZoomSpec = GetOpticalZoomSpec;
    commonHandler->StartContinuousOpticalZoom = StartContinuousOpticalZoom;
    commonHandler->StopContinuousOpticalZoom = StopContinuousOpticalZoom;
    CHECK(UAV_PayloadCamera_RegOpticalZoomHandler(commonHandler)==0);
}

static void Test_Item_RegTapZoomHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegTapZoomHandler 函数");
    T_UAVCameraTapZoomHandler *commonHandler = nullptr;

    commonHandler = (T_UAVCameraTapZoomHandler *)os_handler->Malloc(sizeof(T_UAVCameraTapZoomHandler));
    commonHandler->GetTapZoomState = GetTapZoomState;
    commonHandler->SetTapZoomEnabled = SetTapZoomEnabled;
    commonHandler->GetTapZoomEnabled = GetTapZoomEnabled;
    commonHandler->SetTapZoomMultiplier = SetTapZoomMultiplier;
    commonHandler->GetTapZoomMultiplier = GetTapZoomMultiplier;
    commonHandler->TapZoomAtTarget = TapZoomAtTarget;
    CHECK(UAV_PayloadCamera_RegTapZoomHandler(commonHandler)==0);
}

static void Test_Item_RegMediaDownloadPlaybackHandler(void){
    UAV_LOG_INFO("测试 UAV_PayloadCamera_RegMediaDownloadPlaybackHandler 函数");
    T_UAVCameraMediaDownloadPlaybackHandler *commonHandler = nullptr;

    commonHandler = (T_UAVCameraMediaDownloadPlaybackHandler *)os_handler->Malloc(sizeof(T_UAVCameraMediaDownloadPlaybackHandler));
    commonHandler->GetMediaFileDir = GetMediaFileDir;
    commonHandler->GetMediaFileOriginInfo = GetMediaFileOriginInfo;
    commonHandler->GetMediaFileOriginData = GetMediaFileOriginData;
    commonHandler->CreateMediaFileThumbNail = CreateMediaFileThumbNail;
    commonHandler->GetMediaFileThumbNailInfo = GetMediaFileThumbNailInfo;
    commonHandler->GetMediaFileThumbNailData = GetMediaFileThumbNailData;
    commonHandler->DestroyMediaFileThumbNail = DestroyMediaFileThumbNail;
    commonHandler->CreateMediaFileScreenNail = CreateMediaFileScreenNail;
    commonHandler->GetMediaFileScreenNailData = GetMediaFileScreenNailData;
    commonHandler->GetMediaFileScreenNailInfo = GetMediaFileScreenNailInfo;
    commonHandler->DestroyMediaFileScreenNail = DestroyMediaFileScreenNail;
    commonHandler->StartDownloadNotification = StartDownloadNotification;
    commonHandler->StopDownloadNotification = StopDownloadNotification;
    commonHandler->DeleteMediaFile = DeleteMediaFile;
    commonHandler->GetMediaPlaybackStatus = GetMediaPlaybackStatus;
    commonHandler->SetMediaPlaybackFile = SetMediaPlaybackFile;
    commonHandler->StartMediaPlayback = StartMediaPlayback;
    commonHandler->StopMediaPlayback = StopMediaPlayback;
    commonHandler->PauseMediaPlayback = PauseMediaPlayback;
    commonHandler->SeekMediaPlayback = SeekMediaPlayback;
    CHECK(UAV_PayloadCamera_RegMediaDownloadPlaybackHandler(commonHandler)==0);

}

T_UAVReturnCode UAV_Test_CameraStartService(void)
{
    T_UAVReturnCode uavSta;
    UAV_LOG_INFO("==========camera_manager_routine==========");
    os_handler = UAV_Platform_GetOsalHandler();
    if(os_handler == nullptr)
    {
        UAV_LOG_ERROR("os_handler 0");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    uavSta = UAV_PayloadCamera_Init();
    if(uavSta != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        UAV_LOG_ERROR("camera manager init failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    Test_Item_RegCommonHandler();
    Test_Item_RegExposureMeteringHandler();
    Test_Item_RegFocusHandler();
    Test_Item_RegDigitalZoomHandler();
    Test_Item_RegOpticalZoomHandler();
    Test_Item_RegTapZoomHandler();
    Test_Item_RegMediaDownloadPlaybackHandler();
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;


}