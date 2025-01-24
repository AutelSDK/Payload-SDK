/**
 ********************************************************************
 * @file    pps.c
 * @version V2.0.0
 * @date    2019/9/20
 * @brief
 *
 * @copyright (c) 2021 AUTEL. All rights reserved.
 *
 * All information contained herein is, and remains, the property of AUTEL.
 * The intellectual and technical concepts contained herein are proprietary
 * to AUTEL and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of AUTEL.
 *
 * If you receive this source code without AUTEL’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify AUTEL of its removal. AUTEL reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "pps.h"
#include "uav_error.h"
#include "uav_logger.h"

/* Private constants ---------------------------------------------------------*/
#define PPS_PORT                GPIO_PORT_C
#define PPS_PIN                 GPIO_PIN_15
#define PPS_EXTINT_IRQn         INT002_IRQn 
#define PPS_EXTINT_CH           EXTINT_CH15
#define PPS_EXTINT_SRC          INT_SRC_PORT_EIRQ15
#define PPS_EXTINT_IRQn_PRIO    DDL_IRQ_PRIO_DEFAULT

/* Private types -------------------------------------------------------------*/

/* Private values -------------------------------------------------------------*/
static uint64_t s_ppsNewestTriggerLocalTimeMs = 0;

/* Private functions declaration ---------------------------------------------*/

/* Exported functions definition ---------------------------------------------*/
void UAV_Test_PpsIrqHandler(void)
{
    T_UAVOsalHandler *os_handler = nullptr;
    T_UAVReturnCode psdkStat;
    uint32_t timeMs = 0;

    os_handler = UAV_Platform_GetOsalHandler();
    /* EXTI line interrupt detected */
    if (SET == EXTINT_GetExtIntStatus(PPS_EXTINT_CH)) {
        psdkStat =os_handler->GetTimeMs(&timeMs);
        if (psdkStat == 0)
        {
            s_ppsNewestTriggerLocalTimeMs = timeMs;
            UAV_LOG_INFO("local time: %d", s_ppsNewestTriggerLocalTimeMs);
        }
        EXTINT_ClearExtIntStatus(PPS_EXTINT_CH);
    }
    LED_GREEN_TOGGLE();
}

T_UAVReturnCode UAV_Test_GetNewestPpsTriggerLocalTimeUs(uint64_t *localTimeUs)
{ 
    if (localTimeUs == NULL) {
        UAV_LOG_ERROR("input pointer is null.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_INVALID_PARAMETER;
    }

    if (s_ppsNewestTriggerLocalTimeMs == 0) {
        UAV_LOG_WARN("pps have not been triggered.");
        return UAV_ERROR_SYSTEM_MODULE_CODE_BUSY;
    }

    *localTimeUs = (uint64_t) (s_ppsNewestTriggerLocalTimeMs * 1000);
    // UAV_LOG_INFO("pps trigger local time: %llu", *localTimeUs);
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

T_UAVReturnCode UAV_Test_PpsSignalResponseInit(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;
	LL_PERIPH_WE(LL_PERIPH_GPIO);
    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(PPS_PORT, PPS_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_RISING;
    (void)EXTINT_Init(PPS_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = PPS_EXTINT_SRC;
    stcIrqSignConfig.enIRQn   = PPS_EXTINT_IRQn;
    stcIrqSignConfig.pfnCallback = &UAV_Test_PpsIrqHandler;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, PPS_EXTINT_IRQn_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
    UAV_LOG_INFO("pps signal response init success.");
    return UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS;
}

/* Private functions definition-----------------------------------------------*/

/****************** (C) COPYRIGHT AUTEL Innovations *****END OF FILE****/
