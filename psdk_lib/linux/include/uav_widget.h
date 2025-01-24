/******************************************************************
 * Author     : tyh (tangyinghao@autelrobotics.cn)
 * CreateTime : 2024/2/19
 * Copyright (c) 2024 Shenzhen Autel Robotics Co., Ltd.
 * File Desc  :
 *******************************************************************/

#ifndef PAYLOADSDK_UAV_WIDGET_H
#define PAYLOADSDK_UAV_WIDGET_H
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Widget types
*/
typedef enum {
    UAV_WIDGET_TYPE_BUTTON = 0, /* button widget type */
    UAV_WIDGET_TYPE_SWITCH = 1, /* switch widget type */
    UAV_WIDGET_TYPE_SCALE  = 2, /* scale widget type */
    UAV_WIDGET_TYPE_LIST   = 3, /* list widget type */
    UAV_WIDGET_TYPE_INT_INPUT_BOX = 4, /* integer input box widget type */
}E_UAVWidgetType;

/**
 * @brief Button widget state
*/
typedef enum {
    UAV_WIDGET_BUTTON_STATE_UP = 0,   /* button widget up state */
    UAV_WIDGET_BUTTON_STATE_DOWN = 1, /* button widget press down state */
}E_UAVWidgetButtonState;

/**
 * @brief Switch widget state
*/
typedef enum {
    UAV_WIDGET_SWITCH_STATE_OFF = 0, /* switch widget off state */
    UAV_WIDGET_SWITCH_STATE_ON = 1,  /* switch widget on state */
}E_UAVWidgetSwitchState;

/**
 * @brief Switch widget speaker work mode.
 */
typedef enum {
  UAV_WIDGET_SPEAKER_WORK_MODE_TTS = 0,
  UAV_WIDGET_SPEAKER_WORK_MODE_VOICE = 1,
} E_UavWidgetSpeakerWorkMode;

/**
 * @brief Switch widget speaker play mode.
 */
typedef enum {
  UAV_WIDGET_SPEAKER_PLAY_MODE_SINGLE_PLAY = 0,
  UAV_WIDGET_SPEAKER_PLAY_MODE_LOOP_PLAYBACK = 1,
} E_UavWidgetSpeakerPlayMode;

/**
 * @brief Switch widget speaker state.
 */
typedef enum {
  UAV_WIDGET_SPEAKER_STATE_IDEL = 0,
  UAV_WIDGET_SPEAKER_STATE_TRANSMITTING = 1,
  UAV_WIDGET_SPEAKER_STATE_PLAYING = 2,
  UAV_WIDGET_SPEAKER_STATE_ERROR = 3,
  UAV_WIDGET_SPEAKER_STATE_IN_TTS_CONVERSION = 4,
} E_UavWidgetSpeakerState;

/**
 * @brief Switch widget transmit data event.
 */
typedef enum {
  UAV_WIDGET_TRANSMIT_DATA_EVENT_START,
  UAV_WIDGET_TRANSMIT_DATA_EVENT_TRANSMIT,
  UAV_WIDGET_TRANSMIT_DATA_EVENT_FINISH,
  UAV_WIDGET_TRANSMIT_DATA_EVENT_ABORT,
} E_UavWidgetTransmitDataEvent;



typedef struct {
  E_UavWidgetSpeakerState state;
  E_UavWidgetSpeakerWorkMode workMode;
  E_UavWidgetSpeakerPlayMode playMode;
  uint8_t volume;
} T_UavWidgetSpeakerState;

typedef struct {
    int32_t moduleID; // 一体机模块id 1 探照灯 2 喊话器
    int32_t action ;
    int32_t value ;
    const char* strValue;
} T_UavWidgetSpeakerControl;

//操作请求接口字段说明：
//|        | moduleid | action           | value          |
//    |--------|----------|------------------|----------------|
//    | 探照灯 | 1        | 1：开灯          | NA             |
//    |        |          | 2：关灯          | NA             |
//    |        |          | 3：亮度调节      | 亮度（0-100） 1档 33 2档 66 3档 99   |
//    |        |          | 4：俯仰角调节    | 俯仰角[-90~30] |
//    |        |          | 5：开云台联动    | NA             |
//    |        |          | 6：关云台联动    | NA             |
//    |        |          | 7：RGB指示灯    | 0 默认（白） 1红 2蓝   |
//    |        |          | 8：爆闪模式      | 0 默认（关） 1灯光爆闪 2警示灯爆闪    |

//    |        |          |                  |                |
//    | 喊话器 | 2        | 1：实时喊话开启  | NA             |
//    |        |          | 2：实时喊话关闭  | NA             |
//    |        |          | 3：实时喊话音量  | 音量（0~100）  |
//    |        |          | 4：打开音频文件上传  | strValue:最大128字节,文件名  intValue:crc32      |
//    |        |          | 5：关闭音频文件上传  | strValue:最大128字节,文件名        |
//    |        |          | 6：播放指定音频文件  | 最大128字节,文件名        |
//    |        |          | 7：音频播放 | NA        |
//    |        |          | 8：音频暂停 | NA        |
//    |        |          | 9：音频停止 | NA        |
//    |        |          | 10：上一曲 | NA        |
//    |        |          | 11：下一曲 | NA        |
//    |        |          | 12：删除指定音频文件 | 最大128字节,文件名       |
//    |        |          | 13：删除指定录音文件 | 最大128字节,文件名       |
//    |        |          | 14: 播放opus音频   |        |
//    |        |          | 15: 设置音色   |     0x01 女声   |
//    |        |          | 16: 设置语速   |     0x01-0x64  |
//    |        |          | 17: 开始传输文本   |     0x00  |
//    |        |          | 18: 结束文本传输并播放  |     0x00  |
//    |        |          | 19: 开启关闭循环 |     0x00 关 0x01 开  |
//    |        |          | 20：打开媒体资源文件上传  | strValue:最大128字节,文件名  intValue:crc32      |
//    |        |          | 21：关闭媒体资源文件上传  | strValue:最大128字节,文件名        |
//    |        |          | 23: 设置循环间隔   |     0x01-0xFF 单位为秒  |
//    |        |          | 24: 设置是否喊话器在线 |   设置是否喊话器在线  0x00 不在线 0x01 在线
typedef struct {
    int32_t moduleID; // 一体机模块id 1 探照灯 2 喊话器
    int32_t query ;
    int32_t pageIndex ;
} T_UavWidgetSpeakerQuery;

typedef struct {
    int32_t value ;
    char strValue[256];
    char pageContent[5][256]; // 例如查询播放列表返回字段是JSON数据{"duration":"100","name":"xxxx",size:"123445"}
    int32_t pageIndex;
    int32_t  pageCnt;
} T_UavWidgetSpeakerQueryRsp;

//查询请求接口字段说明：
//|        | moduleid | query            | 返回值result          |
//    |--------|----------|------------------|-----------------------|
//    | 探照灯 | 1        | 1：开关状态      | 1 开  2 关            |
//    |        |          | 2：亮度档位      | -1 无效， 1、2、3 正常|
//    |        |          | 3：俯仰角        | -1 无效，-90 ~ 30 正常|
//    |        |          | 4：云台联动状态  | 1 开  2 关            |
//    |        |          | 5：RGB指示灯    | 0 默认（白） 1红 2蓝   |
//    |        |          | 6：爆闪模式      | 0 默认（关） 1灯光爆闪 2警示灯爆闪    |
//    |        |          |                  |                       |
//    | 喊话器 | 2        | 1：实时喊话状态  | 1 开  2 关            |
//    |        |          | 2：实时喊话音量  | -1 无效 0 ~ 100 正常  |
//    |        |          | 3：查询歌曲列表  |  数量(0x00 - 0x64),文件列表  |
//    |        |          | 4：OPUS编码结果  |  0 成功 1失败   |
//    |        |          | 5：循环开关  |  0 关 1开   |
//    |        |          | 6：更新列表状态 |   0 成功 1失败    |

typedef struct {
  T_UAVReturnCode (*GetSpeakerState)(T_UavWidgetSpeakerState *speakerState);
  T_UAVReturnCode (*SetWorkMode)(E_UavWidgetSpeakerWorkMode workMode);
  T_UAVReturnCode (*SetPlayMode)(E_UavWidgetSpeakerPlayMode playMode);
  T_UAVReturnCode (*SetVolume)(uint8_t volume);
  T_UAVReturnCode (*GetVolume)(uint8_t *volume);
  T_UAVReturnCode (*StartPlay)(void);
  T_UAVReturnCode (*StopPlay)(void);

  T_UAVReturnCode (*ReceiveTtsData)(E_UavWidgetTransmitDataEvent event,
                                    uint32_t offset, uint8_t *buf, uint16_t size);
  T_UAVReturnCode (*ReceiveVoiceData)(E_UavWidgetTransmitDataEvent event,
                                      uint32_t offset, uint8_t *buf, uint16_t size);
} T_UavWidgetSpeakerHandler;

typedef struct {
    T_UAVReturnCode (*SpeakerControl)(T_UavWidgetSpeakerControl *speakerControl);
    T_UAVReturnCode (*SpeakerQuery)(T_UavWidgetSpeakerQuery *speakerQuery,T_UavWidgetSpeakerQueryRsp *speakerQueryRsp);
} T_UavWidgetSpeakerExHandler;

/* Exported functions --------------------------------------------------------*/
T_UAVReturnCode UAV_Widget_RegSpeakerHandler(T_UavWidgetSpeakerHandler *handler);
T_UAVReturnCode UAV_Widget_RegSpeakerExHandler(T_UavWidgetSpeakerExHandler *handler);

#ifdef __cplusplus
}
#endif



#endif // PAYLOADSDK_UAV_WIDGET_H
