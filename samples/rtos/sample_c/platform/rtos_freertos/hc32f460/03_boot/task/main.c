#include "boot.h"
#include "bsp_debug.h"
#include "../bsp/bsp_flash.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

/**
 * @brief  main function
 * @param  None
 * @retval None
 */

int main(void)
{	
    int r_code = -1;
    set_Vector_Table(FLASH_BOOTLOADER_START_ADDR);
    BSP_CLK_Init();
	SysTick_Init(1000U);		
    bsp_debug_init();
    r_code = boot_app_upgradation();
    if(r_code  < 0)
    {
        while(1)
        {
            
        }
    }
    else
    {
        boot_run_application();
    }

}

static int log_data(const uint8_t *data, uint16_t dataLen) {
    SEGGER_RTT_Write(0, data, dataLen);
    return 0;
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
