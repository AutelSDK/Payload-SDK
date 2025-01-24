#ifndef _UPG_APPLICATION_H_
#define _UPG_APPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uav_upgrade.h"
#include "bsp_flash.h"
/*
		-----------------------------------------------------------------------
area	|bootloader| boot_config  | application1 | application2 | parameter   |
		|----------|--------------|--------------|--------------|-------------|
size	|  32K     |  8K          |  232K        |  232K        |  8K         |
		|----------|--------------|--------------|--------------|-------------|
addr	|  0x0000  |  0x8000      |  0xA000      |  0x44000     |  0x7E000    |
		-----------------------------------------------------------------------
*/
#define FLASH_BOOTLOADER_START_ADDR     (FLASH_BASE)
#define FLASH_BOOTLOADER_SIZE           (32*1024)
#define FLASH_BOOTLOADER_END_ADDR       (FLASH_BOOTLOADER_START_ADDR + FLASH_BOOTLOADER_SIZE)

#define FLASH_BOOT_CONFIG_START_ADDR    (FLASH_BOOTLOADER_END_ADDR)
#define FLASH_BOOT_CONFIG_SIZE          (8*1024)
#define FLASH_BOOT_CONFIG_END_ADDR      (FLASH_BOOT_CONFIG_START_ADDR + FLASH_BOOT_CONFIG_SIZE)

#define FLASH_APP1_START_ADDR           (FLASH_BOOT_CONFIG_END_ADDR)
#define FLASH_APP1_SIZE                 (232*1024)
#define FLASH_APP1_END_ADDR             (FLASH_APP1_START_ADDR + FLASH_APP1_SIZE)

#define FLASH_APP2_START_ADDR           (FLASH_APP1_END_ADDR)
#define FLASH_APP2_SIZE                 (232*1024)
#define FLASH_APP2_END_ADDR             (FLASH_APP2_START_ADDR + FLASH_APP2_SIZE)

#define FLASH_PARAMETER_START_ADDR      (FLASH_APP2_END_ADDR)
#define FLASH_PARAMETER_SIZE            (8*1024)
#define FLASH_PARAMETER_END_ADDR        (FLASH_PARAMETER_START_ADDR + FLASH_PARAMETER_SIZE)



#define MAGIC_UPGRADING                 (0xBEAF1234)
#define MAGIC_NORMAL                    (0xAA55AA02)
#define MAGIC_UPGRADED                  (0xBEAF5678)
#pragma pack(1)
typedef struct _boot_config{
	uint32_t magic;				
	uint32_t device_id;	
	uint32_t hw_version;
	uint32_t boot_version;		
	uint32_t boot_size;		
	uint32_t boot_crc32;	
	uint32_t rev3;				
	uint32_t app_version;	
	uint32_t app_size;			
	uint32_t app_crc32;			
	uint32_t min_hw_version;	
	uint32_t max_hw_version;	
	uint8_t  firm[64];			
	uint8_t  sn[96];			
    uint16_t crc16;
} boot_confg_t;
#pragma pack()


void upg_set_Vector_Table(void);
int UpgStartCallback(void);
int ReadFileInfoCallback( T_UAVFileInfo *file_info);
int ReadFileDataCallback(uint32_t offset, uint32_t datalen, char *data);
int FileVerifyCallback(T_UAVVerifyAlgorithmFunc verifyFunc, uint32_t VerifyCodeIn, uint32_t *VerifyCodeOut);
void UpgFinishCallback(bool result);

#ifdef __cplusplus
}
#endif


#endif // _UPG_APPLICATION_H_

