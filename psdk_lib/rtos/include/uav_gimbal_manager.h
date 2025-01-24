/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UAV_GIMBAL_MANAGER_HPP__
#define __UAV_GIMBAL_MANAGER_HPP__

/* Includes ------------------------------------------------------------------*/
#include "uav_platform.h"
#include "uav_gimbal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Gimbal manager rotation command property.
 */
typedef struct {
    E_UAVGimbalRotationMode rotationMode; /*!< Rotation gimbal mode. */
    float pitch; /*!< Pitch angle in degree, unit: deg */
    float roll; /*!< Roll angle in degree, unit: deg */
    float yaw; /*!< Yaw angle in degree, unit: deg */
    int64_t time; /*!< Expect execution time of gimbal rotation, unit: second. */
} T_UAVGimbalManagerRotation;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the gimbal manager module.
 * @note The interface initialization needs to be after UAVCore_Init.
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_Init(void);

/**
 * @brief DeInitialize the gimbal manager module.
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_Deinit(void);

/**
 * @brief Set the work mode of the gimbal.
 
 * @param mode: gimbal work mode, input limit see enum E_UAVGimbalMode
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_SetMode(E_UAVGimbalMode mode);

/**
 * @brief Reset the pitch and yaw of the gimbal.
 
 * @param mode: Reset mode, input limit see enum E_UAVGimbalResetMode
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_Reset(E_UAVGimbalResetMode resetMode);

/**
 * @brief Rotate the angle of the gimbal.
 
 * @param rotation: the rotation parameters to be executed on the target gimbal, including the rotation mode, target
 * angle value and executed time, ref to T_UAVGimbalManagerRotation
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_Rotate(T_UAVGimbalManagerRotation rotation);



/*!
 * @brief Prototype of callback function used to enable or disable extended pitch axis angle range.
 * @details Switching the gimbal limit euler angle of pitch axis to the extended limit angle or the default limit
 * angle.
 
 * @param enabledFlag: flag specified whether enable or disable extended pitch axis angle range.
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_SetPitchRangeExtensionEnabled(bool enabledFlag);

/**
 * @brief Set max speed percentage for gimbal controller.
 
 * @param axis: axis to be set.
 * @param maxSpeedPercentage: max speed value. Recommended calculation formula is "spd = default_max_spd * x / 100",
 * x is default max speed value. Range from 1 to 100.
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_SetControllerMaxSpeedPercentage(E_UAVGimbalAxis axis, uint8_t maxSpeedPercentage);

/**
 * @brief Set smooth factor for gimbal controller, using to smooth control.
 
 * @param axis: axis to be set.
 * @param smoothingFactor: smooth factor. The larger the value, the smaller the acceleration of gimbal. Recommended
 * calculation formula is "acc = 10000 * (0.8 ^ (1 + x)) deg/s^2", x is smooth factor. Range from 0 to 30.
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_SetControllerSmoothFactor(E_UAVGimbalAxis axis, uint8_t smoothingFactor);

/**
 * @brief Restore factory settings of gimbal, including fine tune angle, pitch angle extension enable flag and max
 * speed etc.
 
 * @return Execution result.
 */
T_UAVReturnCode UAV_GimbalManager_RestoreFactorySettings(void);


#ifdef __cplusplus
}
#endif

#endif // __UAV_GIMBAL_MANAGER_HPP__