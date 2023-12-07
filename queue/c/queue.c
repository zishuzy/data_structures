/**
 * @file queue.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Queue implemented in C.
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "queue.h"

#include <stdlib.h>

queue_t *queue_create(void)
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return q;
    }

    q->size = 0;
    q->front = NULL;
    q->tail = NULL;

    return q;
}

void queue_free(queue_t *q, void (*cb)(queue_node_t *node, void *ctx), void *ctx)
{
    if (!q) {
        return;
    }
    queue_node_t *head = q->front;
    queue_node_t *node;
    for (node = head; node != NULL; node = head) {

        if (cb) {
            cb(node, ctx);
        }
        head = node->next;
        free(node);
    }
    free(q);
}

queue_node_t *queue_create_node(void *data)
{
    queue_node_t *node = malloc(sizeof(queue_node_t));
    if (!node) {
        return node;
    }

    node->data = data;
    node->next = NULL;

    return node;
}

void queue_free_node(queue_node_t *node)
{
    if (!node) {
        return;
    }
    free(node);
}

int queue_enqueue(queue_t *q, queue_node_t *node)
{
    if (!q || !node) {
        return -1;
    }

    if (q->front == NULL) {
        q->front = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    node->next = NULL;
    q->size++;

    return 0;
}

int queue_push(queue_t *q, queue_node_t *node)
{
    return queue_enqueue(q, node);
}

queue_node_t *queue_dequeue(queue_t *q)
{
    queue_node_t *node;
    if (!q || q->size == 0) {
        return NULL;
    }

    node = q->front;
    q->front = node->next;
    node->next = NULL;
    if (q->tail == node) {
        q->tail = NULL;
    }
    q->size--;

    return node;
}

queue_node_t *queue_pop(queue_t *q)
{
    return queue_dequeue(q);
}

queue_node_t *queue_front(queue_t *q)
{
    if (!q) {
        return NULL;
    }
    return q->front;
}

int queue_is_empty(queue_t *q)
{
    if (!q) {
        return -1;
    }

    return q->size == 0 ? 1 : 0;
}

uint32_t queue_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}
