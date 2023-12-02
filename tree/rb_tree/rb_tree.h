#ifndef UTILS_RB_TREE
#define UTILS_RB_TREE

#include <stdbool.h>
#include <stdint.h>

struct rbtree_root;

/**
 * @brief 红黑树初始化参数
 *
 */
struct rbtree_arg {
    int is_thread_safe;
    int (*cmp_key)(void *key0, void *key1);
    void *(*copy_key)(void *key);
    void (*free_key)(void *key);
    void *(*copy_value)(void *key);
    void (*free_value)(void *value);
};

/**
 * @brief 创建一个红黑树，返回红黑树句柄
 *
 * @param arg
 * @return struct rbtree_root*
 */
struct rbtree_root *rbtree_init(struct rbtree_arg arg);

/**
 * @brief 销毁一个红黑树
 *
 * @param root 由 rbtree_init 生成的红黑树句柄
 */
void rbtree_destroy(struct rbtree_root *root);

/**
 * @brief 插入一个节点
 *
 * @param root
 * @param key
 * @param value
 * @param key_copy true: 对 key 调用 copy_key 进行拷贝
 * @param val_copy true: 对 val 调用 copy_val 进行拷贝
 * @return int
 */
int rbtree_insert(struct rbtree_root *root, void *key, void *value, bool key_copy, bool val_copy);

/**
 * @brief 判断一个 key 是否存在于树中
 *
 * @param root
 * @param key
 * @return true
 * @return false
 */
bool rbtree_is_exist(struct rbtree_root *root, void *key);

/**
 * @brief 查看一个 key 对应的 value，如果没有返回 NULL
 *
 * @param root
 * @param key
 * @return void*
 */
void *rbtree_search(struct rbtree_root *root, void *key);

/**
 * @brief 删除一个节点
 *
 * @param root
 * @param key
 */
void rbtree_delete(struct rbtree_root *root, void *key);

/**
 * @brief 前序遍历
 *
 * @param root
 * @param cb
 */
void rbtree_preorder(struct rbtree_root *root, void (*cb)(void *key, void *value));

/**
 * @brief 中序遍历
 *
 * @param root
 * @param cb
 */
void rbtree_inorder(struct rbtree_root *root, void (*cb)(void *key, void *value));

/**
 * @brief 后序遍历
 *
 * @param root
 * @param cb
 */
void rbtree_postorder(struct rbtree_root *root, void (*cb)(void *key, void *value));

// void rbtree_levelorder(struct rbtree_root *root, void (*cb)(void *key, void *value));

void print_rbtree(struct rbtree_root *root);

#endif /* UTILS_RB_TREE */
