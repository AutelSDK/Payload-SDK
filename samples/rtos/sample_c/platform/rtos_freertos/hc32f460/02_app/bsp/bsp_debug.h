#ifndef __BSP_DEBUG_H__
#define __BSP_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32_ll.h"

#define LOG_FORMAT(fmt, ...)                "%s__%d__%s\t\t[%d]\t"fmt, __FILE__, __LINE__, __FUNCTION__, SysTick_GetTick(), ##__VA_ARGS__ 
#define LOG_FIFO_DEBUG(fmt, ...)            fifo_printf(LOG_FORMAT(fmt, ##__VA_ARGS__))
#define LOG_DEBUG(fmt, ...)                 m_printf(LOG_FORMAT(fmt, ##__VA_ARGS__))

#define BSP_UART_UNIT       (CM_USART3)
#define BSP_UART_TX_PORT    (GPIO_PORT_B)
#define BSP_UART_TX_PIN     (GPIO_PIN_09)
#define USART_TX_GPIO_FUNC  (GPIO_FUNC_32)

#define BSP_UART_RX_PORT    (GPIO_PORT_B)
#define BSP_UART_RX_PIN     (GPIO_PIN_08)
#define USART_RX_GPIO_FUNC  (GPIO_FUNC_33)
void bsp_debug_init(void);
void bsp_debug_deinit(void);
void bsp_debug_send(uint8_t *pu8TxBuf, uint32_t u32TxLen);
void log_fifo_debug_task(void);
void log_sem_wait(void);

#ifdef __cplusplus
}
#endif


#endif
