/**
 * @file queue_test.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Test queue implemented in C.
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "queue.h"

#include "common/log/log.h"

int main(void)
{
    queue_node_t *node;
    queue_t *q;
    long i;

    q = queue_create();
    if (!q) {
        LOG_ERROR("Failed to create queue!");
        return 1;
    }
    for (i = 0; i < 10; i++) {
        node = queue_create_node((void *)i);
        if (!node) {
            LOG_ERROR("Failed to create node of the queue! i[%ld]", i);
            continue;
        }

        if (queue_enqueue(q, node) < 0) {
            LOG_ERROR("Failed to enqueue node to the queue! i[%ld]", i);
            queue_free_node(node);
        } else {
            LOG_INFO("Successfully enqueue the node. i[%ld]", i);
        }

        LOG_INFO("The size of the queue is %u", queue_size(q));

        node = queue_front(q);
        if (!node) {
            LOG_ERROR("Failed to get front node from the queue!");
        } else {
            LOG_INFO("Get the front node from the queue! node->data[%ld]", (long)node->data);
        }
    }

    while (!queue_is_empty(q)) {
        node = queue_dequeue(q);
        if (!node) {
            LOG_ERROR("Failed to dequeue node from the queue!");
            break;
        } else {
            LOG_INFO("Dequeue the node from the queue. node->data[%ld]", (long)node->data);
        }
        LOG_INFO("The size of the queue is %u", queue_size(q));
        queue_free_node(node);
    }

    queue_free(q, NULL, NULL);

    return 0;
}