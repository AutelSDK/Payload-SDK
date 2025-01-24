#ifndef __UAV_PARACHUTE_H__
#define __UAV_PARACHUTE_H__

#include "uav_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
    void (*open)(void);

}T_UAVParachute;


T_UAVReturnCode UAV_RegParachute(T_UAVParachute *parachute);

#ifdef __cplusplus
}
#endif

#endif // __AUTEL_PARACHUTE_H__
