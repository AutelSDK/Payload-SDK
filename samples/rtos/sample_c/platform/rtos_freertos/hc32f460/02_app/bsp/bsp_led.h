#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32_ll.h"

void bsp_led_init(void);

#define LED_RED_PORT    (GPIO_PORT_H)
#define LED_RED_PIN     (GPIO_PIN_02)

#define LED_GREEN_PORT  (GPIO_PORT_C)
#define LED_GREEN_PIN   (GPIO_PIN_13)

#define LED_RED_ON() GPIO_SetPins(LED_RED_PORT,LED_RED_PIN)
#define LED_RED_OFF() GPIO_ResetPins(LED_RED_PORT,LED_RED_PIN)
#define LED_RED_TOGGLE() GPIO_TogglePins(LED_RED_PORT,LED_RED_PIN)

#define LED_GREEN_ON() GPIO_SetPins(LED_GREEN_PORT,LED_GREEN_PIN)
#define LED_GREEN_OFF() GPIO_ResetPins(LED_GREEN_PORT,LED_GREEN_PIN)
#define LED_GREEN_TOGGLE() GPIO_TogglePins(LED_GREEN_PORT,LED_GREEN_PIN)

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H */
