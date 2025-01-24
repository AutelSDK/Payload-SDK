#include "test_widget_parachute.h"
#include "uav_core.h"
#include "uav_parachute.h"
#include "uav_error.h"
#include "uav_logger.h"

void parachute_open(void)
{
    UAV_LOG_INFO("parachute open");
}

T_UAVParachute Uav_parachute =
{
    .open  = parachute_open,
};



T_UAVReturnCode UAV_Test_ParachuteStartService(void)
{
    T_UAVReturnCode uavStat;
    uavStat = UAV_RegParachute(&Uav_parachute);
    if (uavStat != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)
    {
        return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}