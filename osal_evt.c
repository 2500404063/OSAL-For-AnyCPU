/*
 * osal_evt.c
 *
 *  Created on: 2022.10.3
 *      Author: Felix
 */

#include "osal_evt.h"

static osal_uint8 event_state[OSAL_EVENT_MAX / 8];
static osal_evt_func event_func[OSAL_EVENT_MAX];

static osal_uint8 event_system_flag;
#define OSAL_EVENT_SYSTEM_FLAG_INITED 0x1

void osal_evt_init(void)
{
    for (osal_evt_id i = 0; i < OSAL_EVENT_MAX / 8; ++i)
    {
        event_state[i] = 0x0;
    }
    for (osal_evt_id i = 0; i < OSAL_EVENT_MAX; ++i)
    {
        event_func[i] = 0x0;
    }
    event_system_flag |= OSAL_EVENT_SYSTEM_FLAG_INITED;
}

void osal_evt_run(void)
{
    if (!(event_system_flag & OSAL_EVENT_SYSTEM_FLAG_INITED))
    {
        osal_evt_init();
    }

    while (1)
    {
        for (osal_evt_id i = 0; i < OSAL_EVENT_MAX; ++i)
        {
            if (event_state[i / 8] & 1 << (i % 8))
            {
                event_func[i](i);
#if (OSAL_ENABLE_AUTO_RESET == 1)
                osal_event_reset(i);
#endif
                // use break instead of continue: to keep priority.
                break;
            }
        }
    }
}

osal_evt_result osal_evt_register(osal_evt_id id, osal_evt_func task_func)
{
    if (event_func[id] == 0x0)
    {
        event_func[id] = task_func;
        return osal_evt_success;
    }
    else
    {
        return osal_evt_hasRegistered;
    }
}

osal_evt_result osal_evt_unregister(osal_evt_id id)
{
    if (event_func[id] == 0x0)
    {
        return osal_evt_hasNotRegistered;
    }
    else
    {
        event_func[id] = 0x0;
        return osal_evt_success;
    }
}

osal_evt_result osal_evt_set(osal_evt_id id)
{
    if (event_func[id] == 0x0)
    {
        return osal_evt_hasNotRegistered;
    }
    else
    {
        osal_evt_id index = id / 8;
        osal_uint8 bias = id % 8;
        event_state[index] |= 1 << bias;
        return osal_evt_success;
    }
}

osal_evt_result osal_evt_reset(osal_evt_id id)
{
    osal_evt_id index = id / 8;
    osal_uint8 bias = id % 8;
    event_state[index] &= ~(1 << bias);
    return osal_evt_success;
}
