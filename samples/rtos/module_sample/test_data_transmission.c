#include "test_data_transmission.h"
#include "uav_low_speed_data_channel.h"
#include "uav_core.h"
#include "uav_error.h"
#include "uav_logger.h"


/* Private constants ---------------------------------------------------------*/
#define DATA_TRANSMISSION_TASK_FREQ         (1)
#define DATA_TRANSMISSION_TASK_STACK_SIZE   (8*256)
#define DATA_TRANSMISSION_TASK_PRIORITY     (1)

/* Private types -------------------------------------------------------------*/

/* Private functions declaration ---------------------------------------------*/
static void *UserDataTransmission_Task(void *arg);
static T_UAVReturnCode ReceiveDataFromMobile(int32_t channelID, int data_type,const uint8_t *data, uint16_t len);

/* Private variables ---------------------------------------------------------*/
static T_UAVTaskHandle s_userDataTransmissionThread;

/* Exported functions definition ---------------------------------------------*/
T_UAVReturnCode UAV_Test_DataTransmissionStartService(void)
{
    T_UAVReturnCode uavStat;
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    E_UAVChannelAddress channelAddress;
    uint16_t port;
    UAV_LOG_INFO("============data_transmission_routine============");
    uavStat = UAV_LowSpeedDataChannel_Init();
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("init data transmission module error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }


    uavStat = UAV_LowSpeedDataChannel_RegRecvDataCallback(ReceiveDataFromMobile);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("register receive data from mobile error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    if (os_handler->TaskCreate("user_transmission_task", 
                                UserDataTransmission_Task,
                                DATA_TRANSMISSION_TASK_STACK_SIZE, 
                                NULL, 
                                DATA_TRANSMISSION_TASK_PRIORITY,
                                &s_userDataTransmissionThread) !=
        UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("user data transmission task create error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode UAV_Test_DataTransmissionStopService(void)
{
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    T_UAVReturnCode returnCode;

    if (os_handler->TaskDestroy(s_userDataTransmissionThread) != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("user data transmission task destroy error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    returnCode = UAV_LowSpeedDataChannel_DeInit();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("deinit data transmission module error.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/
#ifndef __CC_ARM
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-noreturn"
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

static void *UserDataTransmission_Task(void *arg)
{
    T_UAVReturnCode uavStat;
    const uint8_t dataToBeSent[] = "UAV Data Transmission Test Data...";
    T_UAVDataChannelState state = {0};
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    E_UAVChannelAddress channelAddress;


    while (1) {
        os_handler->TaskSleepMs(1000 / DATA_TRANSMISSION_TASK_FREQ);

        channelAddress = UAV_CHANNEL_ADDRESS_MASTER_RC_APP;
        uavStat = UAV_LowSpeedDataChannel_SendData(channelAddress, 101,dataToBeSent, sizeof(dataToBeSent));
        if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
            UAV_LOG_ERROR("send data to mobile error.");
        else 
            UAV_LOG_INFO("send data: %s.", dataToBeSent);
        

        uavStat = UAV_LowSpeedDataChannel_GetSendDataState(channelAddress, &state);
        if (uavStat == UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
            UAV_LOG_DEBUG(
                "send to mobile state: realtimeBandwidthBeforeFlowController: %d, realtimeBandwidthAfterFlowController: %d, busyState: %d.",
                state.realtimeBandwidthBeforeFlowController, state.realtimeBandwidthAfterFlowController,
                state.busyState);
        } else {
            UAV_LOG_ERROR("get send to mobile channel state error.");
        }

    }
}

#ifndef __CC_ARM
#pragma GCC diagnostic pop
#endif

static T_UAVReturnCode ReceiveDataFromMobile(int32_t channelID, int data_type,const uint8_t *data, uint16_t len)
{
    char *printData = NULL;
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();

    printData = os_handler->Malloc(len + 1);
    if (printData == NULL) {
        UAV_LOG_ERROR("malloc memory for printData fail.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_MEMORY_ALLOC_FAILED;
    }

    strncpy(printData, (const char *) data, len);
    printData[len] = '\0';
    UAV_LOG_INFO("receive data from mobile: %s, len:%d.", printData, len);

    os_handler->Free(printData);

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}


/****************** (C) COPYRIGHT AUTEL Innovations *****END OF FILE****/
