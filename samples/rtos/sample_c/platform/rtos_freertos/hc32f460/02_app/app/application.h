#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @description: uav user start init
 * @return {*}
 */
void UAV_User_StartTask(void const *args);

//#define USER_APP_NAME               "your_app_name"
//#define USER_APP_ID                 "your_app_id"
//#define USER_APP_KEY                "your_app_key"
//#define USER_APP_LICENSE            "your_app_license"
//#define USER_DEVELOPER_ACCOUNT      "your_developer_account"
#define USER_APP_NAME               "uav_app_name"
#define USER_APP_ID                 "uav_app_id"
#define USER_APP_KEY                "uav_app_key"
#define USER_APP_LICENSE            "uav_app_license"
#define USER_DEVELOPER_ACCOUNT      "uav_developer_account"
#define USER_UTIL_MIN(a, b)                                 (((a) < (b)) ? (a) : (b))
#define USER_UTIL_MAX(a, b)                                 (((a) > (b)) ? (a) : (b))
#ifdef __cplusplus
}
#endif
#endif
