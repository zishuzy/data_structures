#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "queue_c.h"

void test_cb(void *data)
{
    printf("queue cb: %d[%08lx]\n", *(int *)data, (long)data);
}

void test_single()
{
    struct queue *q = create_queue(QUEUE_TRHEAD_UNSAFE);
    int tmp[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    void *data;
    int i;

    for (i = 0; i < 10; ++i) {
        queue_push(q, &tmp[i]);
        printf("queue push tmp: %d[%08lx], q_size[%d]\n", tmp[i], (long)&tmp[i], queue_num(q));
    }

    while (!queue_is_empty(q)) {
        data = queue_pop(q);
        printf("queue pop: %d[%08lx], q_size[%d]\n", *(int *)data, (long)data, queue_num(q));
    }

    for (i = 0; i < 10; ++i) {
        queue_push(q, &tmp[i]);
        printf("queue push tmp: %d[%08lx], q_size[%d]\n", tmp[i], (long)&tmp[i], queue_num(q));
    }
    destroy_queue(q, test_cb);
}

int _data_tmp[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

void *run1(void *arg)
{
    struct queue *q = (struct queue *)arg;
    int i;

    for (i = 0; i < 20; ++i) {
        queue_push(q, &_data_tmp[i]);
        printf("run1: queue push val: %d[%08lx], q_size[%d]\n", _data_tmp[i], (long)&_data_tmp[i],
               queue_num(q));
    }

    return NULL;
}

void *run2(void *arg)
{
    struct queue *q = (struct queue *)arg;
    void *data;
    while (!queue_is_empty(q)) {
        data = queue_pop(q);
        printf("run2: queue pop: %d[%08lx], q_size[%d]\n", *(int *)data, (long)data, queue_num(q));
    }
    return NULL;
}

void test_multi()
{
    pthread_t pth[20];
    void *(*runs[2])(void *) = {run1, run2};
    struct queue *q = create_queue(QUEUE_TRHEAD_SAFE);
    int i;

    for (i = 0; i < 20; i++) {
        pthread_create(&pth[i], NULL, runs[i % 2], q);
    }
}

int main(int argc, char *argv[])
{
    // test_single();
    test_multi();

    sleep(-1);
    return 0;
}
