#include "bsp_gpio.h"
#include "bsp.h"

void bsp_gpio_init(void)
{
	LL_PERIPH_WE(LL_PERIPH_GPIO);
	stc_gpio_init_t stcGpioInit;
	stc_extint_init_t stcExtIntInit;
	stc_irq_signin_config_t stcIrqSignConfig;
	#if MDX_THROWER
		(void)GPIO_StructInit(&stcGpioInit);
		stcGpioInit.u16PinState = PIN_STAT_RST;
		stcGpioInit.u16PinDir = PIN_DIR_OUT;
		(void)GPIO_Init(MOS_CTRL1_PORT, MOS_CTRL1_PIN, &stcGpioInit);
		(void)GPIO_Init(MOS_CTRL2_PORT, MOS_CTRL2_PIN, &stcGpioInit);
		(void)GPIO_Init(MOS_CTRL3_PORT, MOS_CTRL3_PIN, &stcGpioInit);
		(void)GPIO_Init(MOS_CTRL4_PORT, MOS_CTRL4_PIN, &stcGpioInit);

		/* GPIO config */
		(void)GPIO_StructInit(&stcGpioInit);
		stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
		stcGpioInit.u16PullUp = PIN_PU_ON;
		stcGpioInit.u16PinDir = PIN_DIR_IN;
		
		(void)GPIO_Init(IO_DET1_PORT, IO_DET1_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET2_PORT, IO_DET2_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET3_PORT, IO_DET3_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET4_PORT, IO_DET4_PIN, &stcGpioInit);
	#else
		(void)GPIO_StructInit(&stcGpioInit);
		stcGpioInit.u16PinState = PIN_STAT_RST;
		stcGpioInit.u16PinDir = PIN_DIR_OUT;
		(void)GPIO_Init(DCDC_EN_PORT, DCDC_EN_PIN, &stcGpioInit);//DCDC EN
		DCDC_EN_SET;
		/* GPIO config */
		(void)GPIO_StructInit(&stcGpioInit);
		stcGpioInit.u16ExtInt = PIN_EXTINT_OFF;
		stcGpioInit.u16PullUp = PIN_PU_ON;
		stcGpioInit.u16PinDir = PIN_DIR_IN;
		(void)GPIO_Init(IO_DET1_PORT, IO_DET1_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET2_PORT, IO_DET2_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET3_PORT, IO_DET3_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET4_PORT, IO_DET4_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET5_PORT, IO_DET5_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET6_PORT, IO_DET6_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET7_PORT, IO_DET7_PIN, &stcGpioInit);
		(void)GPIO_Init(IO_DET8_PORT, IO_DET8_PIN, &stcGpioInit);
	#endif
}

uint8_t get_gpio_status(void)
{
	uint8_t status=0;
	#if MDX_THROWER
		GPIO_ReadInputPins(IO_DET1_PORT, IO_DET1_PIN)?(status=status|0x01):(status=(status&0xfe));
		GPIO_ReadInputPins(IO_DET2_PORT, IO_DET2_PIN)?(status=status|0x02):(status=(status&0xfd));
		GPIO_ReadInputPins(IO_DET3_PORT, IO_DET3_PIN)?(status=status|0x04):(status=(status&0xfb));
		GPIO_ReadInputPins(IO_DET4_PORT, IO_DET4_PIN)?(status=status|0x08):(status=(status&0xf7));
	#else
		GPIO_ReadInputPins(IO_DET1_PORT, IO_DET1_PIN)?(status=status|0x01):(status=(status&0xfe));
		GPIO_ReadInputPins(IO_DET2_PORT, IO_DET2_PIN)?(status=status|0x02):(status=(status&0xfd));
		GPIO_ReadInputPins(IO_DET3_PORT, IO_DET3_PIN)?(status=status|0x04):(status=(status&0xfb));
		GPIO_ReadInputPins(IO_DET4_PORT, IO_DET4_PIN)?(status=status|0x08):(status=(status&0xf7));
		GPIO_ReadInputPins(IO_DET5_PORT, IO_DET5_PIN)?(status=status|0x10):(status=(status&0xef));
		GPIO_ReadInputPins(IO_DET6_PORT, IO_DET6_PIN)?(status=status|0x20):(status=(status&0xdf));
		GPIO_ReadInputPins(IO_DET7_PORT, IO_DET7_PIN)?(status=status|0x40):(status=(status&0xbf));
		GPIO_ReadInputPins(IO_DET8_PORT, IO_DET8_PIN)?(status=status|0x80):(status=(status&0x7f));
	#endif
	return status;
}
#if MDX_THROWER
void set_gpio(uint8_t ctrl)
{
	if(ctrl&0x01)
	{
			CTRL1_SET;					
	}
	if(ctrl&0x02)
	{
			CTRL2_SET;			
	}
	if(ctrl&0x04)
	{
			CTRL3_SET;		
	}
	if(ctrl&0x08)
	{
			CTRL4_SET;			
	}		
}

void reset_gpio(uint8_t ctrl)
{
	if(ctrl&0x01)
	{
			CTRL1_RESET;					
	}
	if(ctrl&0x02)
	{
			CTRL2_RESET;			
	}
	if(ctrl&0x04)
	{
			CTRL3_RESET;		
	}
	if(ctrl&0x08)
	{
			CTRL4_RESET;			
	}		
}

void mdxThrowerAct(uint32_t index)//
{
	uint8_t ctrl=index&0xff;
	set_gpio(ctrl);
	delay_ms(1000);	
	reset_gpio(ctrl);	
}
void getMdxThrowerStatus (uint32_t *index)//
{
	uint8_t status;
	status=get_gpio_status();
	*index=status;
}
#endif