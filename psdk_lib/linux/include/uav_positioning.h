#ifndef __UAV_POSITIONING_HPP__
#define __UAV_POSITIONING_HPP__
#include <stdint.h>
#include "uav_typedef.h"
#include "uav_time_sync.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /*! Index of event set in which positioning event is located. If not needed, fill in 0. */
    uint16_t eventIndex;
    /*! Timestamp in aircraft time system when the positioning event occur. Users should transfer time in local time
     * system to time in aircraft time system from time synchronization module. */
    T_UAVTimeSyncAircraftTime timestamp;
}T_UAVPositioningEventInfo;

typedef enum {
    UAV_POSITIONING_PROPERTY_NOT_AVAILABLE  = 0,  /*!< Position solution is not available. */
    UAV_POSITIONING_PROPERTY_FIX_POSITION   = 1,  /*!< Position solution is available. */
}E_UAVPositioningProperty;

typedef struct {
    float longitude;  /*!< Longitude in degrees. */
    float latitude;   /*!< Latitude in degrees. */
    float altitude;   /*!< Altitude in meters. */
}T_UAV_PositioningPositionDeviation;

typedef struct {
    E_UAVPositioningProperty property;
    T_UAVAttitude3d uavAttitude;  /*!< Specifies UAV attitude, unit: degree. */
    T_UAV_PositioningPositionDeviation positionDeviation;  /*!< Specifies position deviation, unit: meter. */
}T_UAVPositioningPositionInfo;

extern T_UAVReturnCode UAV_Positioning_Init(void);
extern void UAV_Positioning_Deinit(void);

extern T_UAVReturnCode UAV_Positioning_GetPositioning_Sync(uint8_t eventCount, T_UAVPositioningEventInfo *eventInfo, T_UAVPositioningPositionInfo *positionInfo);

#ifdef __cplusplus
}
#endif

#endif