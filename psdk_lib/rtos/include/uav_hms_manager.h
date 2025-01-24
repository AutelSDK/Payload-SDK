#ifndef __UAV_HMS_MANAGER_HPP__
#define __UAV_HMS_MANAGER_HPP__
#include "uav_platform.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef struct {
    int errorCode;
    int componentIndex;
    int errorLevel;
} T_UAVHmsInfo;

typedef struct {
    T_UAVHmsInfo hmsInfo[16];
    int hmsInfoNum;
} T_UAVHmsInfoTable;

typedef void (*UavHmsInfoCallback)(T_UAVHmsInfoTable hmsInfoTable);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise hms manager module, and user should call this function
 * before using hms manager features.
 * @return Execution result.
 */
T_UAVReturnCode UAV_HmsManager_Init(void);

/**
 * @brief DeInitialize hms manager module.
 * @return Execution result.
 */
T_UAVReturnCode UAV_HmsManager_DeInit(void);

/**
 * @brief Register callback to get hms info.
 * @note: Data is pushed with a frequency of 1Hz.
 * @param callback: see reference of UavHmsInfoCallback.
 * @return Execution result.
 */
T_UAVReturnCode UAV_HmsManager_RegHmsInfoCallback(UavHmsInfoCallback callback);

/**
 * @brief report payload self-check result.
 * @param status: 0: normal, !0: abnormal.
 * @param description: description of abnormal status.
 * @return Execution result.
 */
T_UAVReturnCode UAV_HmsManager_PayloadSelfCheckResult(int status, const char *description);

#ifdef __cplusplus
}
#endif

#endif // __UAV_HMS_MANAGER_HPP__