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
#ifndef __MSG_H
#define __MSG_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f30x.h"

/* Exported_Macros -----------------------------------------------------------*/
#define MIN_REPORT_FREQ       2
#define MAX_REPORT_FREQ       20


#define SYS_CMD_SYS_CONFIG          0x01
#define SYS_CMD_SYS_RESET           0x02
#define SYS_CMD_MOTOR_RESET         0x03
#define SYS_CMD_MOTOR_STOP          0x04
#define SYS_CMD_MOTOR_STALL_CFG     0x05
#define SYS_CMD_ENTER_CAL_MOD       0x06
#define SYS_CMD_CAL_CFG             0x07

#define CHARGE_CTRL_TYPE_MASK      (0x03 << 6)
#define CHARGE_CTRL_CMD_MASK       (0x3F << 0)
#define CHARGE_CTRL_TYPE_AUTO      (0x01 << 6)
#define CHARGE_CTRL_TYPE_MANUAL    (0x02 << 6)
#define CHARGE_CTRL_TYPE_STOP      (0x03 << 6)
#define CHARGE_CTRL_CMD_HOLD       (0x01 << 0)
#define CHARGE_CTRL_CMD_RELEASE    (0x02 << 0)

#define POS_CTRL_TYPE_MASK         (0x03 << 6)
#define POS_CTRL_CMD_MASK          (0x3F << 0)
#define POS_CTRL_TYPE_AUTO         (0x01 << 6)
#define POS_CTRL_TYPE_MANUAL       (0x02 << 6)
#define POS_CTRL_TYPE_STOP         (0x03 << 6)
#define POS_CTRL_CMD_HOLD          (0x01 << 0)
#define POS_CTRL_CMD_RELEASE       (0x02 << 0)

#define DOOR_CTRL_TYPE_MASK        (0x03 << 6)
#define DOOR_CTRL_CMD_MASK         (0x3F << 0)
#define DOOR_CTRL_TYPE_AUTO        (0x01 << 6)
#define DOOR_CTRL_TYPE_MANUAL      (0x02 << 6)
#define DOOR_CTRL_TYPE_STOP        (0x03 << 6)
#define DOOR_CTRL_CMD_OPEN         (0x01 << 0)
#define DOOR_CTRL_CMD_CLOSE        (0x02 << 0)

#define AIRSCREW_CTRL_TYPE_MASK    (0x03 << 6)
#define AIRSCREW_CTRL_CMD_MASK     (0x3F << 0)
#define AIRSCREW_CTRL_TYPE_AUTO    (0x01 << 6)
#define AIRSCREW_CTRL_TYPE_MANUAL  (0x02 << 6)
#define AIRSCREW_CTRL_TYPE_STOP    (0x03 << 6)
#define AIRSCREW_CTRL_CMD_OPEN     (0x01 << 0)
#define AIRSCREW_CTRL_CMD_CLOSE    (0x02 << 0)
#pragma pack (1)
/* Exported_Types ------------------------------------------------------------*/
typedef struct {
    uint32_t auto_spd[4];    //TIM_PEROID
    uint32_t manual_spd[4];  //TIM_PEROID
    uint8_t report_freq;     //Hz
} System_Config_t;

#define  MX_ANG_DIST_UNKNOW   (0xFF)
#define  MX_MOTOR_STATIC      ((uint8_t)(0x01))

typedef struct {
	uint8_t STATE: 3;
	uint8_t SUSPEND: 1;
	uint8_t EXIGENCY: 1;
	uint8_t STALL: 2;
    uint8_t ERR_CODE: 1;
}Door_xyz_Status_t;
typedef struct {
    uint8_t OPEN_RELEASE_TRI: 1;
	uint8_t CLOSE_HOLD_TRI: 1;
	uint8_t OPEN_RELEASE_ERR: 1;
	uint8_t CLOSE_HOLD_ERR: 1;
	uint8_t : 4;
	
}Door_xyz_Sw_t;

typedef struct {
    Door_xyz_Status_t door_status;
    Door_xyz_Status_t x_status;
    Door_xyz_Status_t y_status;
    Door_xyz_Status_t z_status;
    Door_xyz_Sw_t sw_door;
    Door_xyz_Sw_t sw_x;
    Door_xyz_Sw_t sw_y;
    Door_xyz_Sw_t sw_z;
    uint8_t door_ang;
    uint8_t pos_dist;
	uint8_t chg_dist;
    uint8_t airscrew_dist[4];
    uint8_t sys_mod;
}System_Status_t;


/*
typedef struct {

    uint8_t door_err;         //door errcode
    uint8_t chg_err;		//chg errcode
    uint8_t pos_err;		///pos errcode
    uint8_t airscrew_err;     //airscrew errcode
    uint8_t sw_err;           //switch trig timeout err
    uint8_t door_state;       //door motor state
    uint8_t pos_state;        //position motor state
    uint8_t chg_state;        //charge motor state
    uint8_t airscrew_state;   //airscrew motor state
    uint8_t exigency_state;		//exigency_sw state
    uint8_t door_ang;         //door angle from ground
    uint8_t pos_dist;         //current position motor distance from release_pos
    uint8_t chg_dist;         //current charge motor distance from release_pos
    uint8_t airscrew_dist;    //airscrew motor distance from release_pos

} System_Status_t;
*/
//pos chg airscrew motor state
typedef enum {
    PCA_INIT = 0,
    PCA_HOLDED = 1,
    PCA_RELEASED = 2,
    PCA_HOLDING = 3,
    PCA_RELEASING = 4,

} PCA_Status_t;

typedef enum {
    PCA_NO_STALL = 0,
    PCA_HOLD_STALL = 1,
    PCA_RELEASE_STALL = 2,
    
}PCA_Stall_Status_t;

typedef enum {
    DOOR_INIT = 0,
    DOOR_OPENED = 1,
    DOOR_CLOSED = 2,
    DOOR_OPENING = 3,
    DOOR_CLOSING = 4,

} Door_Status_t;

typedef enum {
    DOOR_NO_STALL = 0,
    DOOR_OPEN_STALL = 1,
    DOOR_CLOSE_STALL = 2,
    
}Door_Stall_Status_t;

#pragma pack ( )

/* Exported_Variables --------------------------------------------------------*/



/* Exported_Functions --------------------------------------------------------*/

void can_rx_msg_proc(void);
void report_motor_status(void);

void update_pos_motor_status(uint8_t mx_en, uint8_t sw_start, uint8_t sw_end, uint8_t dir, uint8_t mx_stall, uint32_t pos_step);
void update_chg_motor_status(uint8_t mx_en, uint8_t sw_start, uint8_t sw_end, uint8_t dir, uint8_t mx_stall, uint32_t pos_step);
void update_door_motor_status(uint8_t mx_en, uint8_t sw_start, uint8_t sw_end, uint8_t dir, uint8_t mx_stall, uint32_t pos_step);
void update_airscrew_motor_status(uint8_t mx_en, uint8_t sw_open_hold, uint8_t sw_close_release, uint8_t dir, uint8_t mx_stall, uint16_t *mx_airscrew_pulse_width);
void exigency_sw_handler(void);

#endif
