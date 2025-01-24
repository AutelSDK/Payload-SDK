#include "apply_high_power.h"
#include "uav_logger.h"
#include "bsp_gpio.h"

#define HIGH_POWER_APPLY_PORT      GPIO_PORT_H
#define HIGH_POWER_APPLY_PIN       GPIO_PIN_02 

T_UAVReturnCode UAV_Test_HighPowerApplyPinInit(void)
{
    // stc_gpio_init_t stcGpioInit;

	// (void)GPIO_StructInit(&stcGpioInit);
	// stcGpioInit.u16PinState = PIN_STAT_RST;
	// stcGpioInit.u16PinDir = PIN_DIR_OUT;
    // (void)GPIO_Init(HIGH_POWER_APPLY_PORT, HIGH_POWER_APPLY_PIN, &stcGpioInit);

    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode UAV_Test_WriteHighPowerApplyPin(E_UAVPowerManagementPinState pinState)
{

    switch (pinState) {
        case UAV_POWER_MANAGEMENT_PIN_STATE_DISABLE:
            GPIO_ResetPins(HIGH_POWER_APPLY_PORT, HIGH_POWER_APPLY_PIN);
            break;
        case UAV_POWER_MANAGEMENT_PIN_STATE_ENABLE:
            GPIO_SetPins(HIGH_POWER_APPLY_PORT, HIGH_POWER_APPLY_PIN);
            break;
        default:
            UAV_LOG_ERROR("pin state unknown: %d.", pinState);
            return UAV_ERROR_SYSTEM_MODULE_CODE_UNKNOWN;
    }


    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}
