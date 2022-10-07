/*
 * osal_msg.c
 *
 *  Created on: 2022.10.3
 *      Author: Felix
 */

#include "osal_msg.h"

static osal_msg osal_msgs[OSAL_MSG_CHAN_MAX];

void osal_msg_init (void)
{
    for (int i = 0; i < OSAL_MSG_CHAN_MAX; ++i)
    {
        osal_msgs[i] = 0x0;
    }
}

osal_msg_state osal_msg_write (osal_channel chan_id, osal_msg msg)
{
    if (osal_msgs[chan_id] == 0x0)
    {
        osal_msgs[chan_id] = msg;
        return osal_msg_success;
    }
    else
    {
        return osal_msg_chan_busy;
    }
}

void osal_msg_overwrite (osal_channel chan_id, osal_msg msg)
{
    osal_msgs[chan_id] = msg;
}

osal_msg osal_msg_read (osal_channel chan_id)
{
    osal_msg t = osal_msgs[chan_id];
    osal_msgs[chan_id] = 0x0;
    return t;
}

osal_msg osal_msg_query (osal_channel chan_id)
{
    return osal_msgs[chan_id];
}

void osal_msg_clear (osal_channel chan_id)
{
    osal_msgs[chan_id] = 0x0;
}
