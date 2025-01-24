#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "hc32_ll.h"

/* The clock source of ADC. */
#define ADC_CLK_SYS_CLK                 (1U)
#define ADC_CLK_MPLL                    (2U)
#define ADC_CLK_UPLL                    (3U)

/*
 * Selects a clock source according to the application requirements.
 * PCLK4 is the clock for digital interface.
 * PCLK2 is the clock for analog circuit.
 * PCLK4 and PCLK2 are synchronous when the clock source is xPLL.
 * PCLK4 : PCLK2 = 1:1, 2:1, 4:1, 8:1, 1:2, 1:4.
 * PCLK2 is in range [1MHz, 60MHz].
 * If the system clock is selected as the ADC clock, macro 'ADC_ADC_CLK' can only be defined as 'CLK_PERIPHCLK_PCLK'.
 * If MPLL is selected as the ADC clock, macro 'ADC_ADC_CLK' can be defined as 'CLK_PERIPHCLK_PLLx'(x=P, Q, R).
 * If UPLL is selected as the ADC clock, macro 'ADC_ADC_CLK' can be defined as 'CLK_PERIPHCLK_UPLLx'(x=P, Q, R).
 */
#define ADC_CLK_SEL                     (ADC_CLK_SYS_CLK)

#if (ADC_CLK_SEL == ADC_CLK_SYS_CLK)
#define ADC_CLK                         (CLK_PERIPHCLK_PCLK)

#elif (ADC_CLK_SEL == ADC_CLK_MPLL)
#define ADC_CLK                         (CLK_PERIPHCLK_PLLQ)

#elif (ADC_CLK_SEL == ADC_CLK_UPLL)
#define ADC_CLK                         (CLK_PERIPHCLK_PLLXP)

#else
#error "The clock source your selected does not exist!!!"
#endif

/* ADC unit instance for this example. */
#define ADC_UNIT                        (CM_ADC1)
#define ADC_PERIPH_CLK                  (FCG3_PERIPH_ADC1)

/* Selects ADC channels that needed. */
#define ADC_CH_POTENTIOMETER            (ADC_CH10)
#define ADC_CH                          (ADC_CH_POTENTIOMETER)
#define ADC_CH_PORT                     (GPIO_PORT_C)
#define ADC_CH_PIN                      (GPIO_PIN_00)

#define ADC1_IN3_PORT_PIN_CONFIG GPIO_PORT_A,GPIO_PIN_03
#define ADC1_IN4_PORT_PIN_CONFIG GPIO_PORT_A,GPIO_PIN_04
#define ADC1_IN5_PORT_PIN_CONFIG GPIO_PORT_A,GPIO_PIN_05
#define ADC1_IN6_PORT_PIN_CONFIG GPIO_PORT_A,GPIO_PIN_06
#define ADC1_IN8_PORT_PIN_CONFIG GPIO_PORT_B,GPIO_PIN_00

#define ADC1_IN3_CH ADC_CH3
#define ADC1_IN4_CH ADC_CH4
#define ADC1_IN5_CH ADC_CH5
#define ADC1_IN6_CH ADC_CH6
#define ADC1_IN8_CH ADC_CH8

typedef struct {
    uint16_t vbat;              //mV
    uint16_t ctl1_sensor;         //mV
    uint16_t ctl2_sensor;            //mV
    uint16_t ctl3_sensor;           
    uint16_t ctl4_sensor;           //mA
		uint16_t NTC_R;   //ntc电阻 K
} adc_sample_t;


/* ADC sequence to be used. */
#define ADC_SEQ                         (ADC_SEQ_A)
/* Flag of conversion end. */
#define ADC_EOC_FLAG                    (ADC_FLAG_EOCA)

/* ADC reference voltage. The voltage of pin VREFH. */
#define ADC_VREF                        (3.3F)

/* ADC accuracy(according to the resolution of ADC). */
#define ADC_ACCURACY                    (1UL << 12U)

/* Calculate the voltage(mV). */
#define ADC_CAL_VOL(adcVal)             (uint16_t)((((float32_t)(adcVal) * ADC_VREF) / ((float32_t)ADC_ACCURACY)) * 1000.F)


#define DOWN_REGISTOR 47   //47K电阻
#define REVERF_VOL 3.266f    //ADC参考电压
#define INPUT_VOL  5.03f    //供电电压
#define ADC_STEP 4096    //ADC分辨率

#define NTC_N20DEC_R  1151   // -20°NTC电阻1151K
#define NTC_0DEC_R  357   // 0°NTC电阻357K
#define NTC_50DEC_R  33   // 50°NTC电阻33K


/* Timeout value. */
#define ADC_TIMEOUT_VAL                 (1000U)
static void AdcClockConfig(void);
static void AdcInitConfig(void);
static void AdcSetPinAnalogMode(void);
void adc_sample_task(void);
adc_sample_t* get_adc_raw(void);
extern adc_sample_t samp_data;
static void record_adc_data(uint8_t ch, uint16_t data);
int32_t bsp_adc_init(void);

#endif