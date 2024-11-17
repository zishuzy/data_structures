/**
 * @file heap.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Heap implemented in C.
 * @version 0.1
 * @date 2024-11-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __C_HEAP__
#define __C_HEAP__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct heap {
    int size;
    int capacity;
    int (*lt)(void *l, void *r);
    void **array;
} heap_t;

/**
 * @brief Create a heap.
 *
 * @param capacity The capacity of the heap
 * @param lt       Less than function
 * @return heap_t*
 */
heap_t *heap_create(int capacity, int (*lt)(void *l, void *r));

/**
 * @brief Destroy a heap.
 *
 * @param heap  The heap.
 * @param cb    The callback function.
 * @param ctx   The context.
 */
void heap_destroy(heap_t *heap, void (*cb)(void *data, void *ctx), void *ctx);

/**
 * @brief Push a data to the heap.
 *
 * @param heap The heap.
 * @param data The data.
 * @return int Return 0 on success, -1 on failure.
 */
int heap_push(heap_t *heap, void *data);

/**
 * @brief Get the top data of the heap.
 *
 * @param heap The heap.
 * @param data The data.
 * @return int Return 0 on success, -1 on failure.
 */
int heap_top(heap_t *heap, void **data);

/**
 * @brief Pop the top data of the heap.
 *
 * @param heap The heap.
 * @param data The data.
 * @return int Return 0 on success, -1 on failure.
 */
int heap_pop(heap_t *heap, void **data);

/**
 * @brief Walk the heap.
 *
 * @param heap The heap.
 * @param cb   The callback function.
 * @param ctx  The context.
 */
void heap_walk(heap_t *heap, void (*cb)(void *data, void *ctx), void *ctx);

#ifdef __cplusplus
}
#endif

#endif /* __C_HEAP__ */
