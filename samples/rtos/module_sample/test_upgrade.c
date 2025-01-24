
#include "test_upgrade.h"
#include "uav_core.h"
#include "uav_upgrade.h"
#include "uav_error.h"
#include "uav_logger.h"
#include "upgApplication.h"


T_UAVUpgrade Uav_Upgrade =
{
    .model                  = "lightSpeaker",
    .packet_size            = 240,
    .upg_time               = 2,

    .upg_task_stack_size    = (2*1024),
    .verify_type            = E_VERIFY_TYPE_CRC32,  //mcu当前只能采用crc32校验

    .UpgStartCallback       = UpgStartCallback, 
    .ReadFileInfoCallback   = ReadFileInfoCallback,
    .ReadFileDataCallback   = ReadFileDataCallback,
    .UpgFinishCallback      = UpgFinishCallback,
    .FileVerifyCallback     = FileVerifyCallback,
};


T_UAVReturnCode UAV_Test_UpgradeStartService(void)
{
    T_UAVReturnCode uavStat;
    uavStat = UAV_RegUpgrade(&Uav_Upgrade);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}