/******************************************************************
 * Author     : tyh (tangyinghao@autelrobotics.cn)
 * CreateTime : 2024/2/19
 * Copyright (c) 2024 Shenzhen Autel Robotics Co., Ltd.
 * File Desc  :
 *******************************************************************/
#include "uav_error.h"
#include "uav_widget.h"
#include "uav_logger.h"
#include "uav_register.h"

/* Private constants ---------------------------------------------------------*/
/* The speaker initialization parameters */
#define WIDGET_SPEAKER_DEFAULT_VOLUME                (60)

/* Private values -------------------------------------------------------------*/
static T_UavWidgetSpeakerHandler s_speakerHandler = {0};
static T_UavWidgetSpeakerExHandler s_speakerExHandler = {0};
static T_UavWidgetSpeakerState s_speakerState = {UAV_WIDGET_SPEAKER_STATE_IDEL ,UAV_WIDGET_SPEAKER_WORK_MODE_TTS, UAV_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY, 0};

static T_UAVReturnCode UAV_WidgetSpeakerInit(void);

#include <string>
#include <thread>
#include <sys/time.h>
#include "uav_core.h"
#include "uav_platform.h"
#include "uav_sdk_app_info.h"
#include "uav_fc_subscription.h"
#include "uav_low_speed_data_channel.h"


static void myGimbalAngleMsgCB(void *data)
{
    T_UAVSubscriptionGimbalAngle *angleSt = (T_UAVSubscriptionGimbalAngle*)data;
    LOG_INFO("pitch:{}, roll:{}, yaw:{}", angleSt->x, angleSt->y, angleSt->z);
}
void registThreadFunc(void);

int main(int argc, char *argv[])
{
    T_AUserInfo usrInfo;
    logger_init(std::string(argv[1]));
    if(false == uav_sdk_app_info_init(&usrInfo)) {
        LOG_ERROR("uav_sdk_app_info_init failed");
        return -1;
    }
    UAV_Core_Init(&usrInfo);
    UAV_Core_SetAlias("speaker");
//    UAV_Uart_Init("/dev/pts/20",115200);
    // UAV_Network_Init(LOCALHOST_ETHERNET);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);
    set_payload_type(6);
    UAV_WidgetSpeakerInit();
    LOG_INFO(" speaker init ");

    std::thread registerThread(registThreadFunc);
    registerThread.detach();
    return UAV_Core_ApplicationStart();
}

void registThreadFunc(void)
{
  int ret = 0;
  //wait register success
  if(false == wait_register_ready(100)){
    LOG_ERROR("waitHandShakeRegister failed");
    return;
  }
  //订阅Speaker消息,进行对应的speaker操作
  ret =UAV_LowSpeedDataChannel_Init();
  LOG_INFO(" channel init ret :{}", ret);
  ret = UAV_Widget_RegSpeakerHandler(&s_speakerHandler);
  LOG_INFO(" speaker reg ret :{}", ret);
  ret = UAV_Widget_RegSpeakerExHandler(&s_speakerExHandler);
  LOG_INFO(" speaker exreg ret :{}", ret);
  LOG_INFO("end registThreadFunc");

  // 订阅云台角度信息
  UAV_SubscribeTopic(UAV_SUBSCRIPTION_TOPIC_GIMBAL_ANGLE, 0, myGimbalAngleMsgCB);


}

// static void SetSpeakerState(E_UavWidgetSpeakerState speakerState)
// {
// }

static T_UAVReturnCode GetSpeakerState(T_UavWidgetSpeakerState *speakerState)
{
  *speakerState = s_speakerState;
  LOG_INFO(" speaker State: {} {} {} {}", (int)s_speakerState.state, (int)s_speakerState.workMode,
           (int)s_speakerState.playMode, s_speakerState.volume );
  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetWorkMode(E_UavWidgetSpeakerWorkMode workMode)
{
  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetPlayMode(E_UavWidgetSpeakerPlayMode playMode)
{
  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode StartPlay(void)
{
  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode StopPlay(void)
{
  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SetVolume(uint8_t volume)
{
  s_speakerState.volume = volume;
  LOG_INFO("Set widget speaker volume: {}", volume);
  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

//static T_UAVReturnCode GetVolume(uint8_t *volume)
//{
//  *volume = s_speakerState.volume;
//  LOG_INFO("Get widget speaker volume: %d", *volume);
//  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
//}

static T_UAVReturnCode ReceiveTtsData(E_UavWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size)
{
        return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static T_UAVReturnCode ReceiveAudioData(E_UavWidgetTransmitDataEvent event,
                                        uint32_t offset, uint8_t *buf, uint16_t size)
{
        return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SpeakerControl(T_UavWidgetSpeakerControl *speakerControl)
{
        return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode SpeakerQuery(T_UavWidgetSpeakerQuery *speakerQuery,T_UavWidgetSpeakerQueryRsp *speakerQueryRsp)
{
        return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static T_UAVReturnCode UAV_WidgetSpeakerInit(void)
{
  T_UAVReturnCode returnCode;
//  s_speakerHandler.SetSpeakerState = SetSpeakerState;
  s_speakerHandler.GetSpeakerState = GetSpeakerState;
  s_speakerHandler.SetWorkMode = SetWorkMode;
  s_speakerHandler.StartPlay = StartPlay;
  s_speakerHandler.StopPlay = StopPlay;
  s_speakerHandler.SetPlayMode = SetPlayMode;
  s_speakerHandler.SetVolume = SetVolume;
  // s_speakerHandler.GetVolume = GetVolume;
  s_speakerHandler.ReceiveTtsData = ReceiveTtsData;
  s_speakerHandler.ReceiveVoiceData = ReceiveAudioData;

  s_speakerExHandler.SpeakerControl = SpeakerControl;
  s_speakerExHandler.SpeakerQuery = SpeakerQuery;

  s_speakerState.state = UAV_WIDGET_SPEAKER_STATE_IDEL;
  s_speakerState.workMode = UAV_WIDGET_SPEAKER_WORK_MODE_VOICE;
  s_speakerState.playMode = UAV_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY;



  returnCode = SetVolume(WIDGET_SPEAKER_DEFAULT_VOLUME);
  if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
    LOG_ERROR("Set speaker volume error: 0x%08llX", returnCode);
    return returnCode;
  }

  return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

