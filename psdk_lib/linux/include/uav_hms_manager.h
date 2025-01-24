#ifndef __UAV_HMS_MANAGER_HPP__
#define __UAV_HMS_MANAGER_HPP__
#include <string>

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
    T_UAVHmsInfo *hmsInfo;
    int hmsInfoNum;
} T_UAVHmsInfoTable;

typedef void (*UAV_HmsInfoCallback)(T_UAVHmsInfoTable hmsInfoTable);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise hms manager module, and user should call this function
 * before using hms manager features.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_HmsManager_Init(void);

/**
 * @brief DeInitialize hms manager module.
 * @return Execution result.
 */
extern void UAV_HmsManager_DeInit(void);

/**
 * @brief Register callback to get hms info.
 * @note: Data is pushed with a frequency of 1Hz.
 * @param callback: see reference of UAV_HmsInfoCallback.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_HmsManager_RegHmsInfoCallback(UAV_HmsInfoCallback callback);

/**
 * @brief report payload self-check result.
 * @param status: 0: normal, !0: abnormal.
 * @param description: description of abnormal status.
 * @return Execution result.
 */
extern T_UAVReturnCode UAV_HmsManager_PayloadSelfCheckResult(int status, const char *description);

#ifdef __cplusplus
}
#endif

#endif // __UAV_HMS_MANAGER_HPP__