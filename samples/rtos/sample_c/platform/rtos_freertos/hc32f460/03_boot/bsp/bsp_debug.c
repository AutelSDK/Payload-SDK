#include "bsp_debug.h"
#include "stdio.h"
#include "stdint.h"
#include "stdarg.h"


#define USE_LOG_SEMAPHORE       (0)
#define LOG_TEMP_BUF_LEN        (256UL)


/**
 * @brief  Initialize uart.
 * 
 * @param  None
 * @retval None
*/
void bsp_debug_init(void)
{
    stc_usart_uart_init_t stcUartInit;
    /** Configure usart rx/tx pin.*/
    GPIO_SetFunc(BSP_UART_TX_PORT, BSP_UART_TX_PIN, USART_TX_GPIO_FUNC);
    GPIO_SetFunc(BSP_UART_RX_PORT, BSP_UART_RX_PIN, USART_RX_GPIO_FUNC);

    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USART3, ENABLE);

    /** Initialize usart.*/
    (void)USART_UART_StructInit(&stcUartInit);
    stcUartInit.u32ClockDiv         = USART_CLK_DIV4;
    stcUartInit.u32Baudrate         = 115200;
    stcUartInit.u32OverSampleBit    = USART_OVER_SAMPLE_8BIT;

    if(LL_OK != USART_UART_Init(BSP_UART_UNIT, &stcUartInit, NULL))
    {
        
    }

    USART_FuncCmd(BSP_UART_UNIT,  USART_TX, ENABLE);

}
/**
 * @brief  Deinitialize uart.
 * 
 * @param  None
 * @retval None
*/
void bsp_debug_deinit(void)
{
    USART_FuncCmd(BSP_UART_UNIT, USART_RX | USART_TX, DISABLE);
    USART_DeInit(BSP_UART_UNIT);

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

    for(int i=0; i<u32TxLen; i++)
    {
        while(RESET == USART_GetStatus(BSP_UART_UNIT, USART_FLAG_TX_EMPTY))
        {
            ;
        }
        USART_WriteData(BSP_UART_UNIT, pu8TxBuf[i]);
    }
}

/**
 * @brief  Send data via uart.
 * 
 * @param  ch: Data to be sent.
 * @param  f: Pointer to file.
*/
__weak int fputc(int ch, FILE *f)
{
    bsp_debug_send((uint8_t *)&ch, 1UL);
    return ch;
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