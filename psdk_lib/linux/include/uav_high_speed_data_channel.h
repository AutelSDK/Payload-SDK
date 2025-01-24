#ifndef __UAV_HS_DATA_CHANNEL_HPP__
#define __UAV_HS_DATA_CHANNEL_HPP__

#include <stdint.h>
#include "uav_core.h"
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t dataStream;       //数据流的带宽比例，范围0～100。
    uint8_t videoStream;      //视频流的带宽比例，范围0～100。
    uint8_t downloadStream;   //下载流的带宽比例，范围0～100。
} T_UavDataChannelBandwidthProportionOfHighspeedChannel;

/**
 * @brief Set the bandwidth proportion of the high-speed data channel.
 * @note This function is reserved for future use.
*/
T_UAVReturnCode UavHighSpeedDataChannel_SetBandwidthProportion(T_UavDataChannelBandwidthProportionOfHighspeedChannel *pBandwidthProportion);

/**
 * @brief Send data to mobile end via data stream of the data channel.
 * @note This function can be used only in Linux operating system.
 * @param data: pointer to data to be sent.
 * @param len: length of data to be sent via data stream, and it must be less than or equal to 65000, unit: byte.
*/
T_UAVReturnCode UavHighSpeedDataChannel_SendDataStreamData(int data_type, const uint8_t *data, uint16_t len);

/**
 * @brief Get data transmission state of "dataStream" channel. User can use the state as base for controlling data
 * transmission of data stream. This function exists and can be used only in Linux operation system.
 * @param state: pointer to "dataStream" channel state.
 */
T_UAVReturnCode UavHighSpeedDataChannel_GetDataStreamState(T_UAVDataChannelState *state);

#ifdef __cplusplus
}
#endif

#endif // __UAV_HS_DATA_CHANNEL_HPP__


