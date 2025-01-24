#include <string>
#include <thread>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_sdk_app_info.h"
#include "uav_low_speed_data_channel.h"
#include "uav_high_speed_data_channel.h"

#include "proto/tracerMessage.pb.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }

//channel callback
static void myWorkModeControlCallback(int32_t srcChannelID, int data_type, const uint8_t *data, uint32_t len)
{
    LOG_INFO("myWorkModeControlCallback srcChannelID:{}, data_type:{}, len:{}", (int)srcChannelID, data_type, len);
}

static void tracerThreadFunc(void)
{
    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    {
        // 测试 UAV_LowSpeedDataChannel_Init 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LowSpeedDataChannel_Init 函数");
        T_UAVReturnCode iRet =UAV_LowSpeedDataChannel_Init();
        CHECK_TEST("UAV_LowSpeedDataChannel_Init iRet:{}",iRet);
    }

    {
        // 测试 UavHighSpeedDataChannel_SetBandwidthProportion 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UavHighSpeedDataChannel_SetBandwidthProportion 函数");
        T_UavDataChannelBandwidthProportionOfHighspeedChannel BandwidthProportion;
        BandwidthProportion.dataStream = 50;
        BandwidthProportion.downloadStream= 30;
        BandwidthProportion.downloadStream= 20;
        T_UAVReturnCode iRet =UavHighSpeedDataChannel_SetBandwidthProportion(&BandwidthProportion);
        CHECK_TEST("UavHighSpeedDataChannel_SetBandwidthProportion iRet:{}",iRet);
    }

    {
        // 测试 UAV_LowSpeedDataChannel_RegRecvDataCallback 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LowSpeedDataChannel_RegRecvDataCallback 函数");
        T_UAVReturnCode iRet =UAV_LowSpeedDataChannel_RegRecvDataCallback(myWorkModeControlCallback);
        CHECK_TEST("UAV_LowSpeedDataChannel_RegRecvDataCallback iRet:{}",iRet);
    }
    
    while(true)
    {
        // 测试 UAV_LowSpeedDataChannel_SendData 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LowSpeedDataChannel_SendData 函数");
        std::string str;
        //3-发布系统状态信息
        autel::protocol::TracerSystemInfoMessage TracerSystemInfoMsg;
        TracerSystemInfoMsg.set_workstatus(12);
        TracerSystemInfoMsg.set_faultstatus(13);
        TracerSystemInfoMsg.set_netstatus(14);
        TracerSystemInfoMsg.set_ssid("unknow");
        TracerSystemInfoMsg.set_ipaddress("192.168.1.111");
        TracerSystemInfoMsg.SerializeToString(&str);
        T_UAVReturnCode iRet = UAV_LowSpeedDataChannel_SendData(UAV_CHANNEL_ADDRESS_MASTER_RC_APP, PAYLOAD_TRACER_SYSTEM_INFO, (const uint8_t *)str.c_str(), str.size());
        CHECK_TEST("UAV_LowSpeedDataChannel_SendData iRet:{}",iRet);

        // 测试 UavHighSpeedDataChannel_GetDataStreamState 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UavHighSpeedDataChannel_GetDataStreamState 函数");
        T_UAVDataChannelState dataChannelState;
        iRet =UavHighSpeedDataChannel_GetDataStreamState(&dataChannelState);
        CHECK_TEST("UavHighSpeedDataChannel_GetDataStreamState iRet:{}",iRet);

        // 测试 UavHighSpeedDataChannel_SendDataStreamData 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UavHighSpeedDataChannel_SendDataStreamData 函数");
        iRet = UavHighSpeedDataChannel_SendDataStreamData(5, (const uint8_t *)str.c_str(), str.size());
        CHECK_TEST("UavHighSpeedDataChannel_SendDataStreamData iRet:{}",iRet);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    {
        // 测试 UAV_LowSpeedDataChannel_DeInit 函数的返回值
        // 期望：函数调用成功时返回 UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS
        LOG_INFO("测试 UAV_LowSpeedDataChannel_DeInit 函数");
        UAV_LowSpeedDataChannel_DeInit();
        CHECK_TEST("UAV_LowSpeedDataChannel_DeInit iRet:{}",0);
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
    UAV_Core_SetAlias("dataChannel");
    // UAV_Uart_Init("/dev/pts/20",115200);
    // UAV_Uart_Init(argv[2], 115200); ///< 便于调试串口进行验证
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread tracerThread(tracerThreadFunc);
    tracerThread.detach();

    return UAV_Core_ApplicationStart();
}