#ifndef UAV_SDK_APP_INFO_H
#define UAV_SDK_APP_INFO_H
#include "uav_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USER_APP_NAME           "PL_TEST"
#define USER_APP_ID             "12345678"
#define USER_APP_KEY            "71014379BF0E03104ACAE0EBD90E5B3B248E1DEA0185D73BDFB2DEF867E0D169"
#define USER_APP_LICENSE        "loNZt6reyhyFIgEwKGF6p52NWyx1k9ZcuAxZ7rQtXnYvKkLfO37MNn6ye308/SPeRXZKR9nB5/kl9na9L/K3sSHMp/zhzrtED/tRFa5UvXDrOeeXs1E/UHuwQUpUH7PCR7wJocF8714/Lya+9aZhNC4unZ6vV0tga2kXfx0xyHM="
#define USER_DEVELOPER_ACCOUNT  "your_account"
#define USER_BAUDRATE_RATE      "460800"

inline bool uav_sdk_app_info_init(T_AUserInfo *usrInfo) {
    if( strlen(USER_APP_NAME) >= sizeof(usrInfo->appName) ||
        strlen(USER_APP_ID) >= sizeof(usrInfo->appId) ||
        strlen(USER_APP_KEY) >= sizeof(usrInfo->appKey) ||
        strlen(USER_APP_LICENSE) >= sizeof(usrInfo->appLicense) ||
        strlen(USER_DEVELOPER_ACCOUNT) >= sizeof(usrInfo->developerAccount) ||
        strlen(USER_BAUDRATE_RATE) >= sizeof(usrInfo->baudRate) ) {
        return false;
    }

    strcpy(usrInfo->appName, USER_APP_NAME);
    strcpy(usrInfo->appId, USER_APP_ID);
    strcpy(usrInfo->appKey, USER_APP_KEY);
    strcpy(usrInfo->appLicense, USER_APP_LICENSE);
    strcpy(usrInfo->developerAccount, USER_DEVELOPER_ACCOUNT);
    strcpy(usrInfo->baudRate, USER_BAUDRATE_RATE);
    return true;
}

#ifdef __cplusplus
}
#endif

#endif // UAV_SDK_APP_INFO_H
