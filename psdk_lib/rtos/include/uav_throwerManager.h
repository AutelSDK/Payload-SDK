/*
 * @Author: Jack && R22454/uavrobotics.cn
 * @Date: 2023-10-27 
 * @LastEditors: Jack && R22454/uavrobotics.cn
 * @LastEditTime: 2023-10-27
 * @Description: uav topic interface file
 * Copyright (c) 2023 Uav Robotics. All rights reserved.
 */
#ifndef _UAV_THROWERMANAGER_H_
#define _UAV_THROWERMANAGER_H_

#include "uav_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	E_UAV_THROWER_TYPE_NONE = 0,
	E_UAV_THROWER_TYPE_MODELX = 1,
	E_UAV_THROWER_TYPE_MODELH,
	E_UAV_THROWER_TYPE_MODELM,
}E_UAVThrowerType;

typedef enum
{
    eThrow_OK                     = 0,
    eThrow_ERR,
}throw_error_e;

typedef struct __thrower
{
	E_UAVThrowerType type;
	void (*throwAct)(uint32_t index);//
	void (*getStatus)(uint32_t *index);//
}T_UAVThrower;
/**
 * @description: UAV_RegThrower
 * @return int
 */
T_UAVReturnCode UAV_RegThrower(T_UAVThrower *thrower); 
T_UAVReturnCode UAV_throwerHeartbeat(void *args, uint16_t args_size) ;
T_UAVReturnCode UAV_throwerGetStatus(void);
	
	
#ifdef __cplusplus
}
#endif

#endif // _UAV_TOPIC_H_