/**
 *******************************************************************************
 * @file  usb/usb_dev_cdc/source/usb_bsp.c
 * @brief BSP function for USB example
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_bsp.h"
#include "hc32_ll.h"
#include "usb_dev_int.h"
#include "bsp.h"

/**
 * @addtogroup HC32F460_DDL_Applications
 * @{
 */

/**
 * @addtogroup USB_Dev_Cdc
 * @{
 */

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* unlock/lock peripheral */
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)

/* USBFS Core*/
#define USB_DP_PORT     (GPIO_PORT_A)
#define USB_DP_PIN      (GPIO_PIN_12)
#define USB_DM_PORT     (GPIO_PORT_A)
#define USB_DM_PIN      (GPIO_PIN_11)
#define USB_VBUS_PORT   (GPIO_PORT_A)
#define USB_VBUS_PIN    (GPIO_PIN_09)
#define USB_SOF_PORT    (GPIO_PORT_A)
#define USB_SOF_PIN     (GPIO_PIN_08)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
extern usb_core_instance usb_dev;

/**
 * @brief  handle the USB interrupt
 * @param  None
 * @retval None
 */
static void USB_IRQ_Handler(void)
{
    usb_isr_handler(&usb_dev);
}

/**
 * @brief  USB clock initial
 * @param  None
 * @retval None
 */
static void UsbClockIni(void)
{
    stc_clock_pllx_init_t stcUpllInit;

    (void)CLK_PLLxStructInit(&stcUpllInit);
    stcUpllInit.u8PLLState = CLK_PLLX_ON;
    stcUpllInit.PLLCFGR = 0UL;
    stcUpllInit.PLLCFGR_f.PLLM = (2UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLN = (84UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLR = (7UL  - 1UL);
    stcUpllInit.PLLCFGR_f.PLLQ = (7UL - 1UL);
    stcUpllInit.PLLCFGR_f.PLLP = (7UL - 1UL); //48M
    (void)CLK_PLLxInit(&stcUpllInit);

    /* Set USB clock source */
    CLK_SetUSBClockSrc(CLK_USBCLK_PLLXP);
}

/**
 * @brief  initialize configurations for the BSP
 * @param  [in] pdev        device instance
 * @retval None
 */
void usb_bsp_init(usb_core_instance *pdev)
{
    stc_gpio_init_t stcGpioCfg;

    /* Unlock peripherals or registers */
    LL_PERIPH_WE(EXAMPLE_PERIPH_WE);

    BSP_CLK_Init();

#if (LL_PRINT_ENABLE == DDL_ON)
    DDL_PrintfInit(BSP_PRINTF_DEVICE, BSP_PRINTF_BAUDRATE, BSP_PRINTF_Preinit);
#endif

    /* USB clock source configurate */
    UsbClockIni();

#if (LL_PRINT_ENABLE == DDL_ON)
    DDL_Printf("USBFS start !!\r\n");
#endif
    (void)GPIO_StructInit(&stcGpioCfg);

    stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
    (void)GPIO_Init(USB_DM_PORT, USB_DM_PIN, &stcGpioCfg);
    (void)GPIO_Init(USB_DP_PORT, USB_DP_PIN, &stcGpioCfg);
    GPIO_SetFunc(USB_VBUS_PORT, USB_VBUS_PIN, GPIO_FUNC_10); /* VBUS */
    FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS, ENABLE);
}

/**
 * @brief  configure the NVIC of USB
 * @param  None
 * @retval None
 */
void usb_bsp_nvicconfig(void)
{
    stc_irq_signin_config_t stcIrqRegiConf;
    /* Register INT_SRC_USBFS_GLB Int to Vect.No.030 */
    stcIrqRegiConf.enIRQn = INT030_IRQn;
    /* Select interrupt function */
    stcIrqRegiConf.enIntSrc = INT_SRC_USBFS_GLB;
    /* Callback function */
    stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    /* Registration IRQ */
    (void)INTC_IrqSignIn(&stcIrqRegiConf);
    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
//    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIO_15);
		NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIO_01);
	
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
}

/**
 * @brief  This function provides delay time in micro sec
 * @param  [in] usec        Value of delay required in micro sec
 * @retval None
 */
void usb_udelay(const uint32_t usec)
{
    __IO uint32_t i;
    uint32_t j;
    j = (HCLK_VALUE + 1000000UL - 1UL) / 1000000UL * usec;
    for (i = 0UL; i < j; i++) {
    }
}

/**
 * @brief  This function provides delay time in milli sec
 * @param  [in] msec        Value of delay required in milli sec
 * @retval None
 */
void usb_mdelay(const uint32_t msec)
{
    usb_udelay(msec * 1000UL);
}

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
