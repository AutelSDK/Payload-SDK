#include "bsp_tim.h"
#include "bsp_gpio.h"
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


//===================================THROW1
#define THROW1_UNIT                       (CM_TMRA_4)
#define THROW1_PERIPH_CLK                 (FCG2_PERIPH_TMRA_4)
#define THROW1_PWM_CH                    (TMRA_CH2)
#define THROW1_PWM_PORT                   (GPIO_PORT_B)
#define THROW1_PWM_PIN                   (GPIO_PIN_07)
#define THROW1_PWM_PIN_FUNC               (GPIO_FUNC_4)

//===================================THROW2
#define THROW2_UNIT                       (CM_TMRA_2)
#define THROW2_PERIPH_CLK                 (FCG2_PERIPH_TMRA_2)
#define THROW2_PWM_CH                    (TMRA_CH4)
#define THROW2_PWM_PORT                   (GPIO_PORT_A)
#define THROW2_PWM_PIN                   (GPIO_PIN_03)
#define THROW2_PWM_PIN_FUNC               (GPIO_FUNC_4)

//PB3 PWM3	TIMA_6_PWM5	FUNC5
//PB4 PWM4	TIMA_6_PWM6	FUNC5
//===================================THROW3
#define THROW3_UNIT                       (CM_TMRA_6)
#define THROW3_PERIPH_CLK                 (FCG2_PERIPH_TMRA_6)
#define THROW3_PWM_CH                    (TMRA_CH5)
#define THROW3_PWM_PORT                   (GPIO_PORT_B)
#define THROW3_PWM_PIN                   (GPIO_PIN_03)
#define THROW3_PWM_PIN_FUNC               (GPIO_FUNC_5)

//===================================THROW4
#define THROW4_UNIT                       (CM_TMRA_6)
#define THROW4_PERIPH_CLK                 (FCG2_PERIPH_TMRA_6)
#define THROW4_PWM_CH                    (TMRA_CH6)
#define THROW4_PWM_PORT                   (GPIO_PORT_B)
#define THROW4_PWM_PIN                   (GPIO_PIN_04)
#define THROW4_PWM_PIN_FUNC               (GPIO_FUNC_5)

//PB15 PWM5	TIMA_1_PWM7	FUNC4
//PB14 PWM6	TIMA_1_PWM6	FUNC4
//===================================THROW5
#define THROW5_UNIT                       (CM_TMRA_1)
#define THROW5_PERIPH_CLK                 (FCG2_PERIPH_TMRA_1)
#define THROW5_PWM_CH                    (TMRA_CH7)
#define THROW5_PWM_PORT                   (GPIO_PORT_B)
#define THROW5_PWM_PIN                   (GPIO_PIN_15)
#define THROW5_PWM_PIN_FUNC               (GPIO_FUNC_4)

//===================================THROW6
#define THROW6_UNIT                       (CM_TMRA_1)
#define THROW6_PERIPH_CLK                 (FCG2_PERIPH_TMRA_1)
#define THROW6_PWM_CH                    (TMRA_CH6)
#define THROW6_PWM_PORT                   (GPIO_PORT_B)
#define THROW6_PWM_PIN                   (GPIO_PIN_14)
#define THROW6_PWM_PIN_FUNC               (GPIO_FUNC_4)

//PB13 PWM7	TIMA_1_PWM5	FUNC4
//PB12 PWM8	TIMA_1_PWM8	FUNC4
//===================================THROW7
#define THROW7_UNIT                       (CM_TMRA_1)
#define THROW7_PERIPH_CLK                 (FCG2_PERIPH_TMRA_1)
#define THROW7_PWM_CH                    (TMRA_CH5)
#define THROW7_PWM_PORT                   (GPIO_PORT_B)
#define THROW7_PWM_PIN                   (GPIO_PIN_13)
#define THROW7_PWM_PIN_FUNC               (GPIO_FUNC_4)

//===================================THROW8
#define THROW8_UNIT                       (CM_TMRA_1)
#define THROW8_PERIPH_CLK                 (FCG2_PERIPH_TMRA_1)
#define THROW8_PWM_CH                    (TMRA_CH8)
#define THROW8_PWM_PORT                   (GPIO_PORT_B)
#define THROW8_PWM_PIN                   (GPIO_PIN_12)
#define THROW8_PWM_PIN_FUNC               (GPIO_FUNC_4)


//==========================================================

#define TMRA_MD                         (TMRA_MD_SAWTOOTH)
#define TMRA_DIR                        (TMRA_DIR_UP)
#define TMRA_CLK_DIV                  TMRA_CLK_DIV1024;//100M/1024=97.65625K,1??1000/97.65625=10.24uS
//#define TMRA_PERIOD_VAL                 (293U - 1U)        //97.65625/293=333HZ
#define TMRA_PERIOD_VAL                 (1953U - 1U)        //97.65625/1953=50HZ

#define PWM_CMP_VAL_THROW1                (OPEN_PEM - 1U)//
#define PWM_CMP_VAL_THROW2                (OPEN_PEM - 1U)
#define PWM_CMP_VAL_THROW3                (OPEN_PEM - 1U)//
#define PWM_CMP_VAL_THROW4                (OPEN_PEM - 1U)
#define PWM_CMP_VAL_THROW5                (OPEN_PEM - 1U)
#define PWM_CMP_VAL_THROW6                (OPEN_PEM - 1U)
#define PWM_CMP_VAL_THROW7                (OPEN_PEM - 1U)
#define PWM_CMP_VAL_THROW8                (OPEN_PEM - 1U)


void bsp_tima_init(void)
{
	/* MCU Peripheral registers write unprotected. */
	LL_PERIPH_WE(LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU);

	stc_tmra_init_t stcTmraInit;
	stc_tmra_pwm_init_t stcPwmInit;
	
	//================================================
	GPIO_SetDebugPort(GPIO_PIN_TRST|GPIO_PIN_TDO,DISABLE);//PB3/PB4 I????TDO,NTRST?????????????????

	/* 1. Enable TimerA peripheral clock. */
	FCG_Fcg2PeriphClockCmd(THROW1_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW2_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW3_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW4_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW5_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW6_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW7_PERIPH_CLK, ENABLE);
	FCG_Fcg2PeriphClockCmd(THROW8_PERIPH_CLK, ENABLE);

	/* 2. Set a default initialization value for stcTmraInit. */
	(void)TMRA_StructInit(&stcTmraInit);

	/* 3. Modifies the initialization values depends on the application. */
	stcTmraInit.sw_count.u16ClockDiv = TMRA_CLK_DIV;
	stcTmraInit.sw_count.u16CountMode = TMRA_MD;
	stcTmraInit.sw_count.u16CountDir  = TMRA_DIR;
	stcTmraInit.u32PeriodValue = TMRA_PERIOD_VAL;//????????J?????16bit
	(void)TMRA_Init(THROW1_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW2_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW3_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW4_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW5_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW6_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW7_UNIT, &stcTmraInit);
	(void)TMRA_Init(THROW8_UNIT, &stcTmraInit);

	/* 4. Set the comparison reference value. */
	(void)TMRA_PWM_StructInit(&stcPwmInit);
	stcPwmInit.u16PeriodMatchPolarity=TMRA_PWM_HIGH;

	GPIO_SetFunc(THROW1_PWM_PORT, THROW1_PWM_PIN, THROW1_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW2_PWM_PORT, THROW2_PWM_PIN, THROW2_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW3_PWM_PORT, THROW3_PWM_PIN, THROW3_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW4_PWM_PORT, THROW4_PWM_PIN, THROW4_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW5_PWM_PORT, THROW5_PWM_PIN, THROW5_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW6_PWM_PORT, THROW6_PWM_PIN, THROW6_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW7_PWM_PORT, THROW7_PWM_PIN, THROW7_PWM_PIN_FUNC);
	GPIO_SetFunc(THROW8_PWM_PORT, THROW8_PWM_PIN, THROW8_PWM_PIN_FUNC);
	
		
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW1;
	(void)TMRA_PWM_Init(THROW1_UNIT, THROW1_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW1_UNIT, THROW1_PWM_CH, ENABLE);
	TMRA_Start(THROW1_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW2;
	(void)TMRA_PWM_Init(THROW2_UNIT, THROW2_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW2_UNIT, THROW2_PWM_CH, ENABLE);
	TMRA_Start(THROW2_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW3;
	(void)TMRA_PWM_Init(THROW3_UNIT, THROW3_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW3_UNIT, THROW3_PWM_CH, ENABLE);
	TMRA_Start(THROW3_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW4;
	(void)TMRA_PWM_Init(THROW4_UNIT, THROW4_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW4_UNIT, THROW4_PWM_CH, ENABLE);
	TMRA_Start(THROW4_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW5;
	(void)TMRA_PWM_Init(THROW5_UNIT, THROW5_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW5_UNIT, THROW5_PWM_CH, ENABLE);
	TMRA_Start(THROW5_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW6;
	(void)TMRA_PWM_Init(THROW6_UNIT, THROW6_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW6_UNIT, THROW6_PWM_CH, ENABLE);
	TMRA_Start(THROW6_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW7;
	(void)TMRA_PWM_Init(THROW7_UNIT, THROW7_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW7_UNIT, THROW7_PWM_CH, ENABLE);
	TMRA_Start(THROW7_UNIT);
	
	stcPwmInit.u32CompareValue = PWM_CMP_VAL_THROW8;
	(void)TMRA_PWM_Init(THROW8_UNIT, THROW8_PWM_CH, &stcPwmInit);	
	TMRA_PWM_OutputCmd(THROW8_UNIT, THROW8_PWM_CH, ENABLE);
	TMRA_Start(THROW8_UNIT);
	
}

void set_throw1_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW1_UNIT, THROW1_PWM_CH,stcPwm);
}

void set_throw2_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW2_UNIT, THROW2_PWM_CH,stcPwm);
}

void set_throw3_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW3_UNIT, THROW3_PWM_CH,stcPwm);
}

void set_throw4_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW4_UNIT, THROW4_PWM_CH,stcPwm);
}

void set_throw5_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW5_UNIT, THROW5_PWM_CH,stcPwm);
}

void set_throw6_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW6_UNIT, THROW6_PWM_CH,stcPwm);
}

void set_throw7_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW7_UNIT, THROW7_PWM_CH,stcPwm);
}

void set_throw8_pwm(uint8_t mode)
{
	uint32_t stcPwm;
	if(mode==THROW_OPEN)
	{		
		stcPwm = OPEN_PEM;		
	}
	else if(mode==THROW_CLOSE)
	{
		stcPwm = CLOSE_PWM;			
	}	
	(void)TMRA_SetCompareValue(THROW8_UNIT, THROW8_PWM_CH,stcPwm);
}

void ThrowerAct(uint32_t index)//
{
	uint8_t ctrl=index&0xff;
	osDelay(200);
	if(ctrl&0x01)
	{
			set_throw1_pwm(THROW_CLOSE);			
	}
	if(ctrl&0x02)
	{
			set_throw2_pwm(THROW_CLOSE);
	}
	if(ctrl&0x04)
	{
			set_throw3_pwm(THROW_CLOSE);
	}
	if(ctrl&0x08)
	{
			set_throw4_pwm(THROW_CLOSE);
	}
	if(ctrl&0x10)
	{
			set_throw5_pwm(THROW_CLOSE);
	}
	if(ctrl&0x20)
	{
			set_throw6_pwm(THROW_CLOSE);
	}
	if(ctrl&0x40)
	{
			set_throw7_pwm(THROW_CLOSE);
	}
	if(ctrl&0x80)
	{
			set_throw8_pwm(THROW_CLOSE);
	}
	osDelay(1500);
	if(ctrl&0x01)
	{
			set_throw1_pwm(THROW_OPEN);			
	}
	if(ctrl&0x02)
	{
			set_throw2_pwm(THROW_OPEN);
	}
	if(ctrl&0x04)
	{
			set_throw3_pwm(THROW_OPEN);
	}
	if(ctrl&0x08)
	{
			set_throw4_pwm(THROW_OPEN);
	}
	if(ctrl&0x10)
	{
			set_throw5_pwm(THROW_OPEN);
	}
	if(ctrl&0x20)
	{
			set_throw6_pwm(THROW_OPEN);
	}
	if(ctrl&0x40)
	{
			set_throw7_pwm(THROW_OPEN);
	}
	if(ctrl&0x80)
	{
			set_throw8_pwm(THROW_OPEN);
	}

}
void getThrowerStatus (uint32_t *index)//
{
	uint8_t status;
	status=get_gpio_status();
	*index=status;
}
