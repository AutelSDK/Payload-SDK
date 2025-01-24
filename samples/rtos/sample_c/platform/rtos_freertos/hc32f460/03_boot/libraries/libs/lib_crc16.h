/**
  ******************************************************************************
  * @file    lib_crc.h
  * @author  Autel Robotics Team
  * @brief   Application.
  *
  *
  @verbatim

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 Autel Robotics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#ifndef __LIBS_CRC_H__
#define __LIBS_CRC_H__

#ifdef __cplus_plus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

/* Exported macros -----------------------------------------------------------------------*/


/* Exported typedef ----------------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------------------*/
extern uint16_t crc16_calculate(uint8_t *buf, uint32_t len);

#ifdef __cplus_plus
}
#endif

#endif /*__LIBS_CRC_H__*/
/*--------------------------------- The End ----------------------------------------------*/

