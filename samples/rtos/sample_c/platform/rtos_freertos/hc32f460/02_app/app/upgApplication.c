#include "upgApplication.h"
#include "uav_logger.h"
#include "hc32_ll.h"
#include "aes.h"

//AES解密
static const uint8_t decrypt_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
}; 
static int64_t file_size = 0;
static boot_confg_t boot_cfg = {0};

static int upg_flash_erase(uint32_t addr, uint32_t size);
static int upg_flash_write(uint32_t addr, uint8_t *data, uint32_t size);
static int upg_flash_read(uint32_t addr, uint8_t *data, uint32_t size);
static void run_boot(void); 


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
         if(crc&0x0001)
               crc=(crc>>1)^0x8408;
         else
               crc=(crc>>1);
        }
    }
	return(crc);
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
        return -1;
    }
    if(upg_flash_write(FLASH_BOOT_CONFIG_START_ADDR, (uint8_t *)cfg, sizeof(boot_confg_t)))
    {
        return -1;
    }
    if(read_boot_cfg(&temp))
    {
        return -1;
    }

    return 0;
}
/**
 *@brief: 可在此回调中完成升级前的准备工作 
 *
 * @param 
 * @return 
*/
int UpgStartCallback(void)
{

    return 0;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
int ReadFileInfoCallback( T_UAVFileInfo *file_info)
{
    if(file_info->file_size > FLASH_APP2_SIZE)
    {
        UAV_LOG_DEBUG("file size is too large,%d > %d", file_info->file_size, FLASH_APP2_SIZE); 
        return -1;
    }
    file_size = file_info->file_size;
    // flash_erase(FLASH_APP2_START_ADDR, FLASH_APP2_SIZE);

    return 0;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
int ReadFileDataCallback(uint32_t offset, uint32_t datalen, char *data)
{
    uint32_t addr = 0;
    static uint32_t app_offset = 0;
    addr = FLASH_APP2_START_ADDR + offset;
	
    if(!offset)
    {
        memcpy(&boot_cfg, data, sizeof(boot_confg_t));
        boot_cfg.magic = MAGIC_UPGRADING;
        if(write_boot_cfg(&boot_cfg))
        {
            UAV_LOG_DEBUG("write boot cfg failed.");
            return -1;
        }
        if(upg_flash_erase(FLASH_APP2_START_ADDR, FLASH_APP2_SIZE))
        {
            UAV_LOG_DEBUG("erase app2 failed.");
            return -1;
        }
        app_offset = 0;
        UAV_LOG_DEBUG("write boot cfg success.");
        UAV_LOG_DEBUG("version: 0x%08x, 0x%08x, 0x%08x", boot_cfg.device_id, boot_cfg.hw_version, boot_cfg.boot_version, boot_cfg.app_version);
        UAV_LOG_DEBUG("app crc = 0x%08x", boot_cfg.app_crc32);
        return 0;
    }
    addr = FLASH_APP2_START_ADDR + app_offset;

    if(upg_flash_write(addr, (uint8_t *)data, datalen))
    {
        return -1;
    }
    
    for(uint32_t i = 0; i < datalen; i++)
    {
        if(data[i] != *((uint8_t *)(addr+i)))
        {
            UAV_LOG_DEBUG("write data error, %d", i);
            return -1;
        }
    }
    app_offset += datalen;
    return 0;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
int FileVerifyCallback(T_UAVVerifyAlgorithmFunc verifyFunc, uint32_t VerifyCodeIn, uint32_t *VerifyCodeOut)
{
    *VerifyCodeOut = VerifyCodeIn; 
    return 0;

    verifyFunc(VerifyCodeOut, (uint8_t *)FLASH_APP2_START_ADDR, file_size);
    if(*VerifyCodeOut == VerifyCodeIn) return 0;
    return -1;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
void UpgFinishCallback(bool result)
{
    if(result == true)
    {
        if(read_boot_cfg(&boot_cfg))
        {
            UAV_LOG_DEBUG("read boot cfg failed.");
            return;
        }
        boot_cfg.magic = MAGIC_UPGRADED;
        if(write_boot_cfg(&boot_cfg))
        {
            UAV_LOG_DEBUG("write boot cfg failed.");
            return;
        }
        run_boot();
    }
    else
    {
        UAV_LOG_DEBUG("upgrade failed.");
    }
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
void upg_set_Vector_Table(void)
{
    SCB->VTOR = FLASH_APP1_START_ADDR;
}
/**
 *@brief: 
 *
 * @param 
 * @return 
*/
static void run_boot(void)
{
   NVIC_SystemReset(); 
   while(1);
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