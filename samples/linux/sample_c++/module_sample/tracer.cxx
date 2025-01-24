#include <string>
#include <thread>
#include <sys/time.h>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_upgrade.h"
#include "uav_platform.h"
#include "uav_data_type.h"
#include "uav_sdk_app_info.h"
#include "uav_fc_subscription.h"
#include "uav_low_speed_data_channel.h"
#include "uav_high_speed_data_channel.h"

#include "proto/tracerMessage.pb.h"



void tracerThreadFunc(void);

int main(int argc, char *argv[])
{
    T_AUserInfo usrInfo;

    logger_init(std::string(argv[1]));
    if(false == uav_sdk_app_info_init(&usrInfo)) {
        LOG_ERROR("uav_sdk_app_info_init failed");
        return -1;
    }
    UAV_Core_Init(&usrInfo);
    UAV_Core_SetAlias("tracerV1");

    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);
    // UAV_Uart_Init("/dev/pts/20",115200);

    std::thread tracerThread(tracerThreadFunc);
    tracerThread.detach();

    return UAV_Core_ApplicationStart();
}

/******************************************************************USR Example Code***********************************************************************/
static void myPositionFusedMsgCB(void *data)
{
    T_UAVSubscriptionPositionFused *posSt =  (T_UAVSubscriptionPositionFused*)data;
    LOG_INFO("longitude:{}, latitude:{}, altitude:{}, visibleSatelliteNumber:{}", posSt->longitude, posSt->latitude, posSt->altitude, posSt->visibleSatelliteNumber);
}

static void myPositionGpsMsgCB(void *data)
{
    T_UAVSubscriptionGpsPosition *posSt =  (T_UAVSubscriptionGpsPosition*)data;
    LOG_INFO("longitude:{}, latitude:{}, altitude:{}, visibleSatelliteNumber:{}", posSt->x, posSt->y, posSt->z);
}

static void myEulerAngularVelocityMsgCB(void *data)
{
    T_UAVSubscriptionAngularRateRaw *eulerSt =  (T_UAVSubscriptionAngularRateRaw*)data;
    LOG_INFO("wroll:{}, wpitch:{}, wyaw:{}", eulerSt->x, eulerSt->y, eulerSt->z);
}

static void myVelocityComponentMsgCB(void *data)
{
    T_UAVSubscriptionGpsVelocity *velCompSt =  (T_UAVSubscriptionGpsVelocity*)data;
    LOG_INFO("vX:{}, vY:{}, vZ:{}", velCompSt->x, velCompSt->y, velCompSt->z);
}

static void myEulerAngularMsgCB(void *data)
{
    T_UAVSubscriptionEulerAngular *eulerSt =  (T_UAVSubscriptionEulerAngular*)data;
    LOG_INFO("roll:{}, pitch:{}, yaw:{}", eulerSt->roll, eulerSt->pitch, eulerSt->yaw);
}


//channel callback
static void myWorkModeControlCallback(int32_t srcChannelID, int data_type, const uint8_t *data, uint32_t len)
{
    std::string str;
    autel::protocol::TracerWorkModeControlMessage reqMsg;

    LOG_INFO("myChannelRecvDataCallback from srcChannelID:{}", int(srcChannelID));

    if(!reqMsg.ParseFromArray(data, len)){
        LOG_ERROR("parseMsg TracerWorkModeControlMessage failed");
        return;
    }
    
    LOG_INFO("seq:{}, workMode:{}, dxNum:{}, dxFreq:{}", reqMsg.seq(), reqMsg.workmode(), reqMsg.dxnum(), reqMsg.dxfreq());


    //response
    autel::protocol::TracerWorkModeControlRspMessage rspMsg;
    rspMsg.set_seq(reqMsg.seq());
    rspMsg.set_rlt(0);
    rspMsg.SerializeToString(&str);

    UAV_LowSpeedDataChannel_SendData(srcChannelID, PAYLOAD_TRACER_WORKMODE_CONTROL, (const uint8_t *)str.c_str(), str.size()); //ack
}

static bool getModuleInfo(T_ModuleInfo &info)
{
    info.model = "tracer";
    info.app_version = "V1.0.0";
    info.boot_version = "V1.0.0";
    info.hard_version = "V1.0.0";
    LOG_INFO("getModuleInfo");
    return true;
}

static bool upgradeConditionCheck(void)
{
    LOG_INFO("upgradeConditionCheck");
    return true;
}

static int upgradeExcute(bool force, const std::string &model, const std::string &filename)
{
    LOG_INFO("force: {}, model: {}, filename: {}", force, model.c_str(), filename.c_str());
    return 10;
}

static bool upgradeQuery(T_UAVUpgradeStatus &status)
{
    status.state = 1;
    status.progress = 10;
    status.error_code = 0;
    status.module = "tracer";
    return true;
}

static T_UAVUpgradeHandler g_uavUpgradeHandler = {
    .getModuleInfo = getModuleInfo,
    .upgradeConditionCheck = upgradeConditionCheck,
    .upgradeExcute = upgradeExcute,
    .upgradeQuery = upgradeQuery,
};

void tracerThreadFunc(void)
{
        // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    std::string str;
    T_UAVFileDesc desc;
    std::string model("tracer");

    //升级初始化
    UAV_Upgrade_Init(&g_uavUpgradeHandler);
    //创建低速数据通道回调
    UAV_LowSpeedDataChannel_Init();
    UAV_LowSpeedDataChannel_RegRecvDataCallback(myWorkModeControlCallback);
    // UAV_LowSpeedDataChannel_RegRecvDataCallback(myWorkModeControlCallback);

    //升级固件下载
    if(true == UAV_Upgrade_Firmware_Download(model, desc))
    {
        int64_t read_len = 0;
        std::vector<uint8_t> data;
        int64_t file_size = desc.file_size;
        LOG_INFO("file name: {}, size: {}, package size: {}", desc.file_name, desc.file_size, desc.package_size);
        while(file_size > 0)
        {
            UAV_Upgrade_Firmware_Request(read_len, desc.package_size, 200, data);
            read_len += data.size();
            file_size -= data.size();
            LOG_INFO("download data size: {}", data.size());
        }
        UAV_Upgrade_Firmware_Verify(DIGEST_TYPE_CRC32, "12345678", 50);
        UAV_Upgrade_Firmware_Finish("seccuss", 50);
    }
    //订阅飞机GPS位置信息
    UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_POSITION, 0, myPositionGpsMsgCB);

    //订阅飞机位置信息
    UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_POSITION_FUSED, 0, myPositionFusedMsgCB);

    // 订阅飞机欧拉角速度信息
    UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_ANGULAR_RATE_RAW, 0, myEulerAngularVelocityMsgCB);

    // 订阅飞机速度分量信息
    UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GPS_VELOCITY, 0, myVelocityComponentMsgCB);

    // 订阅飞机姿态欧拉角信息
    UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_EULER_ANGULAR_INFO, 0, myEulerAngularMsgCB);

    while(1){
        sleep(1);
        //1-发布检测结果
        autel::protocol::tracerMeasureInfoMessage tracerMeasureInfoMsg;
        tracerMeasureInfoMsg.set_workstatus(1);
        tracerMeasureInfoMsg.set_tarnum(3);
        for(int i = 0 ; i < 3 ; i ++){
            autel::protocol::singleTargetInfo *info = tracerMeasureInfoMsg.add_tarinfo();
            info->set_tnum(1);
            info->set_tfreq(2);
            info->set_tamp(3);
            info->set_tmfw(4); 
            info->set_tmfy(5); 
            info->set_tffw(6); 
            info->set_tffy(7);
        }
        tracerMeasureInfoMsg.SerializeToString(&str);
        UAV_LowSpeedDataChannel_SendData(UAV_CHANNEL_ADDRESS_MASTER_RC_APP, PAYLOAD_TRACER_MEASURE_INFO, (const uint8_t *)str.c_str(), str.size());

        sleep(1);
        //2-发布错频信息
        autel::protocol::TracerStaggerFreqMessage TracerStaggerFreqMsg;
        TracerStaggerFreqMsg.set_seq(101);
        TracerStaggerFreqMsg.set_freqband(102);
        TracerStaggerFreqMsg.set_timstamp(103);
        TracerStaggerFreqMsg.set_reserv(104);
        TracerStaggerFreqMsg.SerializeToString(&str);
        UAV_LowSpeedDataChannel_SendData(UAV_CHANNEL_ADDRESS_MASTER_RC_APP, PAYLOAD_TRACER_STAGGER_FREQ, (const uint8_t *)str.c_str(), str.size());

        sleep(1);
        //3-发布系统状态信息
        autel::protocol::TracerSystemInfoMessage TracerSystemInfoMsg;
        TracerSystemInfoMsg.set_workstatus(12);
        TracerSystemInfoMsg.set_faultstatus(13);
        TracerSystemInfoMsg.set_netstatus(14);
        TracerSystemInfoMsg.set_ssid("unknow");
        TracerSystemInfoMsg.set_ipaddress("192.168.1.111");
        TracerSystemInfoMsg.SerializeToString(&str);
        UavHighSpeedDataChannel_SendDataStreamData(5, (const uint8_t *)str.c_str(), str.size());
        UAV_LowSpeedDataChannel_SendData(UAV_CHANNEL_ADDRESS_MASTER_RC_APP, PAYLOAD_TRACER_SYSTEM_INFO, (const uint8_t *)str.c_str(), str.size());
    }

}

