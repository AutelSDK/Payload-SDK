#ifndef __BSP_UART4_DMA_H__
#define __BSP_UART4_DMA_H__

#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TxDmaBufDef(num, size) uint8_t u8Tx##num##DmaBuf[size]; 
#define RxDmaBufDef(num, size) uint8_t u8Rx##num##DmaBuf[size];
#define TxRingBufDef(num, size) uint8_t uart##num##_tx_ring_buf[size]  __attribute__((aligned(32)));
#define RxRingBufDef(num, size) uint8_t uart##num##_rx_ring_buf[size]  __attribute__((aligned(32)));
#define BspDevDef(num) bsp_dev_t uart##num##_dev = {0};
#define ReadWriteVerDef(num) uint32_t Rx##num##DmaFront = 0, Rx##num##DmaRear = 0; bool enTx##num##FrameEnd = false;

#define LL_PERIPH_SEL                   (LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU | \
                                         LL_PERIPH_EFM | LL_PERIPH_SRAM)

#define USING_UART4

#ifdef USING_UART4
#define UART4_RX_PORT                   (GPIO_PORT_B)   /* PB4: USART4_RX */
#define UART4_RX_PIN                    (GPIO_PIN_04)
#define UART4_RX_GPIO_FUNC              (GPIO_FUNC_37)

#define UART4_TX_PORT                   (GPIO_PORT_B)   /* PB3: USART4_TX */
#define UART4_TX_PIN                    (GPIO_PIN_03)
#define UART4_TX_GPIO_FUNC              (GPIO_FUNC_36)

/* USART unit definition */
#define UART4_UNIT                      (CM_USART4)
#define UART4_FCG_ENABLE()              (FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USART4, ENABLE))

/* USART timeout bits definition */
#define UART4_TIMEOUT_BITS              (30U)

/* USART interrupt definition */
#define UART4_TX_CPLT_IRQn              (INT010_IRQn)
#define UART4_TX_CPLT_INT_SRC           (INT_SRC_USART4_TCI)

#define UART4_RX_ERR_IRQn               (INT011_IRQn)
#define UART4_RX_ERR_INT_SRC            (INT_SRC_USART4_EI)

#define UART4_RX_TIMEOUT_IRQn           (INT012_IRQn)
#define UART4_RX_TIMEOUT_INT_SRC        (INT_SRC_USART4_RTO)

#define UART4_RX_DMA_TC_IRQn            (INT013_IRQn)
#define UART4_RX_DMA_TC_INT_SRC         (INT_SRC_DMA1_TC1)

#define TX4_DMA_UNIT                     (CM_DMA2)
#define TX4_DMA_CH                       (DMA_CH1)
#define TX4_DMA_FCG_ENABLE()             (FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA2, ENABLE))
#define TX4_DMA_TRIG_SEL                 (AOS_DMA2_1)
#define TX4_DMA_TRIG_EVT_SRC             (EVT_SRC_USART4_TI)
#define TX4_DMA_TC_INT                   (DMA_INT_TC_CH1)
#define TX4_DMA_TC_FLAG                  (DMA_FLAG_TC_CH1)
#define TX4_DMA_TC_IRQn                  (INT013_IRQn)
#define TX4_DMA_TC_INT_SRC               (INT_SRC_DMA2_TC1)

#define RX4_DMA_UNIT                     (CM_DMA1)
#define RX4_DMA_CH                       (DMA_CH1)
#define RX4_DMA_FCG_ENABLE()             (FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA1, ENABLE))
#define RX4_DMA_TRIG_SEL                 (AOS_DMA1_1)
#define RX4_DMA_TRIG_EVT_SRC             (EVT_SRC_USART4_RI)
#define RX4_DMA_RECONF_TRIG_SEL          (AOS_DMA_RC)
#define RX4_DMA_RECONF_TRIG_EVT_SRC      (EVT_SRC_AOS_STRG)
#define RX4_DMA_RC_EVT_SRC               (EVT_SRC_DMA1_TC1)
#define RX4_DMA_TC_FLAG                  (DMA_FLAG_TC_CH1)
#define RX4_DMA_TC_INT                   (DMA_INT_TC_CH1)

void bsp_uart4_init(uint32_t baud);
void uart4_dma_tx(void *buf, uint16_t len);
uint16_t uart4_read(uint8_t *buf, uint16_t len);
void uart4_set_baudrate(uint32_t baud);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART4_DMA_H__ */
