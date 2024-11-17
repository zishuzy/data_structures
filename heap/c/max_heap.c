#include "max_heap.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(int *a, int *b)
{
    int t = *a;
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

static void heapifyup(struct maxheap *heap, int i)
{
    while (i > 0 && heap->array[parent(i)] < heap->array[i]) {
        swap(&heap->array[parent(i)], &heap->array[i]);
        i = parent(i);
    }
}

static void heapifydown(struct maxheap *heap, int i)
{
    int l = left(i);
    int r = right(i);
    int largest = i;
    if (l < heap->size && heap->array[l] > heap->array[largest]) {
        largest = l;
    }
    if (r < heap->size && heap->array[r] > heap->array[largest]) {
        largest = r;
    }
    if (largest != i) {
        swap(&heap->array[i], &heap->array[largest]);
        heapifydown(heap, largest);
    }
}

struct maxheap *maxheap_create(int capacity)
{
    struct maxheap *heap = malloc(sizeof(struct maxheap));
    if (heap == NULL) {
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (int *)malloc(sizeof(int) * capacity);
    if (heap->array == NULL) {
        free(heap);
        return NULL;
    }
    return heap;
}

void maxheap_destroy(struct maxheap *heap)
{
    free(heap->array);
    free(heap);
}

int maxheap_push(struct maxheap *heap, int value)
{
    if (heap->size == heap->capacity) {
        return 1;
    }
    heap->size++;
    heap->array[heap->size - 1] = value;
    heapifyup(heap, heap->size - 1);
    return 0;
}

int maxheap_pop(struct maxheap *heap, int *value)
{
    if (value == NULL) {
        return 1;
    }
    if (heap->size <= 0) {
        return 1;
    } else if (heap->size == 1) {
        *value = heap->array[0];
        heap->size--;
        return 0;
    }

    *value = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapifydown(heap, 0);
    return 0;
}

void maxheap_print(struct maxheap *heap)
{
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
}

#ifdef __TEST__
int main(int argc, char *argv[])
{
    struct maxheap *heap = maxheap_create(10);
    int i = 0;
    int array[10] = {3, 5, 9, 1, 4, 6, 2, 7, 8, 0};

    for (i = 0; i < 10; i++) {
        maxheap_push(heap, array[i]);
        printf("push: %d\n", array[i]);
        maxheap_print(heap);
    }
    maxheap_print(heap);
    for (i = 0; i < 10; i++) {
        int value = 0;
        maxheap_pop(heap, &value);
        printf("pop: %d\n", value);
        maxheap_print(heap);
    }
    maxheap_destroy(heap);

    return 0;
}
#endif
