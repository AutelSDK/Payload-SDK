#ifndef __BSP_UART_DMA_H__
#define __BSP_UART_DMA_H__

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
// #define USING_UART1
// #define USING_UART2
#define USING_UART3

#ifdef USING_UART3
#define UART3_RX_PORT                   (GPIO_PORT_B)   /* PB15: USART3_RX */
#define UART3_RX_PIN                    (GPIO_PIN_08)
#define UART3_RX_GPIO_FUNC              (GPIO_FUNC_33)

#define UART3_TX_PORT                   (GPIO_PORT_B)   /* PB14: USART3_TX */
#define UART3_TX_PIN                    (GPIO_PIN_09)
#define UART3_TX_GPIO_FUNC              (GPIO_FUNC_32)

/* USART unit definition */
#define UART3_UNIT                      (CM_USART3)
#define UART3_FCG_ENABLE()              (FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USART3, ENABLE))

/* USART timeout bits definition */
#define UART3_TIMEOUT_BITS              (30U)

/* USART interrupt definition */
#define UART3_TX_CPLT_IRQn              (INT004_IRQn)
#define UART3_TX_CPLT_INT_SRC           (INT_SRC_USART3_TCI)

#define UART3_RX_ERR_IRQn               (INT005_IRQn)
#define UART3_RX_ERR_INT_SRC            (INT_SRC_USART3_EI)

#define UART3_RX_TIMEOUT_IRQn           (INT006_IRQn)
#define UART3_RX_TIMEOUT_INT_SRC        (INT_SRC_USART3_RTO)

#define TX3_DMA_UNIT                     (CM_DMA2)
#define TX3_DMA_CH                       (DMA_CH0)
#define TX3_DMA_FCG_ENABLE()             (FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA2, ENABLE))
#define TX3_DMA_TRIG_SEL                 (AOS_DMA2_0)
#define TX3_DMA_TRIG_EVT_SRC             (EVT_SRC_USART3_TI)
#define TX3_DMA_TC_INT                   (DMA_INT_TC_CH0)
#define TX3_DMA_TC_FLAG                  (DMA_FLAG_TC_CH0)
#define TX3_DMA_TC_IRQn                  (INT003_IRQn)
#define TX3_DMA_TC_INT_SRC               (INT_SRC_DMA2_TC0)

#define RX3_DMA_UNIT                     (CM_DMA1)
#define RX3_DMA_CH                       (DMA_CH0)
#define RX3_DMA_FCG_ENABLE()             (FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA1, ENABLE))
#define RX3_DMA_TRIG_SEL                 (AOS_DMA1_0)
#define RX3_DMA_TRIG_EVT_SRC             (EVT_SRC_USART3_RI)
#define RX3_DMA_RECONF_TRIG_SEL          (AOS_DMA_RC)
#define RX3_DMA_RECONF_TRIG_EVT_SRC      (EVT_SRC_AOS_STRG)
#define RX3_DMA_RC_EVT_SRC               (EVT_SRC_DMA1_TC0)


void bsp_uart3_init(uint32_t baud);
void uart3_dma_tx(void *buf, uint16_t len);
uint16_t uart3_read(uint8_t *buf, uint16_t len);
void uart3_set_baudrate(uint32_t baud);
#endif
/* Timer0 unit & channel definition */
#define RX1_TMR0_UNIT                    (CM_TMR0_1)
#define RX1_TMR0_CH                      (TMR0_CH_A)
#define RX1_TMR0_FCG_ENABLE()            (FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR0_1, ENABLE))

#define RX2_TMR0_UNIT                    (CM_TMR0_1)
#define RX2_TMR0_CH                      (TMR0_CH_B)
#define RX2_TMR0_FCG_ENABLE()            (FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR0_1, ENABLE))

/* Timer0 unit & channel definition */
#define RX3_TMR0_UNIT                    (CM_TMR0_2)
#define RX3_TMR0_CH                      (TMR0_CH_A)
#define RX3_TMR0_FCG_ENABLE()            (FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR0_2, ENABLE))

/* Timer0 unit & channel definition */
#define RX4_TMR0_UNIT                    (CM_TMR0_2)
#define RX4_TMR0_CH                      (TMR0_CH_B)
#define RX4_TMR0_FCG_ENABLE()            (FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR0_2, ENABLE))

#define UART3_RX_IRQ_PRIO           DDL_IRQ_PRIO_04
#define UART4_RX_IRQ_PRIO           DDL_IRQ_PRIO_00
#define UART2_RX_IRQ_PRIO           DDL_IRQ_PRIO_06

#ifdef __cplusplus
}
#endif

#endif // __BSP_UART_DMA_H__
