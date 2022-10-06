/*
 * osal_queue.h
 *
 *  Created on: 2022.10.3
 *      Author: Felix
 */

#ifndef CODE_OSAL_QUEUE_H_
#define CODE_OSAL_QUEUE_H_

typedef struct _osal_queue_node
{
        void *value;
        struct _osal_queue_node *preNode;
        struct _osal_queue_node *nextNode;
} osal_queue_node;

void osal_queue_node_init (osal_queue_node *node, void *val);
void osal_queue_add_next (osal_queue_node *parent_node, osal_queue_node *node);
void osal_queue_add_pre (osal_queue_node *parent_node, osal_queue_node *node);
void osal_queue_add_line_tail (osal_queue_node *parent_node, osal_queue_node *node);
void osal_queue_add_line_head (osal_queue_node *parent_node, osal_queue_node *node);
void osal_queue_add_round_tail (osal_queue_node *parent_node, osal_queue_node *node);
void osal_queue_remove_node (osal_queue_node *node);
void osal_queue_remove_line_tail (osal_queue_node *node);
void osal_queue_remove_line_head (osal_queue_node *node);
void osal_queue_remove_line_all (osal_queue_node *node);
void osal_queue_convert_toline (osal_queue_node *head_node);
void osal_queue_convert_toround (osal_queue_node *node);
osal_queue_node* osal_queue_line_at (osal_queue_node *head_node, unsigned long pos);

#endif
