#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "hc32_ll.h"
#define MDX_THROWER 0
#if MDX_THROWER
	#define  MOS_CTRL1_PORT        (GPIO_PORT_B)
	#define  MOS_CTRL1_PIN         (GPIO_PIN_14)

	#define  MOS_CTRL2_PORT        (GPIO_PORT_B)
	#define  MOS_CTRL2_PIN         (GPIO_PIN_15)

	#define  MOS_CTRL3_PORT        (GPIO_PORT_B)
	#define  MOS_CTRL3_PIN         (GPIO_PIN_13)

	#define  MOS_CTRL4_PORT        (GPIO_PORT_B)
	#define  MOS_CTRL4_PIN         (GPIO_PIN_12)

	#define  IO_DET1_PORT          (GPIO_PORT_B)
	#define  IO_DET1_PIN           (GPIO_PIN_06)
	#define  IO_DET1_INT_PORT       INT_SRC_PORT_EIRQ6
	#define  IO_DET1_IRQn           INT006_IRQn
	#define  IO_DET1_ExtiCh 				EXTINT_CH06

	#define  IO_DET2_PORT          (GPIO_PORT_B)
	#define  IO_DET2_PIN           (GPIO_PIN_07)
	#define  IO_DET2_INT_PORT       INT_SRC_PORT_EIRQ7
	#define  IO_DET2_IRQn           INT007_IRQn
	#define  IO_DET2_ExtiCh 				EXTINT_CH07

	#define  IO_DET3_PORT          (GPIO_PORT_B)
	#define  IO_DET3_PIN           (GPIO_PIN_08)
	#define  IO_DET3_INT_PORT       INT_SRC_PORT_EIRQ8
	#define  IO_DET3_IRQn           INT008_IRQn
	#define  IO_DET3_ExtiCh 				EXTINT_CH08

	#define  IO_DET4_PORT          (GPIO_PORT_B)
	#define  IO_DET4_PIN           (GPIO_PIN_09)
	#define  IO_DET4_INT_PORT       INT_SRC_PORT_EIRQ9
	#define  IO_DET4_IRQn           INT009_IRQn
	#define  IO_DET4_ExtiCh 				EXTINT_CH09

	/* LED0~3 toggle definition */

	#define CTRL1_SET GPIO_SetPins(MOS_CTRL1_PORT,MOS_CTRL1_PIN);
	#define CTRL1_RESET GPIO_ResetPins(MOS_CTRL1_PORT,MOS_CTRL1_PIN);

	#define CTRL2_SET GPIO_SetPins(MOS_CTRL2_PORT,MOS_CTRL2_PIN);
	#define CTRL2_RESET GPIO_ResetPins(MOS_CTRL2_PORT,MOS_CTRL2_PIN);

	#define CTRL3_SET GPIO_SetPins(MOS_CTRL3_PORT,MOS_CTRL3_PIN);
	#define CTRL3_RESET GPIO_ResetPins(MOS_CTRL3_PORT,MOS_CTRL3_PIN);

	#define CTRL4_SET GPIO_SetPins(MOS_CTRL4_PORT,MOS_CTRL4_PIN);
	#define CTRL4_RESET GPIO_ResetPins(MOS_CTRL4_PORT,MOS_CTRL4_PIN);

	void set_gpio(uint8_t ctrl);
	void reset_gpio(uint8_t ctrl);

	void mdxThrowerAct(uint32_t index);
	void getMdxThrowerStatus (uint32_t *index);

	#define MDX_THROWER_NUM 4
	#define MDX_THROWER_REPORT_SUPPORT 0x0f
	#define MDX_THROWER_TYPE_NAME "MdxThrower1"
#else
	#define  DCDC_EN_PORT        (GPIO_PORT_B)
	#define  DCDC_EN_PIN         (GPIO_PIN_10)

	#define  IO_DET1_PORT          (GPIO_PORT_C)
	#define  IO_DET1_PIN           (GPIO_PIN_14)

	#define  IO_DET2_PORT          (GPIO_PORT_C)
	#define  IO_DET2_PIN           (GPIO_PIN_15)

	#define  IO_DET3_PORT          (GPIO_PORT_A)
	#define  IO_DET3_PIN           (GPIO_PIN_00)

	#define  IO_DET4_PORT          (GPIO_PORT_A)
	#define  IO_DET4_PIN           (GPIO_PIN_02)

	#define  IO_DET5_PORT          (GPIO_PORT_A)
	#define  IO_DET5_PIN           (GPIO_PIN_04)

	#define  IO_DET6_PORT          (GPIO_PORT_A)
	#define  IO_DET6_PIN           (GPIO_PIN_05)

	#define  IO_DET7_PORT          (GPIO_PORT_A)
	#define  IO_DET7_PIN           (GPIO_PIN_06)

	#define  IO_DET8_PORT          (GPIO_PORT_B)
	#define  IO_DET8_PIN           (GPIO_PIN_02)
	
	#define MDM_THROWER_NUM 8
	#define MDM_THROWER_REPORT_SUPPORT 0xff
	#define MDM_THROWER_TYPE_NAME "MdmThrower2"

#endif
	void bsp_gpio_init(void);

	uint8_t get_gpio_status(void);
	#define DCDC_EN_SET GPIO_SetPins(DCDC_EN_PORT,DCDC_EN_PIN);
#endif

