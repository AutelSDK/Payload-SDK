#ifndef UAV_POWER_MANAGEMENT_H
#define UAV_POWER_MANAGEMENT_H
/* Includes ------------------------------------------------------------------*/
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Pin state.
 */
typedef enum {
    UAV_POWER_MANAGEMENT_PIN_STATE_DISABLE = 0, /*!< Specifies pin is in low level state. */
    UAV_POWER_MANAGEMENT_PIN_STATE_ENABLE = 1  /*!< Specifies pin is in high level state. */
}E_UAVPowerManagementPinState;

typedef T_UAVReturnCode (*UAVWriteHighPowerApplyPinCallback)(E_UAVPowerManagementPinState state);
typedef T_UAVReturnCode (*UAVPowerOffNotificationCallback)(bool *powerOffPreparationFlag);

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise power management module, and user should call this function before using power management features.
 */
T_UAVReturnCode UAV_PowerManagement_Init(void);

/**
 * @brief DeInitialise power management module, and user should call this function before using power management features.
 */
T_UAVReturnCode UAV_PowerManagement_Deinit(void);

/**
 * @brief Apply high power from aircraft in blocking mode.
 * @details Before applying, user should register callback function used to set level state of high power application
 * pin using  After applying high power.
 */
T_UAVReturnCode UAV_PowerManagement_ApplyHighPowerSync(void);

/*
 * @brief Register callback function used to set level state of high power application pin. Must be called before
 * applying high power.
 * @param callback: pointer to the callback function.
 * @note 待实现
 */
T_UAVReturnCode UAV_PowerManagement_RegWriteHighPowerApplyPinCallback(UAVWriteHighPowerApplyPinCallback callback);

/**
 * @brief Register callback function used to notify payload that the aircraft will power off soon and get state
 * whether the payload is ready to power off or not.
 * @details After registering, the callback function will be called at a fixed frequency when aircraft will power off.
 * User fill in the power off preparation flag, and once the payload is ready to power off, the callback function will not
 * be called. After a specified time, if the payload is not ready to power off, the aircraft will power off immediately. The
 * specified time is 10s.
 * @param callback: pointer to callback function used to notify aircraft power off message and get payload power off
 * preparation flag.
 */
T_UAVReturnCode UAV_PowerManagement_RegPowerOffNotificationCallback(UAVPowerOffNotificationCallback callback);

#ifdef __cplusplus
}
#endif

#endif // 