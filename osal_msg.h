/*
 * osal_msg.h
 *
 *  Created on: 2022.10.3
 *      Author: Felix
 */

#ifndef CODE_OSAL_MSG_H_
#define CODE_OSAL_MSG_H_

#include "osal_hal.h"

typedef void *osal_msg;

/**
 * OSAL_MSG_CHAN_MAX defines the amount of channels.
 * `osal_channel` must fit in the maximum of OSAL_MSG_CHAN_MAX
 *
 * A task can hold more than one channel, users can define the purpose of channels.
 * But one channel can only hold one msg until the msg has been read.
 */
#define OSAL_MSG_CHAN_MAX 32
typedef osal_uint8 osal_channel;

typedef enum
{
    osal_msg_success = 0, osal_msg_chan_busy,
} osal_msg_state;

/*
 * @description: Initialize osal message system
 */
void osal_msg_init (void);

/*
 * @description: write a msg(a pointer to strcuture or else) to a specific channel
 * @params:
 *        chan_id: the channel id. One channel can only hold one msg.
 *        msg: a pointer
 * @return:
 *        osal_msg_success: written successfully.
 *        osal_msg_chan_busy: the last msg has not been read, so you cannot write.
 */
osal_msg_state osal_msg_write (osal_channel chan_id, osal_msg msg);

/*
 * @description: overwrite a channel, ignore its current msg.
 * @params:
 *        chan_id: the channel id. One channel can only hold one msg.
 *        msg: a pointer
 */
void osal_msg_overwrite (osal_channel chan_id, osal_msg msg);

/*
 * @description: read a channel, and clear the channel.
 *               So this can be called only once.
 * @params:
 *        chan_id: the channel id you want to read.
 * @return:
 *        it returns the msg.
 */
osal_msg osal_msg_read (osal_channel chan_id);

/*
 * @description: read a channel, but not clear the channel.
 *               But you need to clear the channel by calling osal_msg_clear.
 * @params:
 *        chan_id: the channel id you want to read.
 * @return:
 *        it returns the msg.
 */
osal_msg osal_msg_query (osal_channel chan_id);

/*
 * @description: clear the channel
 * @params:
 *        chan_id: the channel id you want to clear.
 */
void osal_msg_clear (osal_channel chan_id);

#endif /* CODE_OSAL_MSG_H_ */
