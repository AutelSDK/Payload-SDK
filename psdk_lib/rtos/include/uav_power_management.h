#ifndef UAV_POWER_MANAGEMENT_H
#define UAV_POWER_MANAGEMENT_H
/* Includes ------------------------------------------------------------------*/
#include "uav_platform.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Pin state.
 */
typedef enum {
    UAV_POWER_MANAGEMENT_PIN_STATE_DISABLE= 0, /*!< Specifies pin is in low level state. */
    UAV_POWER_MANAGEMENT_PIN_STATE_ENABLE = 1  /*!< Specifies pin is in high level state. */
}E_UAVPowerManagementPinState;

/**
 * @brief Prototype of callback function used to set level of high power application pin.
 * @param pinState: level state of pin to be set.
 * @return Execution result.
 */
typedef T_UAVReturnCode (*UAVWriteHighPowerApplyPinCallback)(E_UAVPowerManagementPinState state);

/**
 * @brief Prototype of callback function used to notify payload that the aircraft will power off soon and get state
 * whether the payload is ready to power off or not.
 * @warning User can not execute blocking style operations or functions in a callback function, because that will block UAV
 * root thread, causing problems such as slow system response, payload disconnection or infinite loop.
 * @param powerOffPreparationFlag: pointer to memory space used to store power off preparation flag. True represents
 * that payload has completed all work and is ready to power off. False represents that payload is not ready to power
 * off.
 * @return Execution result.
 */
typedef T_UAVReturnCode (*UAVPowerOffNotificationCallback)(bool *powerOffPreparationFlag);
/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialise power management module, and user should call this function before using power management features.
 */
T_UAVReturnCode UAV_PowerManagement_Init(void);

/**
 * @brief DeInitialise power management module, and user should call this function before using power management features.
 */
T_UAVReturnCode UAV_PowerManagement_DeInit(void);

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
 * @return Execution result.
 */
T_UAVReturnCode UAV_PowerManagement_RegPowerOffNotificationCallback(UAVPowerOffNotificationCallback callback);
#ifdef __cplusplus
}
#endif

#endif // 