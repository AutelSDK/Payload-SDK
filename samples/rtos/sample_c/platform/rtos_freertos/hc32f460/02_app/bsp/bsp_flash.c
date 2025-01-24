/**
  ******************************************************************************
  * @file    SearchLight.c
  * @author  Uav Robotics Team
  * @brief   Application.
  *
  *
  @verbatim

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 Uav Robotics..
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "bsp_flash.h"

static uint32_t GetSector(uint32_t Address);



/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Check address alignment.
 * @param  u32Addr                      Flash address
 * @retval An en_result_t enumeration value:
 *           - LL_OK: Address aligned
 *           - LL_ERR: Address unaligned
 */
int32_t FLASH_CheckAddrAlign(uint32_t u32Addr)
{
    uint32_t u32Step = FLASH_SECTOR_SIZE;

    if (VECT_TAB_STEP > FLASH_SECTOR_SIZE) {
        u32Step = VECT_TAB_STEP;
    }
    if ((u32Addr % u32Step) != 0UL) {
        return LL_ERR;
    }

    return LL_OK;
}

/**
 * @brief  Erase flash sector.
 * @param  u32Addr                      Flash address
 * @param  u32Size                      Firmware size (0: current address sector)
 * @retval An en_result_t enumeration value:
 *           - LL_OK: Erase succeeded
 *           - LL_ERR: Erase timeout
 *           - LL_ERR_INVD_PARAM: The parameters is invalid.
 */
int32_t FLASH_EraseSector(uint32_t u32Addr, uint32_t u32Size)
{
    uint32_t i;
    uint32_t u32PageNum;

    if (u32Addr >= (FLASH_BASE + FLASH_SIZE)) {
        return LL_ERR_INVD_PARAM;
    }

    if (u32Size == 0U) {
        return EFM_SectorErase(u32Addr);
    } else {
        u32PageNum = u32Size / FLASH_SECTOR_SIZE;
        if ((u32Size % FLASH_SECTOR_SIZE) != 0UL) {
            u32PageNum += 1U;
        }
        for (i = 0; i < u32PageNum; i++) {
            if (LL_OK != EFM_SectorErase(u32Addr + (i * FLASH_SECTOR_SIZE))) {
                return LL_ERR;
            }
        }
    }

    return LL_OK;
}

/**
 * @brief  Write data to flash.
 * @param  u32Addr                      Flash address
 * @param  pu8Buff                      Pointer to the buffer to be written
 * @param  u32Len                       Buffer length
 * @retval int32_t:
 *           - LL_OK: Program successful.
 *           - LL_ERR_INVD_PARAM: The parameters is invalid.
 *           - LL_ERR_NOT_RDY: EFM if not ready.
 *           - LL_ERR_ADDR_ALIGN: Address alignment error
 */
int32_t FLASH_WriteData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (FLASH_BASE + FLASH_SIZE))) {
        return LL_ERR_INVD_PARAM;
    }
    if (0UL != (u32Addr % 4U)) {
        return LL_ERR_ADDR_ALIGN;
    }
    return EFM_Program(u32Addr, pu8Buff, u32Len);
}

/**
 * @brief  Read data from flash.
 * @param  u32Addr                      Flash address
 * @param  pu8Buff                      Pointer to the buffer to be reading
 * @param  u32Len                       Buffer length
 * @retval int32_t:
 *           - LL_OK: Read data succeeded
 *           - LL_ERR_INVD_PARAM: The parameters is invalid
 *           - LL_ERR_ADDR_ALIGN: Address alignment error
 */
int32_t FLASH_ReadData(uint32_t u32Addr, uint8_t *pu8Buff, uint32_t u32Len)
{
    uint32_t i;
    uint32_t u32WordLength, u8ByteRemain;
    uint32_t *pu32ReadBuff;
    __IO uint32_t *pu32FlashAddr;
    uint8_t  *pu8Byte;
    __IO uint8_t  *pu8FlashAddr;

    if ((pu8Buff == NULL) || (u32Len == 0U) || ((u32Addr + u32Len) > (FLASH_BASE + FLASH_SIZE))) {
        return LL_ERR_INVD_PARAM;
    }
    if (0UL != (u32Addr % 4U)) {
        return LL_ERR_ADDR_ALIGN;
    }

    pu32ReadBuff  = (uint32_t *)(uint32_t)pu8Buff;
    pu32FlashAddr = (uint32_t *)u32Addr;
    u32WordLength = u32Len / 4U;
    u8ByteRemain  = u32Len % 4U;
    /* Read data */
    for (i = 0UL; i < u32WordLength; i++) {
        *(pu32ReadBuff++) = *(pu32FlashAddr++);
    }
    if (0UL != u8ByteRemain) {
        pu8Byte      = (uint8_t *)pu32ReadBuff;
        pu8FlashAddr = (uint8_t *)pu32FlashAddr;
        for (i = 0UL; i < u8ByteRemain; i++) {
            *(pu8Byte++) = *(pu8FlashAddr++);
        }
    }

    return LL_OK;
}

/**************************************************************************************************
* 函数名称: flash_read_words
* 输入参数: uint32_t 起始地址(此地址必须为4的倍数!!)
            uint32_t* 数据指针
            uint32_t 字(32位)数
* 返回参数: none
* 功能描述: 从指定地址开始读出指定长度的数据
* 修改记录: 1. first initialized,  2018.4.17
**************************************************************************************************/
void flash_read_words(uint32_t raddr, uint32_t *pbuf, uint32_t num)
{
    uint32_t i;

    for(i=0; i < num; i++)
    {
        /* 读取4个字节 */
        pbuf[i] = *(uint32_t*)(raddr);
        /* 偏移4个字节 */
        raddr += 4;
    }
}

/**************************************************************************************************
* 函数名称: flash_write_words
* 输入参数: uint32_t 起始地址(此地址必须为4的倍数!!)
            uint32_t* 数据指针
            uint32_t 字(32位)数(就是要写入的32位数据的个数.)
* 返回参数: none
* 功能描述: 从指定地址开始写入指定长度的数据
* 修改记录: 1. first initialized,  2018.4.17
**************************************************************************************************/
void flash_write_words(uint32_t waddr, uint32_t *pbuf, uint32_t num)
{
    uint32_t i = 0;
    EFM_REG_Unlock();
    EFM_FWMC_Cmd(ENABLE);
    FLASH_WriteData(waddr, (uint8_t *)pbuf, num*4);
    EFM_FWMC_Cmd(DISABLE);
    EFM_REG_Lock();

}


/**************************************************************************************************
* 函数名称: flash_erase
* 输入参数: uint32_t 起始地址,必须是扇区地址
            uint32_t 擦除大小,必须是山区大小整数倍
* 返回参数: none
* 功能描述: 擦除扇区
* 修改记录: 1. first initialized,  2018.4.17
**************************************************************************************************/
void flash_erase(uint32_t saddr, uint32_t size)
{
    EFM_REG_Unlock();
    EFM_FWMC_Cmd(ENABLE);
    FLASH_EraseSector(saddr, size);
    EFM_FWMC_Cmd(DISABLE);
    EFM_REG_Lock();
}



/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
