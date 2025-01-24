/**
  ******************************************************************************
  * @file    SearchLight.c
  * @author  Autel Robotics Team
  * @brief   Application.
  *
  *
  @verbatim

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 Autel Robotics..
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* Flash definitions */

#define FLASH_BASE                      (EFM_START_ADDR)
#define FLASH_SIZE                      (EFM_END_ADDR + 1U)
#define FLASH_SECTOR_SIZE               (SECTOR_SIZE)
#define FLASH_SECTOR_NUM                (64U)

/* SRAM definitions */
#define SRAM_SIZE                       (0x02F000UL)
/* Vector table */
#define VECT_TAB_STEP                   (0x400UL)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
int32_t FLASH_CheckAddrAlign(uint32_t u32Addr);
int32_t FLASH_EraseSector(uint32_t u32Addr, uint32_t u32Size);
int32_t FLASH_WriteData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len);
int32_t FLASH_ReadData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len);
void flash_read_words(uint32_t raddr, uint32_t *pbuf, uint32_t num);
void flash_write_words(uint32_t waddr, uint32_t *pbuf, uint32_t num);
void flash_erase(uint32_t saddr, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
