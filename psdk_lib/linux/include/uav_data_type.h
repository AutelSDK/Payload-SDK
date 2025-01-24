#ifndef __UAV_DATA_TYPE_H__
#define __UAV_DATA_TYPE_H__

typedef enum {
    PAYLOAD_DEVICE_DATA_TYPE_UNKNOW = 0,
    /* 1~9999 can be used by custom */
    PAYLOAD_LIGHTSPEAKER_CONTROL = 10001,         //ref LightSpeakerControlMessage
    PAYLOAD_LIGHTSPEAKER_CONTROL_RSP = 10002,     //ref LightSpeakerControlRspMessage
    PAYLOAD_LIGHTSPEAKER_RUNTIME = 10003,         //ref LightSpeakerRunTimeMessage
    PAYLOAD_LIGHTSPEAKER_RUNTIME_RSP = 10004,     //ref LightSpeakerRunTimeRspMessage
    PAYLOAD_LIGHTSPEAKER_QUERY = 10005,           //ref LightSpeakerQueryMessage
    PAYLOAD_LIGHTSPEAKER_QUERY_RSP = 10006,       //ref LightSpeakerQueryRspMessage

    PAYLOAD_THROWER_STATUS = 10101,               //ref ThrowerStatusMessage
    PAYLOAD_THROWER_ACTION = 10102,               //ref ThrowerActionMessage

    PAYLOAD_TRACER_MEASURE_INFO = 10201,          //ref TracerMeasureInfoMessage
    PAYLOAD_TRACER_WORKMODE_CONTROL = 10202,      //ref TracerWorkModeControlMessage
    PAYLOAD_TRACER_WORKMODE_CONTROL_RSP = 10203,  //ref TracerWorkModeControlRspMessage
    PAYLOAD_TRACER_STAGGER_FREQ = 10204,          //ref TracerStaggerFreqMessage
    PAYLOAD_TRACER_SYSTEM_INFO = 10205,           //ref TracerSystemInfoMessage
}E_UAVPayloadDataType;

#endif // !__UAV_DATA_TYPE_H__
