#ifndef __UAV_LOW_SPEED_DATA_CHANNEL_H__
#define __UAV_LOW_SPEED_DATA_CHANNEL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "uav_platform.h"
#include "uav_typedef.h"

typedef enum {
    UAV_CHANNEL_ADDRESS_UNKNOWN = 0,
    UAV_CHANNEL_ADDRESS_MASTER_RC_APP = 1,
    UAV_CHANNEL_ADDRESS_CLOUD_API = 2,
    UAV_CHANNEL_ADDRESS_SLAVE_RC_APP = 3,
}E_UAVChannelAddress;


/* Exported types ------------------------------------------------------------*/
/**
 * @brief Prototype of callback function used to receive data that come from selected channel address.
 * @warning User can not execute blocking style operations or functions in callback function, because that will block
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param srcChannelID: the channel address of the low speed channel
 * @param data_type: the type of the data
 * @param data: pointer to data.
 * @param len: length of data.
 * @return Execution result.
 */
typedef T_UAVReturnCode (*UavLowSpeedDataChannelRecvDataCallback)(int32_t srcChannelID, int data_type, const uint8_t *data, uint16_t len);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the low speed data channel module.
 * @param uartHandle: pointer to uart handler.
 * @note The interface initialization needs to be after UAV_Core_Init.
 * @return Execution result.
 */
T_UAVReturnCode UAV_LowSpeedDataChannel_Init(void);

/**
 * @brief Deinitialize the low speed data channel module.
 * @param none: 
 * @return Execution result.
 */
T_UAVReturnCode UAV_LowSpeedDataChannel_DeInit(void);

/**
 * @brief Send data to selected channel address end via command channel.
 * @warning If actual bandwidth is below limitation, data can be sent to the endpoint directly. If exceeds the limitation,
 * firstly data will be stored to buffer of the flow controller and be sent to endpoint after a period (an integer multiple of
 * 1s, the exact time depends on bandwidth limitation and buffer size). If the buffer is full, data be will discarded. The
 * capacity of flow controller buffer is 512 bytes.
 * @note Must ensure actual bandwidth is less than bandwidth limitation of corresponding channel or stream, please
 * refer to developer documentation or state of channel/stream for details related to bandwidth limitation. User can
 * get state of "sendDataChannel" command channel via UAV_LowSpeedDataChannel_GetSendDataState() function. If actual
 * bandwidth exceeds the limitation or busy state is set, the user should stop transmitting data or decrease amount of data
 * to be sent.
 * @note Max size of data package sent to selected channel address end on a physical link of command channel is 128.
 * If the length of data to be sent is greater than 128, data to be sent will be divided into multiple packages to send,
 * and the user will also receive multiple data packages on the selected channel address end.
 * @param channelAddress: the channel address of the low speed channel
 * @param type: the type of the data
 * @param data: pointer to data to be sent.
 * @param len: length of data to be sent, unit: byte.
 * @return Execution result.
 */
T_UAVReturnCode UAV_LowSpeedDataChannel_SendData(int32_t channelAddress, int32_t type,const uint8_t *data, uint8_t len);

/**
 * @brief Get data transmission state of "sendToOsdk" command channel. User can use the state as base for controlling data
 * transmission between selected channel address and onboard computer.
 * @param channelAddress: the channel address of the low speed channel
 * @param state: pointer to low speed channel state.
 * @return Execution result.
 */
T_UAVReturnCode UAV_LowSpeedDataChannel_GetSendDataState(int32_t channelAddress, T_UAVDataChannelState *state);

/**
 * @brief Register callback function used to receive data from selected channel address. After registering this callback
 * function, callback function will be called automatically when system receive data from selected channel address.
 * @param channelAddress: the channel address of the low speed channel
 * @param type: the type of the data
 * @param callback: pointer to callback function.
 * @return Execution result.
 */
T_UAVReturnCode UAV_LowSpeedDataChannel_RegRecvDataCallback(UavLowSpeedDataChannelRecvDataCallback callback);

#ifdef __cplusplus
}
#endif

#endif //__UAV_LOW_SPEED_DATA_CHANNEL_H__