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



///* Includes ------------------------------------------------------------------*/

///* Exported_Macros -----------------------------------------------------------*/
#include "hc32_ll.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>



/* Exported_Variables --------------------------------------------------------*/








/* Exported_Functions --------------------------------------------------------*/

//int bsp_init(void);
void bsp_init(void);

int bsp_deinit(void);

void BSP_CLK_Init(void);




#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
