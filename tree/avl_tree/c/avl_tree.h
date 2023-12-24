/**
 * @file avl_tree.h
 * @author zishu (zishuzy@gmail.com)
 * @brief AVL tree implemented in C.
 * @version 0.1
 * @date 2023-12-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef C_AVL_TREE
#define C_AVL_TREE

#include <stdint.h>

typedef struct avltree_node {
    struct avltree_node *parent;
    struct avltree_node *left;
    struct avltree_node *right;
    void *key;
    uint32_t key_len;
    void *val;
    uint32_t val_len;
} avltree_node_t;

/**
 * @brief Create a node for a avl tree.
 *
 * @param data
 * @return avltree_node_t*
 */
avltree_node_t *avltree_node_create(void *key, uint32_t key_len, void *val, uint32_t val_len);

/**
 * @brief Free the node of a avl tree.
 *
 * @param node
 */
void avltree_node_free(avltree_node_t *node,
                       void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                  void *ctx),
                       void *ctx);

/**
 * @brief Destroy the avl tree.
 *
 * @param root
 */
void avltree_destroy(avltree_node_t *root,
                     void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                void *ctx),
                     void *ctx);

/**
 * @brief Insert the node of a avl tree, if the node exists, return NULL.
 *
 * @param root
 * @param key
 * @param key_len
 * @param val
 * @param val_len
 * @param less
 * @return int On success, the root of the avl tree is retuned. On error, NULL is returned.
 */
avltree_node_t *
avltree_insert(avltree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
               int (*less)(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len));

/**
 * @brief Insert the node of a avl tree, if the node exists, the "cb" will be call.
 *
 * @param root
 * @param key
 * @param key_len
 * @param val
 * @param val_len
 * @param less
 * @param cb    If the function returns non-zero, it indicates that the old value will be
 *              replaced; otherwise, it indicates that the old value will not be replaced.
 * @param ctx
 * @return int On success, the root of the avl tree is retuned. On error, NULL is returned.
 */
avltree_node_t *
avltree_insert2(avltree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
                int (*less)(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len),
                int (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len, void *ctx),
                void *ctx);

/**
 * @brief Check if the node exists in the avl tree.
 *
 * @param root
 * @param key
 * @param key_len
 * @param less
 * @return int On exists, 1 is returned. On not exists, 0 is returned.
 */
int avltree_is_exists(avltree_node_t *root, void *key, uint32_t key_len,
                      int (*less)(void *left_key, uint32_t left_len, void *right_key,
                                  uint32_t right_len));

/**
 * @brief Find the node in the avl tree.
 *
 * @param root
 * @param key
 * @param key_len
 * @param less
 * @return avltree_node_t* On success, the node is returned. On error, NULL is returned.
 */
avltree_node_t *avltree_find(avltree_node_t *root, void *key, uint32_t key_len,
                             int (*less)(void *left_key, uint32_t left_len, void *right_key,
                                         uint32_t right_len));

/**
 * @brief Get the depth for the avl tree.
 *
 * @param root
 * @return uint32_t
 */
uint32_t avltree_depth(avltree_node_t *root);

/**
 * @brief Preorder traverse the avl tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void avltree_preorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Inorder traverse the avl tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void avltree_inorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Postorder traverse the avl tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void avltree_postorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Levelorder traverse the avl tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void avltree_levelorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx),
                        void *ctx);

/**
 * @brief Levelorder traverse the avl tree, including the depth info.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void avltree_levelorder2(avltree_node_t *root,
                         int (*cb)(avltree_node_t *node, int depth, void *ctx), void *ctx);

/**
 * @brief Print the avl tree.
 *
 * @param root
 * @param cb_print
 * @param ctx
 */
void avltree_print(avltree_node_t *root, void (*cb_print)(avltree_node_t *node, void *ctx),
                   void *ctx);

#endif /* C_AVL_TREE */
