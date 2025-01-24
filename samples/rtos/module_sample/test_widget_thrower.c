#include "test_widget_thrower.h"
#include "uav_core.h"
#include "uav_throwerManager.h"
#include "uav_error.h"
#include "uav_logger.h"
#include "bsp_tim.h"

T_UAVThrower Uav_thrower =
{
    .type=E_UAV_THROWER_TYPE_MODELX,
	.throwAct=ThrowerAct,
	.getStatus=getThrowerStatus,

};
T_UAVReturnCode UAV_Test_ThrowerStartService(void)
{
    T_UAVReturnCode uavStat;
    uavStat = UAV_RegThrower(&Uav_thrower);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}