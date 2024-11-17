#ifndef __C_MAX_HEAP__
#define __C_MAX_HEAP__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct maxheap {
    int size;
    int capacity;
    int *array;
};

struct maxheap *maxheap_create(int capacity);

void maxheap_destroy(struct maxheap *heap);

int maxheap_push(struct maxheap *heap, int value);

int maxheap_pop(struct maxheap *heap, int *value);

void maxheap_print(struct maxheap *heap);

#ifdef __cplusplus
}
#endif

#endif /* __C_MAX_HEAP__ */
