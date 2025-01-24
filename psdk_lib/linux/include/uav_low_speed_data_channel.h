#ifndef __UAV_LOW_SPEED_DATA_CHANNEL_H__
#define __UAV_LOW_SPEED_DATA_CHANNEL_H__

#include <stdint.h>
#include "uav_core.h"
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif
enum E_UAVChannelID{
    UAV_CHANNEL_ADDRESS_UNKNOW = 0,
    UAV_CHANNEL_ADDRESS_MASTER_RC_APP = 1,
    UAV_CHANNEL_ADDRESS_CLOUD_API = 2,
    UAV_CHANNEL_ADDRESS_SLAVE_RC_APP = 3,
};

typedef void (*UAV_LowSpeedDataChannelRecvDataCallback)(int32_t srcChannelID, int data_type, const uint8_t *data, uint32_t len);

/**
 * @brief Initialize the low speed data channel
 * @note The interface initialization needs to be after UAV_Core_Init.
 * @return 0: success, others: failed
*/
T_UAVReturnCode UAV_LowSpeedDataChannel_Init(void);

/**
 * @brief Deinitialize the low speed data channel
*/
void UAV_LowSpeedDataChannel_DeInit(void);

/**
 * @brief Send custom data to the specified channel
 * @param dstChannelID The destination channel ID
 * @param data_type The type of data
 * @param data The data to be sent
 * @param len The length of the data
 * @return 0: success, others: failed
*/
T_UAVReturnCode UAV_LowSpeedDataChannel_SendData(int32_t dstChannelID, int data_type, const uint8_t *data, uint32_t len);

/**
 * @brief Register callback function used to receive data from selected channel
 * @param callback The callback function
 * @return 0: success, others: failed
*/
T_UAVReturnCode UAV_LowSpeedDataChannel_RegRecvDataCallback(UAV_LowSpeedDataChannelRecvDataCallback callback);

#ifdef __cplusplus
}
#endif

#endif // __UAV_LOW_SPEED_DATA_CHANNEL_H__


