#include "boot.h"
#include "aes.h"
#include "string.h"
#include "bsp_debug.h"

//AES解密
static const uint8_t decrypt_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
}; 

uint32_t g_reboot_magic   __attribute__((section(".bss.ARM.__at_0x20000000")));
/**
 * @brief
 * param:
 * retval:
 */
uint32_t calc_crc32(uint8_t *buf, uint32_t len, uint32_t seed)
{
    uint32_t crc = seed;/*0x00000000;*/
    uint32_t i=0,j=0;
    uint32_t uiCount;
    uint8_t n;
    uint32_t uiTemp;
    uint8_t temp[4] = {0};
    
    uiCount = len/sizeof(uint32_t);
    
    for ( i = 0; i < uiCount; i ++) {
        uiTemp = *(uint32_t*)buf;
        for ( j = 0; j < 32; j ++) {
            if ((crc^uiTemp) & 0x80000000) {
                crc = 0x04C11DB7^(crc<<1);
            } else {
                crc <<= 1;
            }
            uiTemp <<= 1;
        }
        buf += sizeof(uint32_t);
    }
    
    n = len%sizeof(uint32_t);    
    if (n) {
        for (i=0;i<4;i++) {
            if (i<n) {
                temp[i] = *(buf+i);
            } else {
                temp[i] = 0xff;
            }
        }
        uiTemp = *(uint32_t*)temp;//Fill in the end of data with 0xFF

        for ( j = 0; j < 32; j ++) {
            if ((crc^uiTemp) & 0x80000000) {
                crc = 0x04C11DB7^(crc<<1);
            } else {
                crc <<= 1;
            }
            uiTemp <<= 1;
        }
    }    
    return crc;
}



/**
 * @brief
 * param:
 * retval:
 */
static int32_t CheckFlashAppCode(uint32_t flash_addr,uint32_t nCrc, uint32_t nSize)
{

    uint32_t ComputedCRC = 0;
    uint32_t suAppCrcInitial = 0x00000000;


    ComputedCRC = calc_crc32((uint8_t *)flash_addr, nSize, suAppCrcInitial);


    if(nCrc != ComputedCRC)
    {
        return -1;
    }
    return 0;
}


/**
 *  @brief:
 * 
 *  @param:
 *  @retval: 
 */
static uint16_t cal_crc16(uint8_t *ptr,  uint16_t len)	
{
	uint16_t crc;
	uint16_t i,j;
	crc=0xffff;
    for(i=0;i<len;i++)
    {
        crc^=ptr[i];
        for(j=0;j<8;j++)
        {
            if(crc&0x0001) crc=(crc>>1)^0x8408;
            else crc=(crc>>1);
        }
    }
	return(crc);
}


/**
 *@brief: 
 *
 * @param 
 * @return 
*/
static int upg_flash_erase(uint32_t addr, uint32_t size)
{
    if(addr % FLASH_SECTOR_SIZE) return -1;
    if(size > FLASH_APP2_SIZE) size = FLASH_APP2_SIZE;

    flash_erase(addr, size);

    return 0;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
static int upg_flash_write(uint32_t addr, uint8_t *data, uint32_t size)
{
    uint32_t *pWr = (uint32_t *)data;
    uint32_t wrSize = 0;

    if(addr % 4) return -1;

    wrSize = (size+3)>>2;

    for(uint32_t i = 0; i < wrSize; i++)
    {
        uint32_t dat = pWr[i];
        uint32_t mask = 0xffffffff;
        if(i == wrSize-1)
        {
            if(size%4)
            {
                mask = 0xffffffff >> (4-size%4)*8;
                dat &= mask;
                dat |= (~mask);
            }
        }
        flash_write_words(addr, &dat, 1);
        addr += 4;
    }


    return 0;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
static int upg_flash_read(uint32_t addr, uint8_t *data, uint32_t size)
{
    uint32_t *pRd = (uint32_t *)data;
    uint32_t rdSize = 0;

    if(addr % 4) return -1;

    rdSize = (size+3)>>2;

    for(uint32_t i = 0; i < rdSize; i++)
    {
        uint32_t dat = 0;
        flash_read_words(addr, &dat, 1);
        addr += 4;
        if((i == rdSize-1) && (size%4))
        {
            for(uint32_t j = 0; j < size%4; j++)
            {
                data[i*4+j] = (dat >> (j*8)) & 0x000000ff;
            }
        }
        else
        {
            pRd[i] = dat;
        }
    }

    return 0;
}
/**
 *  @brief:
 * 
 *  @param:
 *  @retval: 
 */
static int read_boot_cfg(boot_confg_t *cfg)
{
    upg_flash_read(FLASH_BOOT_CONFIG_START_ADDR, (uint8_t *)cfg, sizeof(boot_confg_t));
    if(cfg->crc16 != cal_crc16((uint8_t *)cfg, sizeof(boot_confg_t)-2))
    {
        LOG_DEBUG("boot cfg crc error\r\n");
        return -1;
    }
    return 0;
}
/**
 *  @brief:
 * 
 *  @param:
 *  @retval: 
 */
static int write_boot_cfg(boot_confg_t *cfg)
{
    boot_confg_t temp = {0};
    cfg->crc16 = cal_crc16((uint8_t *)cfg, sizeof(boot_confg_t)-2);

    if(upg_flash_erase(FLASH_BOOT_CONFIG_START_ADDR, FLASH_BOOT_CONFIG_SIZE))
    {
        LOG_DEBUG("boot cfg erase error\r\n");
        return -1;
    }
    if(upg_flash_write(FLASH_BOOT_CONFIG_START_ADDR, (uint8_t *)cfg, sizeof(boot_confg_t)))
    {
        LOG_DEBUG("boot cfg write error\r\n");
        return -1;
    }
    if(read_boot_cfg(&temp))
    {
        LOG_DEBUG("boot cfg write comfirm failed\r\n");
        return -1;
    }

    return 0;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
int boot_app_upgradation(void)
{
    boot_confg_t boot_cfg = {0};
    uint8_t buf[512] = {0};
    uint32_t size = sizeof(buf);
    uint32_t offset = 0;
	
	uint8_t decrypt_buf[sizeof(buf)] = {0};
    uint32_t decrypt_len = 0;
    uint32_t progress = 0, progress_last = 0;

    if(read_boot_cfg(&boot_cfg))
    {
        goto VERIFY_APP;
    }
    if(boot_cfg.magic != MAGIC_UPGRADED)
    {
        goto VERIFY_APP;
    }
    /** upgradation.*/
    LOG_DEBUG("upgrading...\r\n");
    if(upg_flash_erase(FLASH_APP1_START_ADDR, FLASH_APP1_SIZE))
    {
        LOG_DEBUG("erase app1 failed\r\n");
        return -1;
    }
    while(1)
    {
        memset(buf, 0xff, sizeof(buf));
        if(boot_cfg.app_size - offset < sizeof(buf))
        {
            size = (((boot_cfg.app_size - offset)+15)>>4)<<4;
        }
        else
        {
            size = sizeof(buf);
        }

        if(upg_flash_read(FLASH_APP2_START_ADDR+offset, buf, size))
        {
            LOG_DEBUG("read app2 failed\r\n");
            return -1;
        }
        

        for(int i=0; i<size; i+=16)
        {
            AES128_ECB_decrypt((uint8_t *)(buf+i), decrypt_key, decrypt_buf+i);
        }

        if(upg_flash_write(FLASH_APP1_START_ADDR+offset, (uint8_t *)decrypt_buf, size))
        {
            LOG_DEBUG("write app1 failed\r\n");
            return -1;
        }

        offset += size;
        progress = offset*100/boot_cfg.app_size;
        if(progress != progress_last)
        {
            progress_last = progress;
            LOG_DEBUG("progress:%d%%\r\n", progress);
        }
        if(offset >= boot_cfg.app_size)
        {
            LOG_DEBUG("upgradation done.\r\n");
            break;
        }
        if(offset >= FLASH_APP1_SIZE)
        {
            return -1;
        }
    }
VERIFY_APP:
    if(CheckFlashAppCode(FLASH_APP1_START_ADDR, boot_cfg.app_crc32, boot_cfg.app_size))
    {
        LOG_DEBUG("verify app1 failed.\r\n");
        return -1;
    }
    if(boot_cfg.magic == MAGIC_UPGRADED)
    {
        boot_cfg.magic = MAGIC_NORMAL;
        if(write_boot_cfg(&boot_cfg))
        {
            LOG_DEBUG("update magic failed.\r\n");
            return -1;
        }
    }
    LOG_DEBUG("success, and Jump to app1 .\r\n");
    return 0;
}

/**
 *@brief: 
 *
 * @param 
 * @return 
*/
void set_Vector_Table(uint32_t addr)
{
    SCB->VTOR = addr;
}

/**
 *@brief: 
 *
 * @param 
 * @return 
*/
void boot_run_application(void)
{
    start_app_function start_application;

    start_application = (start_app_function)(*(__IO uint32_t*)(APP_START_ADDR + 4));

    __disable_irq();
    __set_MSP(*(__IO uint32_t*) APP_START_ADDR);
    set_Vector_Table(APP_START_ADDR);
    start_application();
}




