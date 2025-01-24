#ifndef __TEST_SDK_CONFIG_H__
#define __TEST_SDK_CONFIG_H__

#include "uav_platform.h"

#include "test_time_sync.h"
#include "test_fc_subscription.h"
#include "test_upgrade.h"
#include "test_hms_manager.h"
#include "test_data_transmission.h"
#include "test_power_management.h"
#include "test_flight_control.h"
#include "test_positioning.h"
#include "test_flight_control.h"
#include "test_camera_manager.h"
#include "test_camera.h"
#include "test_gimbal_manager.h"
#include "test_gimbal.h"
#include "test_waypoint.h"
#include  "test_widget_parachute.h"
#include "test_widget_lightSpeaker.h"
#include "test_widget_thrower.h"
#include "test_logger.h"

#ifdef __cplusplus
extern "C" {
#endif



#define CONFIG_MODULE_SAMPLE_TIME_SYNC_ON
// #define CONFIG_MODULE_SAMPLE_PPS_TIME_SYNC_ON
// #define CONFIG_MODULE_SAMPLE_LOGGER_ON
// #define CONFIG_MODULE_SAMPLE_POWER_MANAGEMENT_ON
// #define CONFIG_MODULE_SAMPLE_FC_SUBSCRIPTION_ON
// #define CONFIG_MODULE_SAMPLE_DATA_TRANSMISSION_ON
// #define CONFIG_MODULE_SAMPLE_HMS_MANAGER_ON
// #define CONFIG_MODULE_SAMPLE_FLIGHT_CONTROL_ON
// #define CONFIG_MODULE_SAMPLE_UPGRADE_ON
// #define CONFIG_MODULE_SAMPLE_POSITIONING_ON
// #define CONFIG_MODULE_SAMPLE_CAMERA_MANAGER_ON
// #define CONFIG_MODULE_SAMPLE_GIMBAL_MANAGER_ON
// #define CONFIG_MODULE_SAMPLE_WAYPOINT_ON

/** payload devices*/
// #define CONFIG_MODULE_SAMPLE_CAMERA_ON
// #define CONFIG_MODULE_SAMPLE_GIMBAL_ON
// #define CONFIG_MODULE_SAMPLE_PARACHUTE_ON
// #define CONFIG_MODULE_SAMPLE_LIGHTSPEAKER_ON
// #define CONFIG_MODULE_SAMPLE_THROWER_ON
#define CONFIG_MODULE_SAMPLE_CUSTOM_ON

#ifdef __cplusplus
}
#endif


#endif
