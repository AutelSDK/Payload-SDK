#include "application.h"
#include "uav_platform.h"
#include "uav_core.h"
#include "uav_logger.h"
#include "osal.h"

#include "hal_uart.h"


//#include "SEGGER_RTT.h"
//#include "SEGGER_RTT_Conf.h"
//#include "bsp_gpio.h"
//#include "bsp_tim.h"
//#include "bsp_led.h"
//#include "bsp_debug.h"
//#include "bsp.h"
//#include "cdc_data_process.h"
//#include "uav_low_speed_data_channel.h"
#include "pps.h"
#include "apply_high_power.h"
#include "upgApplication.h"
#include "firmwareVersion.h"
#include "test_sdk_config.h"
#include "uav_sdk_app_info.h"
//#include "uav_lightSpeaker.h"
//#include "uav_parachute.h"
static void monitor_task_creat(void);
static bool s_isApplicationStart = false;

static T_UAVReturnCode uavUser_FillInUserInfo(T_UAVUserInfo *userInfo);
static T_UAVReturnCode log_data(const uint8_t *data, uint16_t dataLen) {
    // SEGGER_RTT_Write(0, data, dataLen);
    bsp_debug_send(data,dataLen);
    // usbcdc_write(data, dataLen);
    return 0;
}
T_UAVFirmwareVersion firmwareVersion = 
{
    .majorVersion = USER_FIRMWARE_MAJOR_VERSION,
    .minorVersion = USER_FIRMWARE_MINOR_VERSION,
    .modifyVersion = USER_FIRMWARE_MODIFY_VERSION,
    .debugVersion = USER_FIRMWARE_DEBUG_VERSION,
    .boot_version = USER_BOOT_VERSION,
    .hw_version = USER_HW_VERSION,
};



T_UAVHalUartHandler Uav_HalUartHandler = {
   .baudRate  		= 115200,
   .init           = usbcdc_Init,
   .deInit         = usbcdc_DeInit,
   .write          = usbcdc_write,
   .read           = usbcdc_Read,
   .getStatus      = usbcdc_GetStatus,
   .setBaudRate    = usbcdc_SetBaudRate,
};


//  T_UAVHalUartHandler Uav_HalUartHandler = {
//      .baudRate       = 921600,
//      .init           = uart_Init,
//      .deInit         = uart_DeInit,
//      .write          = uart_write,
//      .read           = uart_Read,
//      .getStatus      = uart_GetStatus,
//      .setBaudRate    = uart_SetBaudRate,
//  };





/**
 * @description: uav user start init
 * @return {*}
 */
void UAV_User_StartTask(void const *args) {
    static uint32_t heap_size = 0;
	T_UAVReturnCode returnCode;
	T_UAVUserInfo userInfo;
    T_UAVOsalHandler Uav_OS_Handler = {
        .TaskCreate         = os_task_create,
		.TaskStart			= os_task_start,
        .TaskDestroy        = os_task_destroy,
        .TaskSleepMs        = os_task_sleep_ms,
        .MutexCreate        = os_mutex_create,
        .MutexDestroy       = os_mutex_destroy,
        .MutexLock          = os_mutex_lock,
        .MutexUnlock        = os_mutex_unlock,
        .SemaphoreCreate    = os_semaphore_create,
        .SemaphoreDestroy   = os_semaphore_destroy,
        .SemaphoreWait      = os_semaphore_wait,
        .SemaphoreTimedWait = os_semaphor_time_wait,
        .SemaphorePost      = os_semaphore_post,
        .GetTimeMs          = os_get_time_ms,
        .GetTimeUs          = os_get_time_us,
        .Malloc             = os_malloc,
        .Free               = os_free,
        .MsToTicks          = os_ms_to_ticks,
        .TaskList           = os_task_list,
        .FreeHeapSize       = os_get_free_heap_size,
    };

    T_UAVLoggerConsole printConsole = {
        .func = log_data,
        .consoleLevel = UAV_LOGGER_CONSOLE_LOG_LEVEL_INFO,
        .isSupportColor = false,
    };
    
    returnCode = UAV_Platform_RegOsalHandler(&Uav_OS_Handler);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return;
    }

    returnCode = UAV_Platform_RegHalUartHandler(&Uav_HalUartHandler);	
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return;
    }

    returnCode = UAV_Logger_AddConsole(&printConsole);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        return;
    }


	returnCode = uavUser_FillInUserInfo(&userInfo);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("fill user info error, please check user info config");
        goto out;
    }

    returnCode=UAV_Core_Init(&userInfo);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("uav core init failed !!!");
        goto out;
    }

    returnCode = UAV_Core_SetAlias("AutelPsdk");
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("set alias failed !!!");
        goto out;
    }

    returnCode = UAV_Core_SetFirmwareVersion(firmwareVersion);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("set firmware version failed !!!");
        goto out;
    }

    // monitor_task_creat();
#if defined CONFIG_MODULE_SAMPLE_CUSTOM_ON

    returnCode = UAV_Core_RegCustom();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("reg custom failed !!!");
        goto out;
    }


#elif defined CONFIG_MODULE_SAMPLE_PARACHUTE_ON
    /** register parachute. */
    returnCode = UAV_Test_ParachuteStartService();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("UAV_RegParachute failed !!!");
        goto out;
    }
    else {
        UAV_LOG_INFO("UAV_RegParachute success!");
    }


#elif defined CONFIG_MODULE_SAMPLE_LIGHTSPEAKER_ON
    /** register lightSpeaker.*/
    returnCode = UAV_Test_LightSpeakerStartService();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("UAV_RegLightSpeaker failed !!!");
        goto out;
    }
    else {
        UAV_LOG_INFO("UAV_RegLightSpeaker success!");
    }
#elif defined CONFIG_MODULE_SAMPLE_THROWER_ON
    returnCode = UAV_Test_ThrowerStartService();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("UAV_RegThrower failed !!!");
        goto out;
    }
    else {
        UAV_LOG_INFO("UAV_RegThrower success!");
    }
#elif defined CONFIG_MODULE_SAMPLE_CAMERA_ON
    returnCode = UAV_Test_CameraStartService();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("camera init failed");
    }
#elif defined CONFIG_MODULE_SAMPLE_GIMBAL_ON
    returnCode = UAV_Test_GimbalStartService();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("gimbal init failed");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_TIME_SYNC_ON
    if (UAV_Test_TimeSyncNTPStartService() != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("psdk time sync init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_PPS_TIME_SYNC_ON
    T_UAVTestTimeSyncHandler testTimeSyncHandler = {
        .PpsSignalResponseInit = UAV_Test_PpsSignalResponseInit,
        .GetNewestPpsTriggerLocalTimeUs = UAV_Test_GetNewestPpsTriggerLocalTimeUs,
    };

    if (UAV_Test_TimeSyncRegHandler(&testTimeSyncHandler) != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("regsiter time sync handler error");
        goto out;
    }
    if (UAV_Test_TimeSyncPpsStartService() != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("psdk time sync init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_POWER_MANAGEMENT_ON
    T_UAVTestApplyHighPowerHandler applyHighPowerHandler = {
        .pinInit = UAV_Test_HighPowerApplyPinInit,
        .pinWrite = UAV_Test_WriteHighPowerApplyPin,
    };

    returnCode = UAV_Test_RegApplyHighPowerHandler(&applyHighPowerHandler);
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("register apply high power handler error");
    }

    returnCode = UAV_Test_PowerManagementStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("power management init error");
	}
#endif

#ifdef CONFIG_MODULE_SAMPLE_DATA_TRANSMISSION_ON
    returnCode = UAV_Test_DataTransmissionStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("data transmission sample init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_FC_SUBSCRIPTION_ON
    returnCode = UAV_Test_FcSubscriptionStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("data subscription sample init error\n");
    }
#endif
	
#ifdef CONFIG_MODULE_SAMPLE_FLIGHT_CONTROL_ON
    returnCode = UAV_Test_flightControlStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("flight control init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_HMS_MANAGER_ON
    returnCode = UAV_Test_HmsManagerStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("hms manager init error");
    }
#endif
	
#ifdef CONFIG_MODULE_SAMPLE_LOGGER_ON
    if (UAV_Test_LoggerStartService() != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("psdk logger init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_UPGRADE_ON
    returnCode = UAV_Test_UpgradeStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("upgrade init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_WAYPOINT_ON
    returnCode = UAV_Test_WaypointStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("waypoint init error");
    }
#endif


#ifdef CONFIG_MODULE_SAMPLE_POSITIONING_ON
    returnCode = UAV_Test_PositioningStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("positioning init error");
    }
#endif


#ifdef CONFIG_MODULE_SAMPLE_GIMBAL_MANAGER_ON
    returnCode = UAV_Test_GimalManagerStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("gimbal manager init error");
    }
#endif

#ifdef CONFIG_MODULE_SAMPLE_CAMERA_MANAGER_ON
    returnCode = UAV_Test_CameraManagerStartService();
    if (returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("camera manager init error");
    }
#endif



    returnCode = UAV_Core_ApplicationStart();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS) {
        UAV_LOG_ERROR("UAV_Core_ApplicationStart failed !!!");
        goto out;
    }
    else {
        UAV_LOG_INFO("UAV_Core_ApplicationStart success!");
    }
    s_isApplicationStart = true;
    while(1)
    {
        Uav_OS_Handler.TaskSleepMs(500);
        // LED_GREEN_TOGGLE();
        // LED_RED_TOGGLE();
#if 0
        char *ucTaskListBuffer = (char *)Uav_OS_Handler.Malloc(1024);
        if(ucTaskListBuffer)
        {
            memset(ucTaskListBuffer, 0x00, 512);
            Uav_OS_Handler.TaskList(ucTaskListBuffer);
            UAV_LOG_INFO("\r\n===========================================================");
            UAV_LOG_INFO("Task List:\r\n%s", ucTaskListBuffer);
            UAV_LOG_INFO("\r\n===========================================================");
            Uav_OS_Handler.Free(ucTaskListBuffer);
        }
        UAV_LOG_INFO("Free Heap Size: %d", Uav_OS_Handler.FreeHeapSize());
#endif
    }
out:
    UAV_LOG_ERROR("UAV_User_StartTask fail !!!!");	
    Uav_OS_Handler.TaskDestroy(NULL);
}

/* Private functions definition-----------------------------------------------*/
static T_UAVReturnCode uavUser_FillInUserInfo(T_UAVUserInfo *userInfo)
{
    
    if(false == uav_sdk_app_info_init(userInfo))
    {
        UAV_LOG_ERROR("uav_sdk_app_info_init failed.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
static void monitor_task(void const *args)
{
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    char *ucTaskListBuffer = nullptr;
    while(1)
    {
        UAV_LOG_INFO("\r\n===========================================================");
        ucTaskListBuffer = (char *)os_handler->Malloc(1024);
        if(ucTaskListBuffer)
        {
            memset(ucTaskListBuffer, 0x00, 1024);
            os_handler->TaskList(ucTaskListBuffer);
            UAV_LOG_INFO("Task List:\r\n%s", ucTaskListBuffer);
            os_handler->Free(ucTaskListBuffer);
        }
        UAV_LOG_INFO("Free Heap Size: %d", os_handler->FreeHeapSize());
        UAV_LOG_INFO("\r\n===========================================================");
        //LED_GREEN_TOGGLE();
        os_handler->TaskSleepMs(500);
    }
}
static void monitor_task_creat(void)
{
    T_UAVOsalHandler *os_handler = UAV_Platform_GetOsalHandler();
    T_UAVTaskHandle monitor_task_handle = NULL;
    if(os_handler == NULL)
    {
        UAV_LOG_ERROR("os_handler is 0.");
        return;
    }
    os_handler->TaskCreate("monitor_task", monitor_task, 1024, NULL, 1, &monitor_task_handle);

}
