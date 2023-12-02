#include "queue_c.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct queue_node {
    struct queue_node *next;
    void *data;
} queue_node_t;

typedef struct queue {
    queue_node_t *front; //指向对头节点
    queue_node_t *tail;  //指向队尾节点
    int num;
    int flag;
    pthread_spinlock_t locker;
} queue_t;

#define LOCK_QUEUE(q)                       \
    {                                       \
        if ((q)->flag == QUEUE_TRHEAD_SAFE) \
            pthread_spin_lock(&q->locker);  \
    }
#define UNLOCK_QUEUE(q)                      \
    {                                        \
        if ((q)->flag == QUEUE_TRHEAD_SAFE)  \
            pthread_spin_unlock(&q->locker); \
    }

queue_t *create_queue(int flag)
{
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->front = NULL;
    q->tail = NULL;
    q->num = 0;
    q->flag = flag;

    if (flag == QUEUE_TRHEAD_SAFE) {
        if (pthread_spin_init(&q->locker, PTHREAD_PROCESS_PRIVATE) != 0) {
            goto err0;
        }
    }
    return q;
err0:
    free(q);
    return NULL;
}

void destroy_queue(struct queue *q, void (*cb)(void *data))
{
    queue_node_t *node;

    LOCK_QUEUE(q);
    while (q->front != NULL) {
        node = q->front;
        if (q->front == q->tail) {
            q->front = NULL;
            q->tail = NULL;
        } else {
            q->front = q->front->next;
        }
        if (cb) {
            cb(node->data);
        }
        free(node);
    }
    q->num = 0;
    UNLOCK_QUEUE(q);
    free(q);
}

bool queue_push(struct queue *q, void *data)
{
    queue_node_t *node;

    if ((node = malloc(sizeof(queue_node_t))) == NULL) {
        return false;
    }
    node->data = data;
    node->next = NULL;

    LOCK_QUEUE(q);
    if (q->front == NULL) {
        q->front = node;
    }
    if (q->tail == NULL) {
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->num++;
    UNLOCK_QUEUE(q);
    return true;
}

void *queue_pop(struct queue *q)
{
    queue_node_t *node;
    void *data = NULL;

    LOCK_QUEUE(q);
    if (q->front == NULL) {
        goto out;
    }

    node = q->front;
    if (q->front == q->tail) {
        q->front = NULL;
        q->tail = NULL;
    } else {
        q->front = q->front->next;
    }
    data = node->data;
    free(node);
    q->num--;
out:
    UNLOCK_QUEUE(q);
    return data;
}

bool queue_is_empty(struct queue *q)
{
    bool rc;
    LOCK_QUEUE(q);
    rc = q->front == NULL;
    UNLOCK_QUEUE(q);
    return rc;
}

int queue_num(struct queue *q)
{
    int num;
    LOCK_QUEUE(q);
    num = q->num;
    UNLOCK_QUEUE(q);
    return num;
}
