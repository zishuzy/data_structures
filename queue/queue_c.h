#ifndef QUEUE_QUEUE_C
#define QUEUE_QUEUE_C

#include <stdbool.h>
#include <stdint.h>

struct queue;
struct queue_node;

#define QUEUE_TRHEAD_UNSAFE (0)
#define QUEUE_TRHEAD_SAFE   (1)

/**
 * @brief 创建队列
 *
 * @param flag QUEUE_TRHEAD_UNSAFE（线程不安全）/QUEUE_TRHEAD_SAFE（线程安全）
 * @return struct queue*
 */
struct queue *create_queue(int flag);

/**
 * @brief 销毁队列
 *
 * @param q
 * @param cb 回调函数中不要调用本队列的操作函数，会有问题
 */
void destroy_queue(struct queue *q, void (*cb)(void *data));

/**
 * @brief 向队列中添加数据
 *
 * @param q
 * @param data
 * @return true
 * @return false
 */
bool queue_push(struct queue *q, void *data);

/**
 * @brief 从队列中弹出一个元素
 *
 * @param q
 * @return void* 返回的数据就是 queue_push 中传入的 data
 */
void *queue_pop(struct queue *q);

/**
 * @brief 判断一个队列是否是空
 *
 * @param q
 * @return true
 * @return false
 */
bool queue_is_empty(struct queue *q);

/**
 * @brief 获取一个队列中元素个数
 *
 * @param q
 * @return int
 */
int queue_num(struct queue *q);

#endif /* QUEUE_QUEUE_C */
