/**
 * @file bs_tree.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Binary search tree implemented in C.
 * @version 0.1
 * @date 2023-12-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef C_BS_TREE
#define C_BS_TREE

#include <stdint.h>

typedef struct bstree_node {
    struct bstree_node *parent;
    struct bstree_node *left;
    struct bstree_node *right;
    void *key;
    uint32_t key_len;
    void *val;
    uint32_t val_len;
} bstree_node_t;

/**
 * @brief Create a node for a binary search tree.
 *
 * @param data
 * @return bstree_node_t*
 */
bstree_node_t *bstree_node_create(void *key, uint32_t key_len, void *val, uint32_t val_len);

/**
 * @brief Free the node of a binary search tree.
 *
 * @param node
 */
void bstree_node_free(bstree_node_t *node,
                      void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                 void *ctx),
                      void *ctx);

/**
 * @brief Destroy the binary search tree.
 *
 * @param root
 */
void bstree_destroy(bstree_node_t *root,
                    void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len, void *ctx),
                    void *ctx);

int bstree_insert(bstree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
                  int (*less)(void *left_key, uint32_t left_key_len, void *right_key,
                              uint32_t right_key_len));

int bstree_insert2(bstree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
                   int (*less)(void *left_key, uint32_t left_key_len, void *right_key,
                               uint32_t right_key_len),
                   void (*cb_free)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                   void *ctx),
                   void *ctx);

int bstree_is_exists(bstree_node_t *root, void *key, uint32_t key_len,
                     int (*less)(void *left_key, uint32_t left_key_len, void *right_key,
                                 uint32_t right_key_len));

/**
 * @brief Get the depth for the binary search tree.
 *
 * @param root
 * @return uint32_t
 */
uint32_t bstree_depth(bstree_node_t *root);

/**
 * @brief Preorder traverse the binary search tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void bstree_preorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Inorder traverse the binary search tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void bstree_inorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Postorder traverse the binary search tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void bstree_postorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Levelorder traverse the binary search tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void bstree_levelorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Levelorder traverse the binary search tree, including the depth info.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void bstree_levelorder2(bstree_node_t *root, int (*cb)(bstree_node_t *node, int depth, void *ctx),
                        void *ctx);

/**
 * @brief Print the binary search tree.
 *
 * @param root
 * @param cb_print
 */
void bstree_print(bstree_node_t *root,
                  void (*cb_print)(void *key, uint32_t key_len, void *val, uint32_t val_len));

#endif /* C_BS_TREE */
