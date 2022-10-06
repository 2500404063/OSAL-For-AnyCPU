#include "osal_queue.h"

void osal_queue_node_init (osal_queue_node *node, void *val)
{
    node->nextNode = node;
    node->preNode = node;
    node->value = val;
}

void osal_queue_add_next (osal_queue_node *parent_node, osal_queue_node *node)
{
    osal_queue_node *parent_node_next = parent_node->nextNode;
    parent_node->nextNode = node;
    node->preNode = parent_node;
    if (parent_node_next != parent_node)
    {
        node->nextNode = parent_node_next;
        parent_node_next->preNode = node;
    }
}

void osal_queue_add_pre (osal_queue_node *parent_node, osal_queue_node *node)
{
    osal_queue_node *parent_node_pre = parent_node->preNode;
    parent_node->preNode = node;
    node->nextNode = parent_node;
    if (parent_node_pre != parent_node)
    {
        node->preNode = parent_node_pre;
        parent_node_pre->nextNode = node;
    }
}

void osal_queue_add_line_tail (osal_queue_node *parent_node, osal_queue_node *node)
{
    while (parent_node->nextNode != parent_node)
    {
        parent_node = parent_node->nextNode;
    }
    osal_queue_add_next(parent_node, node);
}

void osal_queue_add_line_head (osal_queue_node *parent_node, osal_queue_node *node)
{
    while (parent_node->preNode != parent_node)
    {
        parent_node = parent_node->preNode;
    }
    osal_queue_add_pre(parent_node, node);
}

void osal_queue_add_round_tail (osal_queue_node *parent_node, osal_queue_node *node)
{
    osal_queue_node *parent_node_pre = parent_node->preNode;
    parent_node->preNode = node;
    node->nextNode = parent_node;
    node->preNode = parent_node_pre;
    parent_node_pre->nextNode = node;
}

void osal_queue_remove_node (osal_queue_node *node)
{
    if (node->nextNode == node)
    {
        node->preNode->nextNode = node->preNode;
        node->preNode = node;
    }
    else if (node->preNode == node)
    {
        node->nextNode->preNode = node->nextNode;
        node->nextNode = node;
    }
    else
    {
        node->preNode->nextNode = node->nextNode;
        node->nextNode->preNode = node->preNode;
        node->preNode = node;
        node->nextNode = node;
    }
}

void osal_queue_remove_line_tail (osal_queue_node *node)
{
    while (node->nextNode != node)
    {
        node = node->nextNode;
    }
    node->preNode->nextNode = node->preNode;
    node->preNode = node;
}

void osal_queue_remove_line_head (osal_queue_node *node)
{
    while (node->preNode != node)
    {
        node = node->preNode;
    }
    node->nextNode->preNode = node->nextNode;
    node->nextNode = node;
}

void osal_queue_remove_line_all (osal_queue_node *node)
{
    while (node->preNode != node)
    {
        node = node->preNode;
    }
    while (node->nextNode != node)
    {
        osal_queue_node *nextNode = node->nextNode;
        nextNode->preNode = nextNode;
        node->nextNode = node;
        node = nextNode;
    }
}

void osal_queue_convert_toline (osal_queue_node *head_node)
{
    head_node->preNode->nextNode = head_node->preNode;
    head_node->preNode = head_node;
}

void osal_queue_convert_toround (osal_queue_node *node)
{
    osal_queue_node *head_node = node;
    osal_queue_node *tail_node = node;
    while (head_node->preNode != head_node)
    {
        head_node = head_node->preNode;
    }
    while (tail_node->nextNode != tail_node)
    {
        tail_node = tail_node->nextNode;
    }
    head_node->preNode = tail_node;
    tail_node->nextNode = head_node;
}

osal_queue_node* osal_queue_line_at (osal_queue_node *head_node, unsigned long pos)
{
    for (int i = 0; i < pos; ++i)
    {
        head_node = head_node->nextNode;
    }
    return head_node;
}

