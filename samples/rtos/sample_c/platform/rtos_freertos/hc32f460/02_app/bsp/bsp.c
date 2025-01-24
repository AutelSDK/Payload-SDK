/*
 * @Author: Jack && R22454/uavrobotics.cn
 * @Date: 2023-10-27 10:58:00
 * @LastEditors: Jack && R22454/uavrobotics.cn
 * @LastEditTime: 2023-10-27 11:32:54
 * @Description: 
 * Copyright (c) 2023 Uav Robotics. All rights reserved.
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "usb_dev_user.h"
#include "usb_dev_desc.h"
#include "usb_dev_cdc_class.h"
#include "cdc_data_process.h"
#include "bsp_tim.h"
#include "bsp_gpio.h"
#include "bsp_debug.h"
#include "bsp_led.h"

typedef  void ( * pIapFun_TypeDef ) ( void ); //

#if ( configAPPLICATION_ALLOCATED_HEAP == 1 )
uint8_t *ucHeap;		// FreeRTOS heap4 ��ջ�ռ�
#endif

bsp_dev_t usbcdc_dev={0};
usb_core_instance  usb_dev;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


#define APP_START_ADDR 0
void set_Vector_Table(void)
{
    SCB->VTOR = APP_START_ADDR;
}

void usb_ringbuff_init(void)//???
{
	int ret;   
	usbcdc_dev.tx_ring.buffer = usbcdc_dev.tx_buffer;
	usbcdc_dev.rx_ring.buffer = usbcdc_dev.rx_buffer;
	ret = RingBuffer_create(&usbcdc_dev.tx_ring, sizeof(usbcdc_dev.tx_buffer));
	ret = RingBuffer_create(&usbcdc_dev.rx_ring, sizeof(usbcdc_dev.rx_buffer));
}

/**
 * @brief  BSP clock initialize.
 *         Set board system clock to MPLL@200MHz
 * @param  None
 * @retval None
 */
__WEAKDEF void BSP_CLK_Init(void)//???
{
    stc_clock_xtal_init_t     stcXtalInit;
    stc_clock_pll_init_t      stcMpllInit;

    (void)CLK_XtalStructInit(&stcXtalInit);
    (void)CLK_PLLStructInit(&stcMpllInit);

    /* Set bus clk div. */
    CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | \
                                      CLK_PCLK2_DIV4 | CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));

    /* Config Xtal and enable Xtal */
    stcXtalInit.u8Mode = CLK_XTAL_MD_OSC;
    stcXtalInit.u8Drv = CLK_XTAL_DRV_ULOW;
    stcXtalInit.u8State = CLK_XTAL_ON;
    stcXtalInit.u8StableTime = CLK_XTAL_STB_2MS;
    (void)CLK_XtalInit(&stcXtalInit);

    /* MPLL config (XTAL / pllmDiv * plln / PllpDiv = 200M). */
    stcMpllInit.PLLCFGR = 0UL;
    stcMpllInit.PLLCFGR_f.PLLM = 1UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLN = 50UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLP = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLQ = 2UL - 1UL;
    stcMpllInit.PLLCFGR_f.PLLR = 2UL - 1UL;
    stcMpllInit.u8PLLState = CLK_PLL_ON;
    stcMpllInit.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
    (void)CLK_PLLInit(&stcMpllInit);
    /* Wait MPLL ready. */
    while (SET != CLK_GetStableStatus(CLK_STB_FLAG_PLL)) {
        ;
    }

    /* sram init include read/write wait cycle setting */
    SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
    SRAM_SetWaitCycle((SRAM_SRAM12 | SRAM_SRAM3 | SRAM_SRAMR), SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);

    /* flash read wait cycle setting */
    (void)EFM_SetWaitCycle(EFM_WAIT_CYCLE5);
    /* 3 cycles for 126MHz ~ 200MHz */
    GPIO_SetReadWaitCycle(GPIO_RD_WAIT3);
    /* Switch driver ability */
    (void)PWC_HighSpeedToHighPerformance();
    /* Switch system clock source to MPLL. */
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
extern usb_core_instance  usb_dev;

void bsp_init(void)
{
	usb_ringbuff_init();	
	usb_dev_init(&usb_dev, &user_desc, &class_cdc_cbk, &user_cb);	
	/* SysTick configuration */
	SysTick_Init(1000U);		
	bsp_gpio_init();
	//bsp_flash_Init();
	//bsp_adc_init();
	bsp_tima_init();
    bsp_led_init();
    __enable_irq();
    delay_ms(1500);
    bsp_debug_init();   //需在delay_ms(2500)后调用,否则波特率会出错
}

void usb_cdc_bsp_init(void)
{
	usb_dev_init(&usb_dev, &user_desc, &class_cdc_cbk, &user_cb);	
}

int bsp_deinit(void)
{
    __disable_irq();


#if (INSTRUCTION_CACHE_ENABLE != 0U)
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
#endif /* INSTRUCTION_CACHE_ENABLE */

#if (DATA_CACHE_ENABLE != 0U)
    __HAL_FLASH_DATA_CACHE_DISABLE();
#endif /* DATA_CACHE_ENABLE */

#if (PREFETCH_ENABLE != 0U)
    __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
#endif /* PREFETCH_ENABLE */

    return 0;
}





#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */HAL_DeInit
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

void system_reset(void)
{
    bsp_deinit();

    __NVIC_SystemReset();
}

uint32_t get_tick(void)
{
    return SysTick_GetTick();
}

void INTC_IrqInstalHandler(const stc_irq_signin_config_t *pstcConfig, uint32_t u32Priority)
{
	if (NULL != pstcConfig)
	{
		(void)INTC_IrqSignIn(pstcConfig);
		NVIC_ClearPendingIRQ(pstcConfig->enIRQn);
		NVIC_SetPriority(pstcConfig->enIRQn, u32Priority);
		NVIC_EnableIRQ(pstcConfig->enIRQn);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
