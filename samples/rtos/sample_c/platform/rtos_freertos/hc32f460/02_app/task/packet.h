/*============================= (C) Copyright 2015 Autel ===================================
文件名: packet.h
描  述: 
维  护: None
================================ All rights reserved =====================================*/
#ifndef __PACKET_H__
#define __PACKET_H__


/* Includes ------------------------------------------------------------------------------*/
#include "alink.h"
#include "stdint.h"

/* Exported macros -----------------------------------------------------------------------*/

#ifndef MAX
#define MAX(a,b) (a>b ? a : b)
#endif

#ifndef MIN
#define MIN(a,b) (a<b ? a : b)
#endif


#define COM0    0

typedef enum {MSG_ACK = 0, MSG_NACK = 1, UPG_NOT_MATCH = 9} msg_ack_e;


/* Exported functions --------------------------------------------------------------------*/
extern int packet_reply(alink_message_head_t *pmsg);
extern int packet_ack(alink_message_head_t *pmsg, msg_ack_e ack);


#endif /*__PACKET_H__*/
/*--------------------------------- The End ----------------------------------------------*/
