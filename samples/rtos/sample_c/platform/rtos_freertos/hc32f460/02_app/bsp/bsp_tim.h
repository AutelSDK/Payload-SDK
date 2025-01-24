#ifndef __BSP_TIM4_H__
#define __BSP_TIM4_H__

#include "hc32_ll.h"

#define THROW_OPEN 1
#define THROW_CLOSE 0

#define MODULE_X   1

#define ONE_STEP_TIME 10.24 //uS
//MDM塑料舵机500~2500uS,180°±10°,180°/2000uS=0.09°/uS
//中位是1500uS，结构设计动作角度30°,30/0.09/10.24=32.5

#if defined MODULE_X

#define OPEN_PEM 	208			//156*10.24=1603uS
#define CLOSE_PWM 	156			//200*10.24=2048uS
#else
#define OPEN_PEM 	167			//163*10.24=1669uS
#define CLOSE_PWM 	208			//200*10.24=2048uS
#endif

void bsp_tim4_init(void);
unsigned int RTOS_GetRunTimeBase();


void bsp_tima_init(void);
void set_throw1_pwm(uint8_t mode);
void set_throw2_pwm(uint8_t mode);
void set_throw3_pwm(uint8_t mode);
void set_throw4_pwm(uint8_t mode);
void set_throw5_pwm(uint8_t mode);
void set_throw6_pwm(uint8_t mode);
void set_throw7_pwm(uint8_t mode);
void set_throw8_pwm(uint8_t mode);

void ThrowerAct(uint32_t index);
void getThrowerStatus (uint32_t *index);

#endif
