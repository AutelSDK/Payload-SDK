#ifndef __UAV_WAYPOINT_HPP__
#define __UAV_WAYPOINT_HPP__
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UAV_WAYPOINT_ACTION_START = 0, /*!< waypoint mission start action */
    UAV_WAYPOINT_ACTION_STOP,      /*!< waypoint mission stop action */
    UAV_WAYPOINT_ACTION_PAUSE,     /*!< waypoint mission pause action */
    UAV_WAYPOINT_ACTION_RESUME,    /*!< waypoint mission resume action */
}E_UAV_WAYPOINT_ACTION;

typedef enum {
    UAV_WAYPOINT_MISSION_STATE_IDEL = 0, /*!< waypoint mission idel state */
    UAV_WAYPOINT_MISSION_STATE_PREPARE = 16, /*!< waypoint mission prepare state */
    UAV_WAYPOINT_MISSION_STATE_TRANS_MISSION = 32, /*!< waypoint mission transition state */
    UAV_WAYPOINT_MISSION_STATE_MISION = 48, /*!< waypoint mission state */
    UAV_WAYPOINT_MISSION_STATE_BREAK = 64, /*!< waypoint mission break state */
    UAV_WAYPOINT_MISSION_STATE_RESUME = 80, /*!< waypoint mission resume state */
    UAV_WAYPOINT_MISSION_RETURN_FIRSTPOINT = 98, /*!< waypoint mission return first point state */
}E_UAV_WAYPOINT_MISSION_STATE;

typedef enum {
    UAV_WAYPOINT_ACTION_STATE_IDEL = 0, /*!< waypoint action idel state */
    UAV_WAYPOINT_ACTION_STATE_RUNNING = 1, /*!< waypoint action running state */
    UAV_WAYPOINT_ACTION_STATE_FINISHED = 5, /*!< waypoint action finished state */
}E_UAV_WAYPOINT_ACTION_STATE;

typedef struct {
    E_UAV_WAYPOINT_MISSION_STATE state; /*!< waypoint mission state */
    uint32_t wayLineId; /*!< waypoint line id */
    uint16_t currentWaypointIndex; /*!< current waypoint index */
}T_UAV_WAYPOINT_MISSION_STATE;

typedef struct {
    E_UAV_WAYPOINT_ACTION_STATE state; /*!< waypoint action state */
    uint32_t wayLineId; /*!< waypoint line id */
    uint16_t currentWaypointIndex; /*!< current waypoint index */
    uint16_t actionGroupId; /*!< action group id */
    uint16_t actionId; /*!< action id */
}T_UAV_WAYPOINT_ACTION_STATE;

typedef int (*UAVWaypointMissionStateCallback_t)(T_UAV_WAYPOINT_MISSION_STATE missionState);


typedef int (*UAVWaypointActionStateCallback_t)(T_UAV_WAYPOINT_ACTION_STATE actionState);

/*
 * @brief Initialise waypoint module, and user should call this function.
 * before using waypoint features.
 */
extern T_UAVReturnCode UAV_Waypoint_Init(void);

/*
 * @brief Deinitialise waypoint module.
 */
extern void UAV_Waypoint_Deinit(void);

/*
 * @brief Upload kmz file by raw data.
 * @note The size of kmz file is very small, you can use this interface to upload quickly.
 */
extern T_UAVReturnCode UAV_Waypoint_Upload_kmz(const uint8_t *data, uint32_t size);

/*
 * @brief Upload kmz file by file path.
 */
extern T_UAVReturnCode UAV_Waypoint_UploadFile_kmz(const char *file);

/*
 * @brief Execute the mission action.
 * @note This action should be called after uploading the kmz file.
 */
extern T_UAVReturnCode UAV_Waypoint_Action(const E_UAV_WAYPOINT_ACTION action, const int32_t msec);

/*
 * @brief Register the mission state callback for waypoint mission.
 * @note  If you want to monitor the state of waypoint mission, this interface should be called before uploading kmz
 * file or executing this mission action.
 */
extern T_UAVReturnCode UAV_Waypoint_RegisterMissionStateCallback(UAVWaypointMissionStateCallback_t callback);


/*
 * @brief Register the action state callback for waypoint action.
 * @note  If you want to monitor the state of waypoint action, this interface should be called before uploading kmz
 * file or executing this mission action.
 */
extern T_UAVReturnCode UAV_Waypoint_RegisterActionStateCallback(UAVWaypointActionStateCallback_t callback);

#ifdef __cplusplus
}
#endif

#endif

