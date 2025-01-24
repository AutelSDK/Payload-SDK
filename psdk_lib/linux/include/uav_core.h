#ifndef __UAV_CORE_HPP__
#define __UAV_CORE_HPP__
#include <string>
#include "uav_error.h"
#include "uav_typedef.h"

typedef struct {
    char appName[32]; /*!< 指定 Autel SDK 应用名称。 该信息可通过登录开发者网站 https://developer.autel.com/user/apps/#all 获取。 以“\0”结尾。 */
    char appId[16]; /*!< 指定 Autel SDK 应用 ID。 该信息可通过登录开发者网站 https://developer.Autel.com/user/apps/#all 获取。*/
    char appKey[65]; /*!< 指定 Autel SDK 应用密钥。 该信息可通过登录开发者网站 https://developer.Autel.com/user/apps/#all 获取。 */
    char appLicense[512]; /*!< 指定 Autel SDK 应用证书. 该信息可通过登录开发者网站 https://developer.Autel.com/user/apps/#all 获取。 */
    char developerAccount[64]; /*!< 指定 Autel SDK 开发者账号邮箱。 该信息可通过登录开发者网站 https://developer.Autel.com/user/apps/#all 获取。 开发者账号等相关信息需要能够对应。 以“\0”结尾。 */
    char baudRate[7]; /*!< 指定 Autel SDK 通信串口波特率。*/
} T_AUserInfo;

#ifdef __cplusplus
extern "C" {
#endif

extern T_UAVReturnCode UAV_Core_Init(const T_AUserInfo *userInfo);
extern T_UAVReturnCode UAV_Core_SetAlias(const char *productAlias);
extern T_UAVReturnCode UAV_Core_SetFirmwareVersion(T_UAVFirmwareVersion version);
extern T_UAVReturnCode UAV_Core_ApplicationStart(void);
extern void UAV_Core_Deinit(void);
bool wait_register_ready(uint32_t seconds);

#ifdef __cplusplus
}
#endif

#endif // __UAV_CORE_HPP__
