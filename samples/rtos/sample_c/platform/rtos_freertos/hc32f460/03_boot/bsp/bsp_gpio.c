#include "bsp_gpio.h"

void bsp_gpio_init(void)
{
    LL_PERIPH_WE(LL_PERIPH_GPIO);
		stc_gpio_init_t stcGpioInit;
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;

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
}

uint8_t get_gpio_status(void)
{
		uint8_t status=0;
		GPIO_ReadInputPins(IO_DET1_PORT, IO_DET1_PIN)?(status=status|0x01):(status=(status&0xfe));
    GPIO_ReadInputPins(IO_DET2_PORT, IO_DET2_PIN)?(status=status|0x02):(status=(status&0xfd));
    GPIO_ReadInputPins(IO_DET3_PORT, IO_DET3_PIN)?(status=status|0x04):(status=(status&0xfb));
    GPIO_ReadInputPins(IO_DET4_PORT, IO_DET4_PIN)?(status=status|0x08):(status=(status&0xf7));
		return status;
}

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

