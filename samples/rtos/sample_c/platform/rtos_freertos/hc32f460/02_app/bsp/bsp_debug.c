#include "bsp_debug.h"
#include "lib_fifo.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "stdint.h"
#include "stdarg.h"
#include "bsp_led.h"
#include "bsp_uart_dma.h"

#define USE_LOG_SEMAPHORE       (0)
#define BSP_UART_TX_BUF_LEN     (2048UL)
#define BSP_UART_RX_BUF_LEN     (2048UL)
#define LOG_TEMP_BUF_LEN        (256UL)
#define DMA_ENABLE              1
static RingBuffer tx_ring, rx_ring;

#if (USE_LOG_SEMAPHORE>0)
SemaphoreHandle_t log_sem = NULL;
#endif
/**
 * @brief  Initialize uart.
 * 
 * @param  None
 * @retval None
*/
void bsp_debug_init(void)
{
#if DMA_ENABLE 
    bsp_uart3_init(912600);
#else

    stc_usart_uart_init_t stcUartInit;
    /** Configure usart rx/tx pin.*/
    GPIO_SetFunc(BSP_UART_TX_PORT, BSP_UART_TX_PIN, USART_TX_GPIO_FUNC);
    GPIO_SetFunc(BSP_UART_RX_PORT, BSP_UART_RX_PIN, USART_RX_GPIO_FUNC);

    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USART3, ENABLE);

    /** Initialize usart.*/
    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockDiv         = USART_CLK_DIV4;
    stcUartInit.u32Baudrate         = 921600;
    stcUartInit.u32OverSampleBit    = USART_OVER_SAMPLE_8BIT;

    if(LL_OK != USART_UART_Init(BSP_UART_UNIT, &stcUartInit, NULL))
    {
        
    }

    USART_FuncCmd(BSP_UART_UNIT,  USART_TX, ENABLE);
    RingBuffer_create(&tx_ring, BSP_UART_TX_BUF_LEN);
    RingBuffer_create(&rx_ring, BSP_UART_RX_BUF_LEN);
#if (USE_LOG_SEMAPHORE>0)
    log_sem = xSemaphoreCreateBinary();
#endif

#endif

}
/**
 * @brief  Deinitialize uart.
 * 
 * @param  None
 * @retval None
*/
void bsp_debug_deinit(void)
{
#if DMA_ENABLE 

#else
    USART_FuncCmd(BSP_UART_UNIT, USART_RX | USART_TX, DISABLE);
    USART_DeInit(BSP_UART_UNIT);
    RingBuffer_destroy(&tx_ring);
#if (USE_LOG_SEMAPHORE>0)
    vSemaphoreDelete(log_sem);
#endif
#endif
}

/**
 * @brief  Send data via uart.
 * 
 * @param  pu8TxBuf: Pointer to data buffer.
 * @param  u32TxLen: Length of data.
 * @retval None
*/
void bsp_debug_send(uint8_t *pu8TxBuf, uint32_t u32TxLen)
{

#if DMA_ENABLE 
    uart3_dma_tx(pu8TxBuf, u32TxLen);
#else
    for(int i=0; i<u32TxLen; i++)
    {
        while(RESET == USART_GetStatus(BSP_UART_UNIT, USART_FLAG_TX_EMPTY))
        {
            ;
        }
        USART_WriteData(BSP_UART_UNIT, pu8TxBuf[i]);
    }
#endif
}

/**
 * @brief  Send data via uart.
 * 
 * @param  ch: Data to be sent.
 * @param  f: Pointer to file.
*/
int fputc(int ch, FILE *f)
{
    bsp_debug_send((uint8_t *)&ch, 1UL);
    return ch;
}


/**
 * @brief  Uart task.
 * 
 * @param  None
 * @retval None
*/
void log_fifo_debug_task(void)
{
    uint16_t len = 0;
    char tx_buf[256] = {0};

    len = RingBuffer_read(&tx_ring, (uint8_t *)tx_buf, sizeof(tx_buf));
    if(len > 0) 
    {
        LED_GREEN_TOGGLE();
        bsp_debug_send((uint8_t *)tx_buf, len);
    }
}

/**
 * @brief  Printf via uart.
 * 
 * @param  fmt: Format string.
 * @param  ...: Variable parameters.
 * @retval None
*/
void m_printf(const char *fmt, ...)
{
    va_list args;
    char buf[LOG_TEMP_BUF_LEN] = {0};
    uint16_t len = 0;

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    if(len > sizeof(buf)) len = sizeof(buf);
    va_end(args);
    if(len > 0) bsp_debug_send((uint8_t *)buf, len);

}
/**
 * @brief  fifo Printf via uart.
 * 
 * @param  fmt: Format string.
 * @param  ...: Variable parameters.
*/
void fifo_printf(const char *fmt, ...)
{
    va_list args;
    char buf[LOG_TEMP_BUF_LEN] = {0};
    uint16_t len = 0;

    va_start(args, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    if(len > sizeof(buf)) len = sizeof(buf);
    va_end(args);

    if(len > 0) 
    {
        RingBuffer_write(&tx_ring, (const uint8_t *)buf, len);

#if (USE_LOG_SEMAPHORE>0)
        if(log_sem != NULL)
            xSemaphoreGive(log_sem);
#endif
    }
}

void log_sem_wait(void)
{

#if (USE_LOG_SEMAPHORE>0)
    if(log_sem != NULL)
        xSemaphoreTake(log_sem, portMAX_DELAY);
#else
    osDelay(10);
#endif
}