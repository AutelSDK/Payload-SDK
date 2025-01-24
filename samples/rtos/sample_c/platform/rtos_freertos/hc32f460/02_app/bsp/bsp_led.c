#include "bsp_led.h"



void bsp_led_init(void)
{
    stc_gpio_init_t stcGpioInit;

	(void)GPIO_StructInit(&stcGpioInit);
	stcGpioInit.u16PinState = PIN_STAT_RST;
	stcGpioInit.u16PinDir = PIN_DIR_OUT;
    (void)GPIO_Init(LED_RED_PORT, LED_RED_PIN, &stcGpioInit);
    (void)GPIO_Init(LED_GREEN_PORT, LED_GREEN_PIN, &stcGpioInit);
    LED_RED_ON();
    LED_GREEN_ON();
}