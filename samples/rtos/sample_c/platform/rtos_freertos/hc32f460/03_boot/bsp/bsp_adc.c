#include "bsp_adc.h"


/**
 * @brief  Main function of adc_base project
 * @param  None
 * @retval int32_t return value, if needed
 */
static adc_sample_t samp_data;
static const uint8_t adc_channels[] = {ADC1_IN8_CH, ADC1_IN3_CH, ADC1_IN4_CH, ADC1_IN5_CH, ADC1_IN6_CH};
#define ADC_CHANNEL_NUM 5
#define SERSOR_R 0.15 //感应电阻值
int32_t bsp_adc_init(void)
{
    /* System clock is MRC@8MHz */

    /* MCU Peripheral registers write unprotected. */
    LL_PERIPH_WE(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU);
    /* Configures ADC. */
    AdcClockConfig();
    AdcInitConfig();
    /* MCU Peripheral registers write protected. */
    LL_PERIPH_WP(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU);

    memset((void*)&samp_data, 0, sizeof(samp_data));

    /***************** Configuration end, application start **************/

}




/**
 * @brief  Configures ADC clock.
 * @param  None
 * @retval None
 */
static void AdcClockConfig(void)
{
#if (ADC_CLK_SEL == ADC_CLK_SYS_CLK)
    /*
     * 1. Configures the clock divider of PCLK2 and PCLK4 here or in the function of configuring the system clock.
     *    In this example, the system clock is MRC@8MHz.
     *    PCLK4 is the digital interface clock, and PCLK2 is the analog circuit clock.
     *    Make sure that PCLK2 and PCLK4 meet the following conditions:
     *      PCLK4 : PCLK2 = 1:1, 2:1, 4:1, 8:1, 1:2, 1:4.
     *      PCLK2 is in range [1MHz, 60MHz].
     */
    CLK_SetClockDiv((CLK_BUS_PCLK2 | CLK_BUS_PCLK4), (CLK_PCLK2_DIV8 | CLK_PCLK4_DIV2));

#elif (ADC_CLK_SEL == ADC_CLK_MPLL)
    /*
     * 1. Configures MPLL and the divider of MPLLx(x=P, Q, R).
     *    MPLLx(x=P, Q, R) is used as both the digital interface clock and the analog circuit clock.
     *    MPLLx(x=P, Q, R) must be in range [1MHz, 60MHz] for ADC use.
     *    The input source of MPLL is XTAL(8MHz).
     */
    stc_clock_pll_init_t stcMpllInit;
    stc_clock_xtal_init_t stcXtalInit;

    /* Configures XTAL. MPLL input source is XTAL. */
    (void)CLK_XtalStructInit(&stcXtalInit);
    stcXtalInit.u8State      = CLK_XTAL_ON;
    stcXtalInit.u8Drv        = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8Mode       = CLK_XTAL_MD_OSC;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_499US;
    (void)CLK_XtalInit(&stcXtalInit);

    (void)CLK_PLLStructInit(&stcMpllInit);
    /*
     * MPLLx(x=P, Q, R) = ((PLL_source / PLLM) * PLLN) / PLLx
     * MPLLP = (8 / 1) * 40 /8  = 40MHz
     * MPLLQ = (8 / 1) * 40 /10 = 32MHz
     * MPLLR = (8 / 1) * 40 /16 = 20MHz
     */
    stcMpllInit.u8PLLState = CLK_PLL_ON;
    stcMpllInit.PLLCFGR = 0UL;
    stcMpllInit.PLLCFGR_f.PLLM = (1UL  - 1UL);
    stcMpllInit.PLLCFGR_f.PLLN = (40UL - 1UL);
    stcMpllInit.PLLCFGR_f.PLLP = (8UL  - 1UL);
    stcMpllInit.PLLCFGR_f.PLLQ = (10UL - 1UL);
    stcMpllInit.PLLCFGR_f.PLLR = (16UL - 1UL);
    /* stcMpllInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL; */
    (void)CLK_PLLInit(&stcMpllInit);

#elif (ADC_CLK_SEL == ADC_CLK_UPLL)
    /*
     * 1. Configures UPLL and the divider of UPLLx(x=P, Q, R).
     *    UPLLx(x=P, Q, R) is used as both the digital interface clock and the analog circuit clock.
     *    UPLLx(x=P, Q, R) must be in range [1MHz, 60MHz] for ADC use.
     *    The input source of UPLL is HRC(16MHz).
     */
    stc_clock_pllx_init_t stcUpllInit;

    /* Enable HRC(16MHz) for UPLL. */
    CLK_HrcCmd(ENABLE);

    /* Specify the input source of UPLL. NOTE!!! UPLL and MPLL use the same input source. */
    CLK_SetPLLSrc(CLK_PLL_SRC_HRC);
    /* UPLL configuration. */
    (void)CLK_PLLxStructInit(&stcUpllInit);
    /*
     * UPLLx(x=P, Q, R) = ((PLL_source / PLLM) * PLLN) / PLLx
     * UPLLP = (16 / 2) * 40 / 8  = 40MHz
     * UPLLQ = (16 / 2) * 40 / 10 = 32MHz
     * UPLLR = (16 / 2) * 40 / 16 = 20MHz
     */
    stcUpllInit.u8PLLState = CLK_PLLX_ON;
    stcUpllInit.PLLCFGR = 0UL;
    stcUpllInit.PLLCFGR_f.PLLM = (2UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLN = (40UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLR = (8UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLQ = (10UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLP = (16UL - 1UL);
    (void)CLK_PLLxInit(&stcUpllInit);
#endif
    /* 2. Specifies the clock source of ADC. */
    CLK_SetPeriClockSrc(ADC_CLK);
}

/**
 * @brief  Initializes ADC.
 * @param  None
 * @retval None
 */
static void AdcInitConfig(void)
{
		uint8_t ch_index=0;
		stc_adc_init_t stcAdcInit;
    /* 1. Enable ADC peripheral clock. */
    FCG_Fcg3PeriphClockCmd(ADC_PERIPH_CLK, ENABLE);

    /* 2. Modify the default value depends on the application. Not needed here. */
    (void)ADC_StructInit(&stcAdcInit);

    /* 3. Initializes ADC. */
    (void)ADC_Init(ADC_UNIT, &stcAdcInit);

    /* 4. ADC channel configuration. */
    /* 4.1 Set the ADC pin to analog input mode. */
    AdcSetPinAnalogMode();
    /* 4.2 Enable ADC channels. Call ADC_ChCmd() again to enable more channels if needed. */
    for(int i=0;i<ADC_CHANNEL_NUM;i++)
        ADC_ChCmd(ADC_UNIT, ADC_SEQ, adc_channels[ch_index], ENABLE);

    /* 5. Conversion data average calculation function, if needed.
          Call ADC_ConvDataAverageChCmd() again to enable more average channels if needed. */
    ADC_ConvDataAverageConfig(ADC_UNIT, ADC_AVG_CNT8);
    ADC_ConvDataAverageChCmd(ADC_UNIT, ADC_CH, ENABLE);
}

/**
 * @brief  Set specified ADC pin to analog mode.
 * @param  None
 * @retval None
 */
static void AdcSetPinAnalogMode(void)
{
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(ADC1_IN3_PORT_PIN_CONFIG, &stcGpioInit);
    (void)GPIO_Init(ADC1_IN4_PORT_PIN_CONFIG, &stcGpioInit);
    (void)GPIO_Init(ADC1_IN5_PORT_PIN_CONFIG, &stcGpioInit);
    (void)GPIO_Init(ADC1_IN6_PORT_PIN_CONFIG, &stcGpioInit);
    (void)GPIO_Init(ADC1_IN8_PORT_PIN_CONFIG, &stcGpioInit);
}

/**
 * @brief  Use ADC in polling mode.
 * @param  None
 * @retval None
 */
static void AdcPolling(void)
{
    uint16_t u16AdcValue;
    int32_t iRet = LL_ERR;
    __IO uint32_t u32TimeCount = 0UL;

    /* Can ONLY start sequence A conversion.
       Sequence B needs hardware trigger to start conversion. */
    ADC_Start(ADC_UNIT);
    do {
        if (ADC_GetStatus(ADC_UNIT, ADC_EOC_FLAG) == SET) {
            ADC_ClearStatus(ADC_UNIT, ADC_EOC_FLAG);
            iRet = LL_OK;
            break;
        }
    } while (u32TimeCount++ < ADC_TIMEOUT_VAL);

    if (iRet == LL_OK) {
        /* Get any ADC value of sequence A channel that needed. */
        u16AdcValue = ADC_GetValue(ADC_UNIT, ADC_CH);
//        DDL_Printf("The ADC value of potentiometer is %u, voltage is %u mV\r\n",
 //                  u16AdcValue, ADC_CAL_VOL(u16AdcValue));
    } else {
        ADC_Stop(ADC_UNIT);
//        DDL_Printf("ADC exception.\r\n");
    }
}



static void record_adc_data(uint8_t ch, uint16_t data)
{
    switch(ch) {
    case ADC1_IN8_CH:
        samp_data.vbat = data * 3.3f * (10 + 2) / 2 / 4096 * 1000;//mV
        break;
    case ADC1_IN3_CH:
        samp_data.ctl1_sensor= data * 3.3f / 4096/SERSOR_R* 1000;//mA
        break;
    case ADC1_IN4_CH:
        samp_data.ctl2_sensor= data * 3.3f / 4096/SERSOR_R* 1000;//mA
        break;
    case ADC1_IN5_CH:
        samp_data.ctl3_sensor= data * 3.3f / 4096/SERSOR_R* 1000;//mA
        break;
    case ADC1_IN6_CH:
        samp_data.ctl4_sensor= data * 3.3f / 4096/SERSOR_R* 1000;//mA
        break;
    default :
        break;
    }

}


adc_sample_t* get_adc_raw(void)
{
    return &samp_data;
}


void adc_sample_task(void)
{
    static uint8_t ch_index = 0;
    static uint8_t triged = 0;
    static uint32_t last_ts = 0;
    uint16_t data = 0;
		uint16_t u16AdcValue;

//    if(get_past_ticks(last_ts) < 2) 
//		{
//        return;
//    }
//    last_ts = get_systick();
		delay_ms(2);

    if(!triged) 
    {
        
        ADC_ConvDataAverageConfig(ADC_UNIT, ADC_AVG_CNT8);
        ADC_ConvDataAverageChCmd(ADC_UNIT, adc_channels[ch_index], ENABLE);
        ADC_Start(ADC_UNIT);
        triged = 1;
    } 
    else 
    {

				if (ADC_GetStatus(ADC_UNIT, ADC_EOC_FLAG) == SET) 
				{
						ADC_ClearStatus(ADC_UNIT, ADC_EOC_FLAG);
						u16AdcValue = ADC_GetValue(ADC_UNIT, adc_channels[ch_index]);

						record_adc_data(adc_channels[ch_index], data);

						triged = 0;
						ch_index ++;
						ch_index %= ADC_CHANNEL_NUM;
				}

    }
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
