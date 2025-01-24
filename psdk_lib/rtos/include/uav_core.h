/*
 * @Author: kevin && R22006/uavrobotics.cn
 * @Date: 2023-10-12 14:56:16
 * @LastEditors: kevin && R22006/uavrobotics.cn
 * @LastEditTime: 2023-10-12 15:07:01
 * @Description: core interface file.
 * Copyright (c) 2023 Uav Robotics. All rights reserved.
 */

#ifndef _UAV_CORE_H_
#define _UAV_CORE_H_

#include "uav_platform.h"
#ifdef __cplusplus
extern "C" {
#endif


/* Exported types ------------------------------------------------------------*/


typedef struct {
    char appName[32]; /*!< Specifies Uav SDK app name. This info can be obtained by logging in to the
                           developer website https://developer.Uav.com/user/apps/#all. End with '\0'. */
    char appId[16]; /*!< Specifies Uav SDK app ID. This info can be obtained by logging in to the
                         developer website https://developer.Uav.com/user/apps/#all. */
    char appKey[65]; /*!< Specifies Uav SDK app key. This info can be obtained by logging in to the
                          developer website https://developer.Uav.com/user/apps/#all. */
    char appLicense[512]; /*!< Specifies Uav SDK app license. This info can be obtained by logging in to the
                               developer website https://developer.Uav.com/user/apps/#all. */
    char developerAccount[64]; /*!< Specifies Uav SDK developer account email. This info can be obtained by
                                    logging in to the developer website https://developer.Uav.com/user/apps/#all.
                                    Developer's account and other related information need to be able to correspond.
                                    End with '\0'. */
} T_UAVUserInfo;

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize the Payload SDK core in blocking mode.
 * @note The call location of this interface requires special attention, The call needs to be completed after the
 * registration of console/OSAL handler functions/HAL handler functions are completed. At the same time, it must be
 * initialized at the beginning of calling other functional module interfaces. You need to fill in the developer
 * information correctly to ensure the initialization is successful. For additional instructions, please refer to the
 * tutorial“PSDK Initialization”.
 * @note This function does not return until the correct aircraft type and PSDK adapter type is obtained. The logic ensures
 * that aircraft and PSDK adapter have been started up normally before PSDK functional module and user's program run.
 * General execution time of this function is 2~4 seconds.
 * @param userInfo: pointer to the PSDK application information.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Core_Init(const T_UAVUserInfo *userInfo);

/**
 * @brief Set an alias that satisfies the condition for UAV application or product.
 * @details Alias will display in UAV Pilot, if exist.
 * @note Still need to pass in correct UAV APP name that is obtained from UAV SDK developer website to UAV_Core_Init()
 * interface. The UAV APP name will be used to bind or verification.
 * @note Alias will be effective after a while, and the max value is 1s.
 * @param productAlias: pointer to product alias string, and alias end with '\0'. The max length of the string is 31. If
 * length of alias string is greater than 31, alias string will be truncated and passed in.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Core_SetAlias(const char *productAlias);

/**
 * @brief Set custom firmware version for UAV application or product.
 * @details Payload firmware version will always display in UAV Pilot payload settings interface.
 * @param version: the custom firmware version to be set.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Core_SetFirmwareVersion(T_UAVFirmwareVersion version);
/**
 * @brief register a custom payload devices for UAV application or product.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Core_RegCustom(void);
/**
 * @brief Notify that the Payload SDK core application starts.
 * @note The call location of this interface requires special attention, The call needs to be completed after all the
 * module initialize and register interfaces.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Core_ApplicationStart(void);

/**
 * @brief DeInitialize the Payload SDK core in blocking mode.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Core_DeInit(void);



#ifdef __cplusplus
}
#endif
#endif
