/**************************************************************************************
 *
 * �ļ�������
 * �������ڣ�
 *
 * ��ע��
 *
 **************************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HC32F460
#define HC32F460
#endif

///* Includes ------------------------------------------------------------------*/

///* Exported_Macros -----------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include "lib_fifo.h"
#include "hc32_ll.h"


#define MAX_DEV_NUM					8

#define RX_BUF_SIZE					(1048*2)
#define RX_PACKET_SIZE				512

#define TX_BUF_SIZE					2048	
#define TX_PACKET_SIZE				64



#define get_system_tick		HAL_GetTick
#define delay_ms			SysTick_Delay
#define Ddl_Delay1ms delay_ms
#define get_tick_past(A)	(HAL_GetTick() - A)

#define ENTER_CRITICAL_ZONE __disable_irq
#define EXIT_CRITICAL_ZONE	__enable_irq


/* Exported_Types ------------------------------------------------------------*/

typedef struct bsp_dev 
{
		volatile	uint8_t is_sending;
		RingBuffer tx_ring;
		RingBuffer rx_ring;

		uint8_t tx_buffer[TX_BUF_SIZE] __attribute__((aligned(32)));
		uint8_t tx_packet[TX_PACKET_SIZE] __attribute__((aligned(32)));//?????

		uint8_t rx_buffer[RX_BUF_SIZE] __attribute__((aligned(32)));
		uint8_t rx_packet[RX_PACKET_SIZE] __attribute__((aligned(32)));
}bsp_dev_t;


/* Exported_Variables --------------------------------------------------------*/


extern bsp_dev_t usbcdc_dev;





/* Exported_Functions --------------------------------------------------------*/

void set_Vector_Table(void);

void bsp_init(void);

int bsp_deinit(void);

void usb_cdc_bsp_init(void);

void red_led_toggle(void);

void green_led_google(void);

void system_reset(void);

void BSP_CLK_Init(void);//???

void jump_to_app(uint32_t address, uint32_t boot_params);
void usb_ringbuff_init(void);
void INTC_IrqInstalHandler(const stc_irq_signin_config_t *pstcConfig, uint32_t u32Priority);

#define bsp_get_clk() \
{ \
    stc_clock_freq_t stcClockFreq; \
    CLK_GetClockFreq(&stcClockFreq); \
    UAV_LOG_INFO("system clock: %d", stcClockFreq.u32SysclkFreq); \
    UAV_LOG_INFO("hclk clock: %d", stcClockFreq.u32HclkFreq); \
    UAV_LOG_INFO("pclk1 clock: %d", stcClockFreq.u32Pclk1Freq); \
    UAV_LOG_INFO("pclk4 clock: %d", stcClockFreq.u32Pclk4Freq); \
    UAV_LOG_INFO("pclk3 clock: %d", stcClockFreq.u32Pclk3Freq); \
    UAV_LOG_INFO("pclk2 clock: %d", stcClockFreq.u32Pclk2Freq); \
    UAV_LOG_INFO("ext clock: %d", stcClockFreq.u32ExclkFreq); \
}

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
