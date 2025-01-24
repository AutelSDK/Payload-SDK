#ifndef __UAV_PLATFORM_HPP__
#define __UAV_PLATFORM_HPP__
#include <string>
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern T_UAVReturnCode UAV_Uart_Init(const char *dev, int baudrate);
extern T_UAVReturnCode UAV_Usb_Init();

#define LOCALHOST_ETHERNET std::string("lo")
#define LOCALHOST_ETHERNET_TEST std::string("eth0")
extern T_UAVReturnCode UAV_Network_Init(const std::string ether);

extern int get_payload_id(void);

#ifdef __cplusplus
}
#endif

#endif // __UAV_PLATFORM_HPP__
