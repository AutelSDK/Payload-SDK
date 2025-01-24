#ifndef __UAV_TIME_SYNC_H__
#define __UAV_TIME_SYNC_H__
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UAV_TIME_SYNC_TYPE_UNKNOW = 0,
    UAV_TIME_SYNC_TYPE_NTP = 1,
    UAV_TIME_SYNC_TYPE_PPS_CLOCK = 2
} E_UAVTimeSyncType;

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Prototype of callback function used to get the newest PPS triggered timestamp.
 * @warning User can not execute blocking style operations or functions in callback function, because that will block
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param localTimeUs: pointer to memory space used to store PPS triggered timestamp.
 * @return Execution result.
 */
typedef T_UAVReturnCode (*UAV_GetNewestPpsTriggerLocalTimeUsCallback)(uint64_t *localTimeUs);

typedef void (*UAV_TimeSyncRecvDataCallback)(E_UAVTimeSyncType type, int64_t timestamp);

extern T_UAVReturnCode UAV_TimeSync_Init(void);
extern void UAV_TimeSync_Deinit(void);
extern T_UAVReturnCode UAV_TimeSync_NTP(int64_t &timestamp);
/**
 * @brief specify the time synchronization type.
 * @param type: time synchronization type.
*/
T_UAVReturnCode UAV_TimeSync_SetType(E_UAVTimeSyncType type);

/**
 * @brief Register callback function used to get the newest timestamp in local time system when PPS rising edge signal
 * is detected.
 * @details PSDK uses the timestamp information to synchronise time of local time system and RTK navigation and
 * positioning system.
 * @param callback: pointer to the callback function.
 * @return Execution result.
 */
T_UAVReturnCode UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback(UAV_GetNewestPpsTriggerLocalTimeUsCallback callback);

/**
 * @brief Data structure of time in aircraft time system.
 */
typedef struct {
    uint16_t year;        /*!< Specifies year. */
    uint8_t month;        /*!< Specifies month. */
    uint8_t day;          /*!< Specifies day. */
    uint8_t hour;         /*!< Specifies hour. */
    uint8_t minute;       /*!< Specifies minute. */
    uint8_t second;       /*!< Specifies second. */
    uint16_t milliseconds; /*!< Specifies milliseconds. */
}T_UAVTimeSyncAircraftTime;

/**
 * @brief Transfer local time to time in aircraft time system.
 * @note Before calling the interface, users must call UAV_TimeSync_RegGetNewestPpsTriggerTimeCallback() to register
 * callback function used to report the latest triggered time of PPS signal.
 * @param localTimeUs: local time, unit: microsecond.
 * @param aircraftTime: pointer to memory space used to store time in aircraft time system.
 * @return Execution result.
 */
T_UAVReturnCode UAV_TimeSync_TransferToAircraftTime(uint64_t localTimeUs, T_UAVTimeSyncAircraftTime *aircraftTime);


#ifdef __cplusplus
}
#endif

#endif // !__UAV_TIME_SYNC_H__
