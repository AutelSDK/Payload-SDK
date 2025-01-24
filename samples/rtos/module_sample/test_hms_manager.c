#include "uav_core.h"
#include "uav_logger.h"
#include "uav_hms_manager.h"

#define UAV_TEST_SELF_CHECK_TASK_STACK_SIZE     (256*8) 
#define UAV_TEST_SELF_CHECK_TASK_PRIORITY       (1) 

static T_UAVTaskHandle selfCheckTaskHandle = nullptr;
int hms_status = 0;
char hms_description[32] = {0};

static void *UAV_Test_HmsSelfCheckTask(void *arg);

static void hmsInfoCallbackFunc(T_UAVHmsInfoTable hmsInfoTable)
{
    for (int i = 0; i < hmsInfoTable.hmsInfoNum; i++)
    {
        UAV_LOG_INFO("index %d: errorCode %d, componentIndex %d, errorLevel %d", i,
            hmsInfoTable.hmsInfo[i].errorCode, hmsInfoTable.hmsInfo[i].componentIndex, hmsInfoTable.hmsInfo[i].errorLevel);
    }
}

T_UAVReturnCode UAV_Test_HmsManagerStartService(void)
{
    T_UAVReturnCode returnCode;
    T_UAVOsalHandler *os_handler = nullptr;
    UAV_LOG_INFO("==========hms_manager_routine==========");
    returnCode = UAV_HmsManager_Init();
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        LRLOG_ERROR("init failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }
    returnCode = UAV_HmsManager_RegHmsInfoCallback(hmsInfoCallbackFunc);
    if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        LRLOG_ERROR("reg cb failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    os_handler = UAV_Platform_GetOsalHandler();
    if(os_handler == nullptr)
    {
        LRLOG_ERROR("os_handler 0");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    os_handler->TaskCreate("SelfCheckTask",
                            UAV_Test_HmsSelfCheckTask,
                           UAV_TEST_SELF_CHECK_TASK_STACK_SIZE,
                           nullptr,
                           UAV_TEST_SELF_CHECK_TASK_PRIORITY,
                           &selfCheckTaskHandle);
    if(selfCheckTaskHandle == nullptr)
    {
        LRLOG_ERROR("task create failed");
        return UAV_ERROR_SYSTEM_MODULE_CODE_SYSTEM_ERROR;
    }

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

static void *UAV_Test_HmsSelfCheckTask(void *arg)
{
    T_UAVReturnCode returnCode;
    T_UAVOsalHandler *os_handler = nullptr;
    os_handler = UAV_Platform_GetOsalHandler();
    strcpy(hms_description, "SelfCheckTask");
    hms_status = 0;
    while(1)
    {
        returnCode = UAV_HmsManager_PayloadSelfCheckResult(hms_status, hms_description);
        if(returnCode != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
        {
            LRLOG_ERROR("self check failed");
        }
        else 
        {
            UAV_LOG_INFO("self check success");
        }
        
        os_handler->TaskSleepMs(1000);
    }
    return nullptr;
}