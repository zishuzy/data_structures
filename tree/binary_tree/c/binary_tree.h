/**
 * @file binary_tree.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Binary tree implemented in C.
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef C_BINARY_TREE
#define C_BINARY_TREE

#include <stdint.h>

typedef struct btree_node {
    struct btree_node *left;
    struct btree_node *right;
    void *data;
    uint32_t len;
} btree_node_t;

/**
 * @brief Create a node for a binary tree.
 *
 * @param data
 * @return btree_node_t*
 */
btree_node_t *btree_node_create(void *data, uint32_t len);

/**
 * @brief Free the node of a binary tree.
 *
 * @param node
 */
void btree_node_free(btree_node_t *node);

/**
 * @brief Destroy the binary tree.
 *
 * @param root
 */
void btree_destroy(btree_node_t *root);

/**
 * @brief Get the depth for the binary tree.
 *
 * @param root
 * @return uint32_t
 */
uint32_t btree_depth(btree_node_t *root);

/**
 * @brief Preorder traverse the binary tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void btree_preorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Inorder traverse the binary tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void btree_inorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Postorder traverse the binary tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void btree_postorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Levelorder traverse the binary tree.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void btree_levelorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

/**
 * @brief Levelorder traverse the binary tree, including the depth info.
 *
 * @param root
 * @param cb
 * @param ctx
 */
void btree_levelorder2(btree_node_t *root, int (*cb)(btree_node_t *node, int depth, void *ctx),
                       void *ctx);

/**
 * @brief Print the binary tree.
 *
 * @param root
 * @param cb_print
 */
void btree_print(btree_node_t *root, void (*cb_print)(void *data));

#endif /* C_BINARY_TREE */
