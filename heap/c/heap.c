#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(void **a, void **b)
{
    void *t = *a;
    *a = *b;
    *b = t;
}

static int parent(int i)
{
    return (i - 1) / 2;
}

static int left(int i)
{
    return 2 * i + 1;
}

static int right(int i)
{
    return 2 * i + 2;
}

static void heapifyup(struct heap *heap, int i)
{
    while (i > 0 && heap->lt(heap->array[parent(i)], heap->array[i])) {
        swap(&heap->array[parent(i)], &heap->array[i]);
        i = parent(i);
    }
}

static void heapifydown(struct heap *heap, int i)
{
    int l = left(i);
    int r = right(i);
    int largest = i;
    if (l < heap->size && heap->lt(heap->array[largest], heap->array[l])) {
        largest = l;
    }
    if (r < heap->size && heap->lt(heap->array[largest], heap->array[r])) {
        largest = r;
    }
    if (largest != i) {
        swap(&heap->array[i], &heap->array[largest]);
        heapifydown(heap, largest);
    }
}

static int heap_resize(struct heap *h)
{
    int capacity = h->capacity * 2;
    void *arrry = NULL;
    if (capacity < 0) { // 溢出了
        return -1;
    }
    h->capacity = capacity;
    arrry = (void *)realloc(h->array, h->capacity * sizeof(void *));
    if (!arrry) {
        return -1;
    }
    h->array = arrry;
    return 0;
}

struct heap *heap_create(int capacity, int (*lt)(void *l, void *r))
{
    struct heap *heap = malloc(sizeof(struct heap));
    if (!heap) {
        return NULL;
    }
    heap->array = (void *)malloc(sizeof(void *) * capacity);
    if (!heap->array) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->lt = lt;
    return heap;
}

void heap_destroy(struct heap *heap, void (*cb)(void *data, void *ctx), void *ctx)
{
    if (!heap) {
        return;
    }
    for (int i = 0; i < heap->size; i++) {
        cb(heap->array[i], ctx);
    }
    free(heap->array);
    free(heap);
}

int heap_push(struct heap *heap, void *data)
{
    if (!heap) {
        return -1;
    }
    if (heap->size == heap->capacity && heap_resize(heap)) {
        // 扩容失败
        return -1;
    }
    heap->size++;
    heap->array[heap->size - 1] = data;
    heapifyup(heap, heap->size - 1);
    return 0;
}

int heap_top(struct heap *heap, void **data)
{
    if (!heap || !data) {
        return -1;
    }
    if (heap->size <= 0) {
        return -1;
    }
    *data = heap->array[0];
    return 0;
}

int heap_pop(struct heap *heap, void **data)
{
    if (!heap || !data) {
        return -1;
    }
    if (heap->size <= 0) {
        return -1;
    } else if (heap->size == 1) {
        *data = heap->array[0];
        heap->size--;
        return 0;
    }
    *data = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapifydown(heap, 0);
    return 0;
}

void heap_walk(struct heap *heap, void (*cb)(void *data, void *ctx), void *ctx)
{
    if (!heap) {
        return;
    }
    for (int i = 0; i < heap->size; i++) {
        cb(heap->array[i], ctx);
    }
}

#ifdef __TEST__
typedef struct heap_data {
    int value;
} heap_data_t;

int heap_data_lt(void *l, void *r)
{
    return ((heap_data_t *)l)->value < ((heap_data_t *)r)->value;
}

void heap_data_print(void *data, void *ctx)
{
    printf("%d ", ((heap_data_t *)data)->value);
}

void heap_data_free(void *data, void *ctx)
{
    free(data);
}

int main(int argc, char *argv[])
{
    struct heap *heap = heap_create(10, heap_data_lt);
    int i = 0;
    int array[10] = {3, 5, 9, 1, 4, 6, 2, 7, 8, 0};

    for (i = 0; i < 10; i++) {
        heap_data_t *data = malloc(sizeof(heap_data_t));
        data->value = array[i];
        heap_push(heap, data);
        printf("\npush: %d\n", data->value);
        heap_walk(heap, (void (*)(void *, void *))heap_data_print, NULL);
    }
    printf("\n");
    heap_walk(heap, (void (*)(void *, void *))heap_data_print, NULL);
    printf("\n");
    for (i = 0; i < 10; i++) {
        void *data = NULL;
        heap_pop(heap, &data);
        heap_data_print(data, NULL);
        printf("\npop: %d\n", ((heap_data_t *)data)->value);
        heap_walk(heap, (void (*)(void *, void *))heap_data_print, NULL);
        heap_data_free(data, NULL);
    }
    printf("\n");
    heap_destroy(heap, (void (*)(void *, void *))heap_data_free, NULL);
    return 0;
}
#endif
