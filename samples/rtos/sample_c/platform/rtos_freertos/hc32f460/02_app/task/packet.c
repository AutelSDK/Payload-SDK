/*============================= (C) Copyright 2015 Autel ===================================
文件名: packet.c
描  述: 协议封装接口程序（数据按照协议打包、解包）
维  护: None
================================ All rights reserved =====================================*/
/* Includes ------------------------------------------------------------------------------*/
#include "includes.h"


/* Private typedef -----------------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------------------*/
static uint8_t send_buf[512];
//const uint8_t crc_extra[256]= ALINK_MESSAGE_CRCS;

/* Private function prototypes -----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------------------*/


/* Public functions ----------------------------------------------------------------------*/

/******************************************************************************************
 Function Name: 应答/非应答函数
 Description  : ACK / NACK应答
 Input Params : @*pmsg--消息指针
                @ack--应答类型ACK / NACK
 Output Params: @return-成功返回1，失败返回负数
 Global	List  : None
 Note         : 调用该函数时发送包序列号为最近接收的数据包序号
 Usage        : None
******************************************************************************************/
int packet_ack(alink_message_head_t *pmsg, msg_ack_e ack)
{
    uint16_t len;
    uint8_t *pbyte = (uint8_t *)pmsg->payload64;

    if(pmsg == NULL) {
        return -1;
    }

    if(!(ack == MSG_ACK || ack == MSG_NACK)) {
        return -2;
    }

    pmsg->len = 1;
    pbyte[0] = ack;

    alink_finalize_message_chan(pmsg, pmsg->sourceid, pmsg->destid, 0, pmsg->len, alink_message_crcs[pmsg->msgid]);
    can_tx_alink_msg(pmsg, pmsg->len + ALINK_NUM_NON_PAYLOAD_BYTES);

    return 1;
}


/******************************************************************************************
 Function Name: 应答消息函数
 Description  : 应答消息
 Input Params : @pmsg		消息指针
 Output Params: @return		成功返回1，失败返回负数
 Global	List  : None
 Note         : 调用该函数时发送包序列号不变
 Usage        : None
******************************************************************************************/
int packet_reply(alink_message_head_t *pmsg)
{
    uint16_t len;

    if(pmsg == NULL) {
        return -1;
    }

    alink_finalize_message_chan(pmsg, pmsg->sourceid, pmsg->destid, 0, pmsg->len, alink_message_crcs[pmsg->msgid]);
    can_tx_alink_msg(pmsg, pmsg->len + ALINK_NUM_NON_PAYLOAD_BYTES);

    return 1;
}


/*--------------------------------- The End ----------------------------------------------*/

