#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_liveview.h"
#include "uav_sdk_app_info.h"

static FILE *s_fp = nullptr;

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


static void UAVLiveView_H264StreamCallback(int32_t payload_id, const uint8_t *buf, uint32_t len)
{
    LOG_INFO("camera id: {}, len: {}", payload_id, len);

    if(nullptr != s_fp)
        fwrite((const void *)buf, 1, len, s_fp);
    fsync(fileno(s_fp));
}

static void liveview_routine(void)
{
    //wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }
    {
        // 测试 UAV_LiveView_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LiveView_Init 函数");
        s_fp = fopen("liveview.h264", "wb");
        T_UAVReturnCode iRet =UAV_LiveView_Init();
        CHECK_TEST("UAV_LiveView_Init iRet:{}",iRet);
        LOG_INFO("UAV_LiveView_Init iRet:{}",iRet);
    }

    {
        // 测试 UAV_LiveView_StartH264Stream 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LiveView_StartH264Stream 函数");
        T_UAVReturnCode iRet =UAV_LiveView_StartH264Stream(101, UAV_LIVEVIEW_CAMERA_SOURCE_DEFAULT, UAVLiveView_H264StreamCallback);
        CHECK_TEST("UAV_LiveView_StartH264Stream iRet:{}",iRet);
        LOG_INFO("UAV_LiveView_StartH264Stream iRet:{}",iRet);
    }
    std::this_thread::sleep_for(std::chrono::seconds(15));
    // {
    //     // 测试 UAV_LiveView_RequestIntraFrameData 函数的返回值，注意当前不支持关键帧请求，会导致码流停止
    //     // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
    //     LOG_INFO("测试 UAV_LiveView_RequestIntraFrameData 函数");
    //     T_UAVReturnCode iRet =UAV_LiveView_RequestIntraFrameData(101, UAV_LIVEVIEW_CAMERA_SOURCE_DEFAULT);
    //     CHECK_TEST("UAV_LiveView_RequestIntraFrameData iRet:{}",iRet);
    //     LOG_INFO("UAV_LiveView_RequestIntraFrameData iRet:{}",iRet);
    // }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    {
        // 测试 UAV_LiveView_StopH264Stream 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LiveView_StopH264Stream 函数");
        T_UAVReturnCode iRet =UAV_LiveView_StopH264Stream(101, UAV_LIVEVIEW_CAMERA_SOURCE_DEFAULT);
        CHECK_TEST("UAV_LiveView_StopH264Stream iRet:{}",iRet);
        LOG_INFO("UAV_LiveView_StopH264Stream iRet:{}",iRet);
    }

    {
        // 测试 UAV_LiveView_Deinit 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LiveView_Deinit 函数");
        T_UAVReturnCode iRet =UAV_LiveView_Deinit();
        CHECK_TEST("UAV_LiveView_Deinit iRet:{}",iRet);
        LOG_INFO("UAV_LiveView_Deinit iRet:{}",iRet);
    }
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
    UAV_Core_SetAlias("liveviewV1");
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);
    std::thread liveview(liveview_routine);
    liveview.detach();
    return UAV_Core_ApplicationStart();
}