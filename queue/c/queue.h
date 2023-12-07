/**
 * @file queue.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Queue implemented in C.
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef C_QUEUE
#define C_QUEUE

#include <stdint.h>

typedef struct queue_node {
    struct queue_node *next;
    void *data;
} queue_node_t;

typedef struct queue {
    queue_node_t *front;
    queue_node_t *tail;
    uint32_t size;
} queue_t;

/**
 * @brief Create a queue.
 *
 * @return queue_t*
 */
queue_t *queue_create(void);

/**
 * @brief Free the queue.
 *
 * @param q
 * @param cb
 * @param ctx
 */
void queue_free(queue_t *q, void (*cb)(queue_node_t *node, void *ctx), void *ctx);

/**
 * @brief Create a node for a queue.
 *
 * @param data
 * @return queue_node_t*
 */
queue_node_t *queue_node_create(void *data);

/**
 * @brief Free the node of queue.
 *
 * @param node
 */
void queue_node_free(queue_node_t *node);

/**
 * @brief Enqueue the node into the queue.
 *
 * @param q
 * @param node
 * @return int On success, 0 is retuned. On error, -1 is returned.
 */
int queue_enqueue(queue_t *q, queue_node_t *node);

/**
 * @brief queue_enqueue
 *
 * @param q
 * @param node
 * @return int On success, 0 is retuned. On error, -1 is returned.
 */
int queue_push(queue_t *q, queue_node_t *node);

/**
 * @brief Dequeue a node from the queue.
 *
 * @param q
 * @return queue_node_t*
 */
queue_node_t *queue_dequeue(queue_t *q);

/**
 * @brief queue_dequeue
 *
 * @param q
 * @return queue_node_t*
 */
queue_node_t *queue_pop(queue_t *q);

/**
 * @brief Get the front node of the queue.
 *
 * @param q
 * @return queue_node_t*
 */
queue_node_t *queue_front(queue_t *q);

/**
 * @brief Check if the queue is empty.
 *
 * @param q
 * @return int On error, -1 is returned. On success, return 1 if not empty, 0 if empty.
 */
int queue_is_empty(queue_t *q);

/**
 * @brief Get the size of the queue.
 *
 * @param q
 * @return uint32_t
 */
uint32_t queue_size(queue_t *q);

#endif /* C_QUEUE */
