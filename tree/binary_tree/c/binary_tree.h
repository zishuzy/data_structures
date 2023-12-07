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

typedef struct btree_node {
    struct btree_node *left;
    struct btree_node *right;
    void *data;
} btree_node_t;

btree_node_t *btree_node_create(void *data);

void btree_node_free(btree_node_t *node);

void btree_preorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

void btree_inorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

void btree_postorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

void btree_levelorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx);

#endif /* C_BINARY_TREE */
