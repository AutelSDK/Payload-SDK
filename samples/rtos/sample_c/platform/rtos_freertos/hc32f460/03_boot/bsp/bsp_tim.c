#include "bsp_tim.h"
volatile unsigned int g_cpu_runtime = 0;
/* Peripheral register WE/WP selection */
#define LL_PERIPH_SEL                       (LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU | \
                                             LL_PERIPH_EFM | LL_PERIPH_SRAM)

/* TMR4 unit definition */
#define TMR4_UNIT                           (CM_TMR4_1)
#define TMR4_FCG_ENABLE()                   (FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR4_1, ENABLE))

/* TMR4 count value per second */
#define TMR4_COUNT_VALUE_PER_SECOND(div)    ((TMR4_ClockFreq() / (1UL << (uint32_t)(div))))

/* TMR4 interrupt definition */
#define TMR4_COUNT_PEAK_IRQn                (INT000_IRQn)
#define TMR4_COUNT_PEAK_INT_SRC             (INT_SRC_TMR4_1_OVF)

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static __IO en_functional_state_t m_enToggleLed = DISABLE;

/**
 * @brief  Get TMR4 clock frequency.
 * @param  None
 * @retval TMR4 clock frequency
 */
static uint32_t TMR4_ClockFreq(void)
{
    return CLK_GetBusClockFreq(CLK_BUS_PCLK1);
}

/**
 * @brief  TMR4 Count peak interrupt handler callback.
 * @param  None
 * @retval None
 */
static void TMR4_CountPeak_IrqCallback(void)
{
    m_enToggleLed = ENABLE;
//		RTOS_GetRunTimeBase();
	g_cpu_runtime++;
    TMR4_ClearStatus(TMR4_UNIT, TMR4_FLAG_CNT_PEAK);
}

void bsp_tim4_init(void)//?????
{
		stc_tmr4_init_t stcTmr4Init;
    stc_irq_signin_config_t stcIrqConfig;

    /* MCU Peripheral registers write usnprotected */
    LL_PERIPH_WE(LL_PERIPH_SEL);

    /* Enable TMR4 peripheral clock */
    TMR4_FCG_ENABLE();

    /* Initialize TMR4 Counter */
    stcTmr4Init.u16ClockSrc = TMR4_CLK_SRC_INTERNCLK;
    stcTmr4Init.u16ClockDiv = TMR4_CLK_DIV64;
    stcTmr4Init.u16CountMode = TMR4_MD_TRIANGLE;
    stcTmr4Init.u16PeriodValue = (uint16_t)(TMR4_COUNT_VALUE_PER_SECOND(stcTmr4Init.u16ClockDiv) / \
                                            (4UL * ((uint32_t)TMR4_INT_CNT_MASK15 + 1UL)));
    (void)TMR4_Init(TMR4_UNIT, &stcTmr4Init);

    /* Set TMR4 count interrupt mask times */
    TMR4_SetCountIntMaskTime(TMR4_UNIT, TMR4_INT_CNT_PEAK, TMR4_INT_CNT_MASK15);

    /* Register IRQ handler && configure NVIC. */
    stcIrqConfig.enIRQn = TMR4_COUNT_PEAK_IRQn;
    stcIrqConfig.enIntSrc = TMR4_COUNT_PEAK_INT_SRC;
    stcIrqConfig.pfnCallback = &TMR4_CountPeak_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqConfig);
    NVIC_ClearPendingIRQ(stcIrqConfig.enIRQn);
    NVIC_SetPriority(stcIrqConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(stcIrqConfig.enIRQn);

    /* MCU Peripheral registers write protected */
    LL_PERIPH_WP(LL_PERIPH_SEL);

    /* Enable TMR4 peak interrupt */
    TMR4_IntCmd(TMR4_UNIT, TMR4_INT_CNT_PEAK, ENABLE);

    /* Start TMR4 count. */
    TMR4_Start(TMR4_UNIT);
}


unsigned int RTOS_GetRunTimeBase()
{
    return g_cpu_runtime;
}