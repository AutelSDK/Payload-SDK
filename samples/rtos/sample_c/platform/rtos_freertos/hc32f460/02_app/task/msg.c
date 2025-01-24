/**************************************************************************************
 *
 * 文件描述：
 * 创建日期：
 *
 * 备注：
 *
 **************************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private_Defines -----------------------------------------------------------*/

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/
System_Config_t sys_config = {
    .auto_spd[0] = TIM_PERIOD_MIN,
    .auto_spd[1] = TIM_PERIOD_MIN,
    .auto_spd[2] = TIM_PERIOD_MIN,
    .auto_spd[3] = TIM_PERIOD_MIN,

    .manual_spd[0] = TIM_PERIOD_MIN,
    .manual_spd[1] = TIM_PERIOD_MIN,
    .manual_spd[2] = TIM_PERIOD_MIN,
    .manual_spd[3] = TIM_PERIOD_MIN,

    .report_freq = 2};
System_Status_t msys_status = {
    .door_status = 0,
    .x_status = 0,
    .y_status = 0,
    .z_status = 0,
    .sw_door = 0,
    .sw_x = 0,
    .sw_y = 0,
    .sw_z = 0,
    .door_ang = 0,
    .pos_dist = 0,
    .chg_dist = 0,
    .airscrew_dist = {0},
    .sys_mod = 0,
};
extern motor_args_t motor_args;
/* FunctionPrototypes --------------------------------------------------------*/
static void alink_msg_handler(uint8_t *data, uint16_t len);
static void sys_cmd_msg_handler(uint8_t *data, uint16_t len);
static void charge_ctrl_msg_handler(uint8_t *data, uint16_t len);
static void pos_ctrl_msg_handler(uint8_t *data, uint16_t len);
static void door_ctrl_msg_handler(uint8_t *data, uint16_t len);
static void airscrew_ctrl_msg_handler(uint8_t *data, uint16_t len);

/* Functions -----------------------------------------------------------------*/

/******************************************************************************
 * 功能：
 * 输入：无
 * 返回：
 * 说明：
 *
 *****************************************************************************/
void can_rx_msg_proc(void)
{
    can_msg_t *pmsg = NULL;
    static alink_message_head_t msg;
    static alink_status_t status;
    uint32_t len;
    static uint8_t ch;

    pmsg = can_rx_msg_from_fifo();
    if (pmsg == NULL)
    {
        return;
    }

    if (pmsg->rxmsg.rx_ff == CAN_FF_STANDARD)
    {
        if ((pmsg->rxmsg.rx_sfid & ALINK_FILTER_MASK4) == ALINK_FILTER_ID4)
        {
            alink_msg_handler(pmsg->rxmsg.rx_data, pmsg->rxmsg.rx_dlen);
        }
        else if ((pmsg->rxmsg.rx_sfid & SYS_CMD_FILTER_MASK3) == SYS_CMD_FILTER_ID3)
        {
            debug_printf("recv sys_cmd \r\n");
            sys_cmd_msg_handler(pmsg->rxmsg.rx_data, pmsg->rxmsg.rx_dlen);
        }
        else if ((pmsg->rxmsg.rx_sfid & CHARGE_CTRL_FILTER_MASK2) == CHARGE_CTRL_FILTER_ID2)
        {
            debug_printf("recv charge_ctrl \r\n");
            charge_ctrl_msg_handler(pmsg->rxmsg.rx_data, pmsg->rxmsg.rx_dlen);
        }
        else if ((pmsg->rxmsg.rx_sfid & POS_CTRL_FILTER_MASK1) == POS_CTRL_FILTER_ID1)
        {
            debug_printf("recv pos_ctrl \r\n");
            pos_ctrl_msg_handler(pmsg->rxmsg.rx_data, pmsg->rxmsg.rx_dlen);
        }
        else if ((pmsg->rxmsg.rx_sfid & DOOR_CTRL_FILTER_MASK0) == DOOR_CTRL_FILTER_ID0)
        {
            debug_printf("recv door_ctrl \r\n");
            door_ctrl_msg_handler(pmsg->rxmsg.rx_data, pmsg->rxmsg.rx_dlen);
        }
        else if ((pmsg->rxmsg.rx_sfid & AIRSCREW_CTRL_FILTER_MASK4) == AIRSCREW_CTRL_FILTER_ID4)
        {
            debug_printf("recv airscrew \r\n");
            airscrew_ctrl_msg_handler(pmsg->rxmsg.rx_data, pmsg->rxmsg.rx_dlen);
        }
    }
}

static void alink_msg_handler(uint8_t *data, uint16_t len)
{
    int i = 0;
    static alink_status_t alink_stat;
    static alink_message_head_t alink_msg;

    for (i = 0; i < len; i++)
    {
        if (ALINK_FRAMING_OK == alink_parse_char(0, data[i], &alink_msg, &alink_stat))
        {

            if (alink_msg.destid != DEV_NSTDRV_0)
            {
                return;
            }

            if (alink_msg.msgid > 0xD0)
            {
                debug_printf("app recv boot alink %d\r\n", alink_msg.msgid);
                boot_do_cmd(&alink_msg);
            }
            else
            {
                debug_printf("app recv unknow alink %d\r\n", alink_msg.msgid);
            }
        }
    }
}

static void sys_cmd_msg_handler(uint8_t *data, uint16_t len)
{
    uint32_t tmp = 0;
    uint8_t motor = 0, dir;
    switch (data[0])
    {
    case SYS_CMD_SYS_CONFIG:
    {
        motor_args.door_acc_stall_th = data[1];
        motor_args.door_uniform_stall_th = data[2];
        motor_args.door_dec_stall_th = data[3];
        motor_args.pos_acc_stall_th = data[4];
        motor_args.pos_uniform_stall_th = data[5];
        motor_args.chg_acc_stall_th = data[6];
        motor_args.chg_uniform_stall_th = data[7];
        /*
        tmp = calc_timer_peroid(data[1]);
        sys_config.auto_spd[POS_MOTOR] = tmp;
        sys_config.manual_spd[POS_MOTOR] = tmp;
        sys_config.auto_spd[CHARGE_MOTOR] = tmp;
        sys_config.manual_spd[CHARGE_MOTOR] = tmp;
        sys_config.auto_spd[AIRSCREW_MOTOR] = tmp;
        sys_config.manual_spd[AIRSCREW_MOTOR] = tmp;

        tmp = calc_timer_peroid(data[2]);
        sys_config.auto_spd[DOOR_MOTOR] = tmp;
        sys_config.manual_spd[DOOR_MOTOR] = tmp;

        sys_config.report_freq = limit_val(data[3], MIN_REPORT_FREQ, MAX_REPORT_FREQ);
        */

        break;
    }
    case SYS_CMD_SYS_RESET:
    {
        if (data[1] == 0xAA && data[2] == 0x55)
        {
            NVIC_SystemReset();
        }
        break;
    }
    case SYS_CMD_MOTOR_RESET:
    {
        if (data[1] & 0x02)
        {
            drv_motor_start(POS_MOTOR, M1_MAX_STEP_CNT, MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MIN, 1, 1, 1);
        }
        else if (data[1] & 0x04)
        {
            drv_motor_start(CHARGE_MOTOR, M2_MAX_STEP_CNT, MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MIN, 1, 1, 1);
        }
        else if (data[1] & 0x01)
        {
            drv_motor_start(DOOR_MOTOR, M3_MAX_STEP_CNT, MX_DIR_OPEN_HOLD, TIM_PERIOD_MIN, 1, 1, 1);
        }
        else if (data[1] & 0x08)
        {
            drv_motor_start(AIRSCREW_MOTOR, M4_MAX_STEP_CNT, MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MIN, 1, 1, 1);
        }
        break;
    }

    case SYS_CMD_MOTOR_STOP:
    {
        if (data[1]==1)
        {
            msys_status.x_status.EXIGENCY = msys_status.y_status.EXIGENCY = msys_status.z_status.EXIGENCY = msys_status.door_status.EXIGENCY = 1;
            drv_motor_start(POS_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
            drv_motor_start(CHARGE_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
            drv_motor_start(DOOR_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
            drv_motor_start(AIRSCREW_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
        }
        else if(data[1]==2)
        {
            msys_status.x_status.EXIGENCY = msys_status.y_status.EXIGENCY = msys_status.z_status.EXIGENCY = msys_status.door_status.EXIGENCY = 0;
        }
        break;
    }
    case SYS_CMD_MOTOR_STALL_CFG:
    { /*堵转参数标定*/
        // motor_config(data[1], data[2], data[3]);
        break;
    }
    case SYS_CMD_ENTER_CAL_MOD: /*校准模式进入退出*/
    {
        if (data[1] == 1)
        {
            msys_status.sys_mod = 1;
        }
        else if (data[1] == 2)
        {
            msys_status.sys_mod = 0;
            store_and_load_motor_args();
        }
        debug_printf("calibration module: %d\r\n", msys_status.sys_mod);
        break;
    }
    case SYS_CMD_CAL_CFG: /*行程、零点参数校准*/
    {
        if (msys_status.sys_mod)
        {
            motor = data[1];
            if ((msys_status.x_status.SUSPEND != 1 && motor == 1) ||
                (msys_status.y_status.SUSPEND != 1 && motor == 2) ||
                (msys_status.door_status.SUSPEND != 1 && motor == 3))
            {
                break;
            }
                
            if (data[2] == 1)
            {
                dir = 1;
            }
            else if (data[2] == 2)
            {
                dir = 0;
            }
            /*校准最小分辨率0.5度、250步*/
            modify_motor_cfg(motor, dir);
            if (motor > 3)
            {
                drv_motor_start(AIRSCREW_MOTOR, M4_MAX_STEP_CNT, MX_DIR_CLOSE_RELEASE, motor_args.mx_max_spd[3], 1, 0, 0);
            }
            else if (motor == 3)
            {
                debug_printf("calibration action->door dir: %d\r\n", dir);
                drv_motor_start(motor - 1, 250, dir ? MX_DIR_CLOSE_RELEASE : MX_DIR_OPEN_HOLD, motor_args.mx_max_spd[motor - 1], 1, 0, 0);
            }
            else
            {
                drv_motor_start(motor - 1, 250, dir ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, motor_args.mx_max_spd[motor - 1], 1, 0, 0);
            }
        }
        break;
    }
    default:
        break;
    }
}

static void pos_ctrl_msg_handler(uint8_t *data, uint16_t len)
{
    uint8_t cmd = data[0] & POS_CTRL_CMD_MASK;

    if (msys_status.x_status.EXIGENCY)
    {
        return;
    }
    switch (data[0] & POS_CTRL_TYPE_MASK)
    {
    case POS_CTRL_TYPE_AUTO:
    {
        if (cmd == POS_CTRL_CMD_HOLD)
        {
            if (msys_status.x_status.STALL == PCA_HOLD_STALL)
                return;
            if ((msys_status.x_status.STATE == PCA_HOLDING) && (msys_status.x_status.SUSPEND != 1))
                return;
        }
        else if (cmd == POS_CTRL_CMD_RELEASE)
        {
            if (msys_status.x_status.STALL == PCA_RELEASE_STALL)
                return;
            if ((msys_status.x_status.STATE == PCA_RELEASING) && (msys_status.x_status.SUSPEND != 1))
                return;
        }
        drv_motor_start(POS_MOTOR, motor_args.mx_max_step[POS_MOTOR], cmd == POS_CTRL_CMD_HOLD ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.auto_spd[POS_MOTOR], 1, 0, 1);
        break;
    }
    case POS_CTRL_TYPE_STOP:
    {
        drv_motor_start(POS_MOTOR, 0, cmd == POS_CTRL_CMD_HOLD ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MAX, 0, 0, 1);
        if ((!msys_status.sw_x.CLOSE_HOLD_TRI) || (!msys_status.sw_x.OPEN_RELEASE_TRI))
            break;
    }
    case POS_CTRL_TYPE_MANUAL:
    {
        drv_motor_start(POS_MOTOR, M1_MANUAL_STEP_CNT, cmd == POS_CTRL_CMD_HOLD ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.manual_spd[POS_MOTOR], 1, 0, 0);
        break;
    }

    default:
        break;
    }
}

static void charge_ctrl_msg_handler(uint8_t *data, uint16_t len)
{
    uint8_t cmd = data[0] & CHARGE_CTRL_CMD_MASK;

    if (msys_status.y_status.EXIGENCY)
    {
        return;
    }
    switch (data[0] & CHARGE_CTRL_TYPE_MASK)
    {
    case CHARGE_CTRL_TYPE_AUTO:
    {
        if (cmd == CHARGE_CTRL_CMD_HOLD)
        {
            if (msys_status.y_status.STALL == PCA_HOLD_STALL)
                return;
            if ((msys_status.y_status.STATE == PCA_HOLDING) && (msys_status.y_status.SUSPEND != 1))
                return;
        }
        else if (cmd == CHARGE_CTRL_CMD_RELEASE)
        {
            if (msys_status.y_status.STALL == PCA_RELEASE_STALL)
                return;
            if ((msys_status.y_status.STATE == PCA_RELEASING) && (msys_status.y_status.SUSPEND != 1))
                return;
        }
        drv_motor_start(CHARGE_MOTOR, motor_args.mx_max_step[CHARGE_MOTOR], cmd == CHARGE_CTRL_CMD_HOLD ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.auto_spd[CHARGE_MOTOR], 1, 0, 1);
        break;
    }
    case CHARGE_CTRL_TYPE_STOP:
    {
        drv_motor_start(CHARGE_MOTOR, 0, cmd == CHARGE_CTRL_CMD_HOLD ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MAX, 0, 0, 1);
        break;
    }
    case CHARGE_CTRL_TYPE_MANUAL:
    {
        drv_motor_start(CHARGE_MOTOR, M2_MANUAL_STEP_CNT, cmd == CHARGE_CTRL_CMD_HOLD ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.manual_spd[CHARGE_MOTOR], 1, 0, 0);
        break;
    }

    default:
        break;
    }
}

static void door_ctrl_msg_handler(uint8_t *data, uint16_t len)
{
    uint8_t cmd = data[0] & DOOR_CTRL_CMD_MASK;
    uint8_t type = data[0] & DOOR_CTRL_TYPE_MASK;
    if (msys_status.door_status.EXIGENCY)
    {
        return;
    }

    switch (data[0] & DOOR_CTRL_TYPE_MASK)
    {
    case DOOR_CTRL_TYPE_AUTO:
    {
        if (cmd == DOOR_CTRL_CMD_OPEN)
        {
            if (msys_status.door_status.STALL == DOOR_OPEN_STALL)
                return;
            if ((msys_status.door_status.STATE == DOOR_OPENING) && (msys_status.door_status.SUSPEND != 1))
                return;
        }
        else if (cmd == DOOR_CTRL_CMD_CLOSE)
        {
            if (msys_status.door_status.STALL == DOOR_CLOSE_STALL)
                return;
            if ((msys_status.door_status.STATE == DOOR_CLOSING) && (msys_status.door_status.SUSPEND != 1))
                return;
        }
        drv_motor_start(DOOR_MOTOR, motor_args.mx_max_step[DOOR_MOTOR], cmd == DOOR_CTRL_CMD_OPEN ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.auto_spd[DOOR_MOTOR], 1, 0, 1);
        break;
    }
    case DOOR_CTRL_TYPE_STOP:
    {
        drv_motor_start(DOOR_MOTOR, 0, cmd == DOOR_CTRL_CMD_OPEN ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MAX, 0, 0, 1);

        break;
    }
    case DOOR_CTRL_TYPE_MANUAL:
    {
        drv_motor_start(DOOR_MOTOR, M3_MANUAL_STEP_CNT, cmd == DOOR_CTRL_CMD_OPEN ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.manual_spd[DOOR_MOTOR], 1, 0, 0);
        break;
    }

    default:
        break;
    }
}

static void airscrew_ctrl_msg_handler(uint8_t *data, uint16_t len)
{
    uint8_t cmd = data[0] & AIRSCREW_CTRL_CMD_MASK;

    if (msys_status.z_status.EXIGENCY)
    {
        return;
    }

    switch (data[0] & AIRSCREW_CTRL_TYPE_MASK)
    {
    case AIRSCREW_CTRL_TYPE_AUTO:
    {
        if (cmd == AIRSCREW_CTRL_CMD_OPEN)
        {
            if (msys_status.z_status.STALL == PCA_HOLD_STALL)
                return;
            if ((msys_status.z_status.STATE == PCA_HOLDING) && (msys_status.z_status.SUSPEND != 1))
                return;
        }
        else if (cmd == AIRSCREW_CTRL_CMD_CLOSE)
        {
            if (msys_status.z_status.STALL == PCA_RELEASE_STALL)
                return;
            if ((msys_status.z_status.STATE == PCA_RELEASING) && (msys_status.z_status.SUSPEND != 1))
                return;
        }
        drv_motor_start(AIRSCREW_MOTOR, M4_MAX_STEP_CNT, cmd == AIRSCREW_CTRL_CMD_OPEN ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.auto_spd[AIRSCREW_MOTOR], 1, 0, 1);
        break;
    }
    case AIRSCREW_CTRL_TYPE_STOP:
    {
        drv_motor_start(AIRSCREW_MOTOR, 0, cmd == AIRSCREW_CTRL_CMD_OPEN ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, TIM_PERIOD_MAX, 0, 0, 1);
        break;
    }
    case AIRSCREW_CTRL_TYPE_MANUAL:
    {
        drv_motor_start(AIRSCREW_MOTOR, M4_MANUAL_STEP_CNT, cmd == AIRSCREW_CTRL_CMD_OPEN ? MX_DIR_OPEN_HOLD : MX_DIR_CLOSE_RELEASE, sys_config.manual_spd[AIRSCREW_MOTOR], 1, 0, 0);
        break;
    }

    default:
        break;
    }
}

void report_motor_status(void)
{
    static uint8_t buf[8];
    static uint32_t runtim = 0;
    uint8_t sw_status = 0;

    update_tmc_status();

    if (GET_PASTTICK(runtim) < (1000 / sys_config.report_freq))
        return;
    runtim = GET_CURTICK;

    update_motor_status();

    adc_sample_t *adc_sample;
    adc_sample = get_adc_raw();

    buf[0] = *(uint8_t *)&msys_status.door_status;
    // debug_printf("door_status:%d\r\n",*(uint8_t *)&msys_status.door_status);
    if (msys_status.door_status.STALL)
    {
        buf[0] &= ~(3 << 5);
        buf[0] |= 1 << 5;
    }
    buf[1] = msys_status.door_ang;
    buf[2] = *(uint8_t *)&msys_status.sw_door;
    /*if ((buf[2] & (3 << 2)) || (buf[0] & (1 << 5)))
    {
        buf[0] |= 1 << 7;
    }
    else
    {
        buf[0] &= ~(1 << 7);
    }*/
    /*该位为自检电源和驱动芯片状态字*/
    buf[3] = adc_sample->vbat > 1800 ? 0 : 1;
    buf[4] = msys_status.sys_mod;

    can_tx_msg_to_fifo(DRIVER_STATUS_FILTER_ID0, 8, buf);
    can_tx_msg_to_bus();

    memset(buf, 0, 8);
    buf[0] = *(uint8_t *)&msys_status.x_status;
    if (msys_status.x_status.STALL)
    {
        buf[0] &= ~(3 << 5);
        buf[0] |= 1 << 5;
    }

    buf[1] = msys_status.pos_dist;
    buf[2] = *(uint8_t *)&msys_status.sw_x;
    /* if ((buf[2] & (3 << 2)) || (buf[0] & (1 << 5)))
     {
         buf[0] |= 1 << 7;
     }
     else
     {
         buf[0] &= ~(1 << 7);
     }
 */
    can_tx_msg_to_fifo(DRIVER_STATUS_FILTER_ID1, 8, buf);
    can_tx_msg_to_bus();

    memset(buf, 0, 8);
    buf[0] = *(uint8_t *)&msys_status.y_status;
    if (msys_status.y_status.STALL)
    {
        buf[0] &= ~(3 << 5);
        buf[0] |= 1 << 5;
    }
    buf[1] = msys_status.chg_dist;
    buf[2] = *(uint8_t *)&msys_status.sw_y;
    /*  if ((buf[2] & (3 << 2)) || (buf[0] & (1 << 5)))
      {
          buf[0] |= 1 << 7;
      }
      else
      {
          buf[0] &= ~(1 << 7);
      }
  */
    can_tx_msg_to_fifo(DRIVER_STATUS_FILTER_ID2, 8, buf);
    can_tx_msg_to_bus();

    memset(buf, 0, 8);
    buf[0] = *(uint8_t *)&msys_status.z_status;
    if (msys_status.z_status.STALL)
    {
        buf[0] &= ~(3 << 5);
        buf[0] |= 1 << 5;
    }
    buf[1] = msys_status.airscrew_dist[0];
    buf[2] = *(uint8_t *)&msys_status.sw_z;
    /*   if ((buf[2] & (3 << 2)) || (buf[0] & (1 << 5)))
       {
           buf[0] |= 1 << 7;
       }
       else
       {
           buf[0] &= ~(1 << 7);
       }
   */
    can_tx_msg_to_fifo(DRIVER_STATUS_FILTER_ID3, 8, buf);
    can_tx_msg_to_bus();
    //debug_printf("AIRSCREW_MOTOR:err-%d; stall-%d; exigency-%d; suspend-%d; status:%d\r\n;", \
    buf[0]&(1<<7),buf[0]&(1<<5),buf[0]&(1<<4), buf[0]&(1<<3),buf[0]&0x07);
    // debug_printf("AIRSCREW_MOTOR POS:%d\r\n",buf[1]);
     // debug_printf("AIRSCREW_MOTOR LIMIT SWITCH:lock err-%d; release err-%d; locked-%d; release-%d;\r\n", \
    buf[2]&(1<<3),buf[2]&(1<<2),buf[2]&(1<<1), buf[2]&(1<<0));
}

void update_pos_motor_status(uint8_t mx_en, uint8_t sw_open_hold, uint8_t sw_close_release, uint8_t dir, uint8_t mx_stall, uint32_t pos_step)
{
    msys_status.sw_x.OPEN_RELEASE_TRI = sw_close_release;
    msys_status.sw_x.CLOSE_HOLD_TRI = sw_open_hold;
    if (pos_step == motor_args.mx_max_step[POS_MOTOR])
    {
        if (sw_open_hold)
        {
            msys_status.sw_x.CLOSE_HOLD_ERR = 0;
        }
        else
        {
            msys_status.sw_x.CLOSE_HOLD_ERR = 1;
        }
        msys_status.x_status.STATE = PCA_HOLDED;
        msys_status.pos_dist = 100;
    }
    else if (pos_step == 0)
    {
        if (sw_close_release)
        {
            msys_status.sw_x.OPEN_RELEASE_ERR = 0;
        }
        else
        {
            msys_status.sw_x.OPEN_RELEASE_ERR = 1;
        }
        msys_status.x_status.STATE = PCA_RELEASED;
        msys_status.pos_dist = 0;
    }
    else if (MX_POSITION_UNKNOW == pos_step)
    {

        msys_status.sw_x.OPEN_RELEASE_ERR = 0;
        msys_status.sw_x.CLOSE_HOLD_ERR = 0;

        msys_status.x_status.STATE = PCA_INIT;
        msys_status.pos_dist = MX_ANG_DIST_UNKNOW;
    }
    else
    {
        msys_status.sw_x.CLOSE_HOLD_ERR = 0;
        msys_status.sw_x.OPEN_RELEASE_ERR = 0;
        msys_status.x_status.STATE = dir == MX_DIR_OPEN_HOLD ? PCA_HOLDING : PCA_RELEASING;
        msys_status.pos_dist = pos_step * 100 / motor_args.mx_max_step[POS_MOTOR];
    }
    /*更新堵转状态并根据堵转状态查询限位开关状态*/
    msys_status.x_status.STALL = mx_stall;
    (!mx_en) ? (msys_status.x_status.SUSPEND |= MX_MOTOR_STATIC) : (msys_status.x_status.SUSPEND &= ~MX_MOTOR_STATIC);
    // debug_printf("msys_status.sw_x:%d\r\n",msys_status.sw_x);
}

void update_chg_motor_status(uint8_t mx_en, uint8_t sw_open_hold, uint8_t sw_close_release, uint8_t dir, uint8_t mx_stall, uint32_t pos_step)
{
    msys_status.sw_y.OPEN_RELEASE_TRI = sw_close_release;
    msys_status.sw_y.CLOSE_HOLD_TRI = sw_open_hold;
    if (pos_step == motor_args.mx_max_step[CHARGE_MOTOR])
    {
        if (sw_open_hold)
        {
            msys_status.sw_y.CLOSE_HOLD_ERR = 0;
        }
        else
        {
            msys_status.sw_y.CLOSE_HOLD_ERR = 1;
        }
        msys_status.y_status.STATE = PCA_HOLDED;
        msys_status.chg_dist = 100;
    }
    else if (pos_step == 0)
    {
        if (sw_close_release)
        {
            msys_status.sw_y.OPEN_RELEASE_ERR = 0;
        }
        else
        {
            msys_status.sw_y.OPEN_RELEASE_ERR = 1;
        }
        msys_status.y_status.STATE = PCA_RELEASED;
        msys_status.chg_dist = 0;
    }
    else if (MX_POSITION_UNKNOW == pos_step)
    {
        msys_status.sw_y.OPEN_RELEASE_ERR = 0;
        msys_status.sw_y.CLOSE_HOLD_ERR = 0;

        msys_status.y_status.STATE = PCA_INIT;
        msys_status.chg_dist = MX_ANG_DIST_UNKNOW;
    }
    else
    {
        msys_status.sw_y.CLOSE_HOLD_ERR = 0;
        msys_status.sw_y.OPEN_RELEASE_ERR = 0;
        msys_status.y_status.STATE = dir == MX_DIR_OPEN_HOLD ? PCA_HOLDING : PCA_RELEASING;
        msys_status.chg_dist = pos_step * 100 / motor_args.mx_max_step[CHARGE_MOTOR];
    }
    /*更新堵转状态并根据堵转状态查询限位开关状态*/
    msys_status.y_status.STALL = mx_stall;
    (!mx_en) ? (msys_status.y_status.SUSPEND |= MX_MOTOR_STATIC) : (msys_status.y_status.SUSPEND &= ~MX_MOTOR_STATIC);
}

void update_door_motor_status(uint8_t mx_en, uint8_t sw_open_hold, uint8_t sw_close_release, uint8_t dir, uint8_t mx_stall, uint32_t pos_step)
{
    msys_status.sw_door.OPEN_RELEASE_TRI = sw_open_hold;
    msys_status.sw_door.CLOSE_HOLD_TRI = sw_close_release;
   
    if (pos_step == motor_args.mx_max_step[DOOR_MOTOR])
    {
        if (msys_status.sw_door.CLOSE_HOLD_TRI)
        {
            msys_status.sw_door.CLOSE_HOLD_ERR = 0;
        }
        else
        {
            msys_status.sw_door.CLOSE_HOLD_ERR = 1;
        }
        msys_status.door_status.STATE = DOOR_CLOSED;
        msys_status.door_ang = 180;
    }
    else if (pos_step == 0)
    {
        if (msys_status.sw_door.OPEN_RELEASE_TRI)
        {
            msys_status.sw_door.OPEN_RELEASE_ERR = 0;
        }
        else
        {
            msys_status.sw_door.OPEN_RELEASE_ERR = 1;
        }
        msys_status.door_status.STATE = DOOR_OPENED;
        msys_status.door_ang = 0;
    }
    else if (MX_POSITION_UNKNOW == pos_step)
    {
        msys_status.sw_door.CLOSE_HOLD_ERR = 0;
        msys_status.sw_door.OPEN_RELEASE_ERR = 0;
        msys_status.door_status.STATE = PCA_INIT;
        msys_status.door_ang = MX_ANG_DIST_UNKNOW;
    }
    else
    {
        msys_status.sw_door.CLOSE_HOLD_ERR = 0;
        msys_status.sw_door.OPEN_RELEASE_ERR = 0;
        msys_status.door_status.STATE = dir == MX_DIR_OPEN_HOLD ? PCA_HOLDING : PCA_RELEASING;
        msys_status.door_ang = pos_step * 180 / motor_args.mx_max_step[DOOR_MOTOR];
    }
    /*更新堵转状态并根据堵转状态查询限位开关状态*/
    msys_status.door_status.STALL = mx_stall;
    (!mx_en) ? (msys_status.door_status.SUSPEND |= MX_MOTOR_STATIC) : (msys_status.door_status.SUSPEND &= ~MX_MOTOR_STATIC);
}

void update_airscrew_motor_status(uint8_t mx_en, uint8_t sw_open_hold, uint8_t sw_close_release, uint8_t dir, uint8_t mx_stall, uint16_t *mx_airscrew_pulse_width)
{
    msys_status.airscrew_dist[0] = (mx_airscrew_pulse_width[0] - motor_args.mx_airscrew_fb_horizontal[0]) / 7;
    msys_status.airscrew_dist[1] = (motor_args.mx_airscrew_fb_horizontal[1] - mx_airscrew_pulse_width[1]) / 7;
    msys_status.airscrew_dist[2] = (motor_args.mx_airscrew_fb_horizontal[2] - mx_airscrew_pulse_width[2]) / 7;
    msys_status.airscrew_dist[3] = (mx_airscrew_pulse_width[3] - motor_args.mx_airscrew_fb_horizontal[3]) / 7;

    msys_status.sw_z.OPEN_RELEASE_TRI = sw_close_release;
    msys_status.sw_z.CLOSE_HOLD_TRI = sw_open_hold;
    if (sw_open_hold)
    {
        msys_status.z_status.STATE = PCA_HOLDED;
        /*测得角度大于100或小于80，报异常*/
        /*if(msys_status.airscrew_dist[0]>100||msys_status.airscrew_dist[1]>100||msys_status.airscrew_dist[2]>100||msys_status.airscrew_dist[3]>100 \
            ||msys_status.airscrew_dist[0]<80||msys_status.airscrew_dist[1]<80||msys_status.airscrew_dist[2]<80||msys_status.airscrew_dist[3]<80)
        {
            msys_status.z_status.ERR_CODE=1;
        }
        else
        {
            msys_status.z_status.ERR_CODE=0;
        }*/
    }
    else if (sw_close_release)
    {
        msys_status.z_status.STATE = PCA_RELEASED;
    }
    else
    {
        msys_status.z_status.STATE = dir == MX_DIR_OPEN_HOLD ? PCA_HOLDING : PCA_RELEASING;
    }
    msys_status.sw_z.CLOSE_HOLD_ERR = 0;
    msys_status.sw_z.OPEN_RELEASE_ERR = 0;
    (!mx_en) ? (msys_status.z_status.SUSPEND |= MX_MOTOR_STATIC) : (msys_status.z_status.SUSPEND &= ~MX_MOTOR_STATIC);
    // debug_printf("motor_args.mx_airscrew_pulse_width:%d, %d, %d, %d\r\n",motor_args.mx_airscrew_fb_real[0],motor_args.mx_airscrew_fb_real[1],motor_args.mx_airscrew_fb_real[2],motor_args.mx_airscrew_fb_real[3]);
    // debug_printf("airscrew Pos:%d, %d, %d, %d\r\n\r\n",msys_status.airscrew_dist[0],msys_status.airscrew_dist[1],msys_status.airscrew_dist[2],msys_status.airscrew_dist[3]);
}

void exigency_sw_handler(void)
{
    static uint8_t last_sw_state = 0;
    static uint32_t runtim = 0;

    if (GET_PASTTICK(runtim) < 2)
        return;
    runtim = GET_CURTICK;

    if (get_exigency_sw() && !last_sw_state)
    {
        drv_motor_start(POS_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
        drv_motor_start(CHARGE_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
        drv_motor_start(DOOR_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
        drv_motor_start(AIRSCREW_MOTOR, 0, MX_DIR_OPEN_HOLD, TIM_PERIOD_MAX, 0, 0, 1);
        msys_status.x_status.EXIGENCY|= get_exigency_sw();
        msys_status.y_status.EXIGENCY|= get_exigency_sw();
        msys_status.z_status.EXIGENCY|= get_exigency_sw(); 
        msys_status.door_status.EXIGENCY |= get_exigency_sw();
    }
    last_sw_state = get_exigency_sw();
}

/*********************************** END OF FILE *************************************/
