/*
 * osal_evt.h
 *
 *  Created on: 2022.10.3
 *      Author: Felix
 */

#ifndef CODE_OSAL_EVT_H_
#define CODE_OSAL_EVT_H_

#include "osal_hal.h"

/*
 * OSAL_EVENT_MAX defines the amount of events.
 * The smaller of task_id, the priority is higher.
 *
 * `osal_task_id` should hold the maximum of OSAL_EVENT_MAX
 */

#define OSAL_EVENT_MAX            32   //8*n
typedef osal_uint8 osal_evt_id;
typedef void (*osal_evt_func) (osal_evt_id);

#define OSAL_ENABLE_AUTO_RESET    0    //1: enable auto-reset   0: disbale auto-reset

typedef enum
{
    osal_evt_success = 0, osal_evt_hasRegistered, osal_evt_hasNotRegistered
} osal_evt_result;

/*
 * @description: Initialize the event system.
 */
void osal_evt_init (void);

/*
 * @description: OSAL Run. This function will dispatch events.
 */
void osal_evt_run (void);

/*
 * @description: register a event. Bind a function with a task id.
 * @params:
 *        id: the task id.
 *        task_func: the event handler function.
 * @return:
 *        osal_evt_success: register successfully.
 *        osal_evt_hasRegistered: the current id has been used.
 */
osal_evt_result osal_evt_register (osal_evt_id id, osal_evt_func task_func);

/*
 * @description: unregister a event.
 * @params:
 *        id: the task id you want to unregister.
 * @return:
 *        osal_evt_success: unregister successfully.
 *        osal_evt_hasNotRegistered: the current id has not been used.
 */
osal_evt_result osal_evt_unregister (osal_evt_id id);

/*
 * @description: set a event happened.
 * @params:
 *         id: the event id you want to set.
 */
osal_evt_result osal_evt_set (osal_evt_id id);

/*
 * @description: reset a event.
 *               In the event handler function should call this function to clear event.
 * @params:
 *         id: the event id you want to reset.
 */
osal_evt_result osal_evt_reset (osal_evt_id id);

#endif /* CODE_OSAL_EVT_H_ */
