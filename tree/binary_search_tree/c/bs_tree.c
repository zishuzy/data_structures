/**
 * @file bs_tree.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Binary search tree implemented in C.
 * @version 0.1
 * @date 2023-12-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdlib.h>

#include "common/log/log.h"

#include "queue/c/queue.h"

#include "bs_tree.h"

static void bstree_print_helper(bstree_node_t *node, int depth, int *arr_flag,
                                void (*cb_print)(void *data))
{
    int i;

    for (i = 0; i < depth - 1; i++) {
        printf("%s", arr_flag[i] ? "│   " : "    ");
    }
    if (depth) {
        printf("%s", arr_flag[depth - 1] ? "├── " : "└── ");
    }

    if (node == NULL) {
        printf("(null)\n");
        return;
    }
    cb_print(node->val);
    printf("\n");
    // printf("%s\n", (char *)node->data);

    arr_flag[depth] = 1;
    bstree_print_helper(node->right, depth + 1, arr_flag, cb_print);
    arr_flag[depth] = 0;
    bstree_print_helper(node->left, depth + 1, arr_flag, cb_print);
}

bstree_node_t *bstree_node_create(void *key, uint32_t key_len, void *val, uint32_t val_len)
{
    bstree_node_t *node = malloc(sizeof(bstree_node_t));
    if (!node) {
        return NULL;
    }

    node->key = key;
    node->key_len = key_len;
    node->val = val;
    node->val_len = val_len;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void bstree_node_free(bstree_node_t *node,
                      void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                 void *ctx),
                      void *ctx)
{
    if (!node) {
        return;
    }
    cb(node->key, node->key_len, node->val, node->val_len, ctx);
    free(node);
}

void bstree_destroy(bstree_node_t *root,
                    void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len, void *ctx),
                    void *ctx)
{
    if (!root) {
        return;
    }
    bstree_destroy(root->left, cb, ctx);
    bstree_destroy(root->right, cb, ctx);
    bstree_node_free(root, cb, ctx);
}

int bstree_insert(bstree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
                  int (*less)(void *key_left, uint32_t key_left_len, void *key_right,
                              uint32_t key_right_len),
                  int (*upgrade)(bstree_node_t *old, void *key, uint32_t key_len, void *val,
                                 uint32_t val_len, void *ctx),
                  void *ctx)
{
    bstree_node_t *node;
    int rc = 0;
    if (!root) {
        return -1;
    }

    if (less(key, key_len, root->key, root->key_len)) {
        if (!root->left) {
            node = bstree_node_create(key, key_len, val, val_len);
            if (!node) {
                return -1;
            }
            root->left = node;
        } else {
            rc = bstree_insert(root->left, key, key_len, val, val_len, less);
        }
    } else if (less(root->key, root->key_len, key, key_len)) {
        if (!root->right) {
            node = bstree_node_create(key, key_len, val, val_len);
            if (!node) {
                return -1;
            }
            root->right = node;
        } else {
            rc = bstree_insert(root->right, key, key_len, val, val_len, less);
        }
    } else { // node == root
        if (upgrade && upgrade(root, key, key_len, val, val_len, ctx)) {
            root->key = key;
            root->val_len = val_len;
            root->val = val;
            root->val_len = val_len;
        } else {
            rc = -1;
        }
    }
    return rc;
}

int bstree_insert_node(bstree_node_t *root, bstree_node_t *node,
                       int (*less)(bstree_node_t *left, bstree_node_t *right))
{
    int rc = 0;
    if (!root || !node) {
        return -1;
    }

    if (less(node, root)) {
        if (!root->left) {
            root->left = node;
        } else {
            rc = bstree_insert_node(root->left, node, less);
        }
    } else if (less(root, node)) {
        if (!root->right) {
            root->right = node;
        } else {
            rc = bstree_insert_node(root->right, node, less);
        }
    } else { // node == root
        rc = -1;
    }
    return rc;
}

static int bstree_insert2_(bstree_node_t *root, bstree_node_t *parent, bstree_node_t *node,
                           int (*less)(bstree_node_t *left, bstree_node_t *right),
                           int (*upgrade)(bstree_node_t *old, bstree_node_t *node, void *ctx),
                           void *ctx)
{
    int rc = 0;

    if (less(node, root)) {
        if (!root->left) {
            root->left = node;
        } else {
            rc = bstree_insert2_(root->left, root, node, less, upgrade, ctx);
        }
    } else if (less(root, node)) {
        if (!root->right) {
            root->right = node;
        } else {
            rc = bstree_insert2_(root->right, root, node, less, upgrade, ctx);
        }
    } else { // node == root
        if (upgrade && upgrade(root, node, ctx)) {
            if (parent) {
                if (parent->left == root) {
                    parent->left = node;
                } else {
                    parent->right = node;
                }
                node->left = root->left;
                node->right = root->right;
            }

            root->key = node->key;
            root->val = node->val;
        }
    }
    return rc;
}

int bstree_insert_node2(bstree_node_t *root, bstree_node_t *node,
                        int (*less)(bstree_node_t *left, bstree_node_t *right),
                        int (*upgrade)(bstree_node_t *old, bstree_node_t *node, void *ctx),
                        int (*free)(bstree_node_t *old), void *ctx)
{
    int rc = 0;
    if (!root || !node) {
        return -1;
    }

    if (less(node, root)) {
        if (!root->left) {
            root->left = node;
        } else {
            rc = bstree_insert_node(root->left, node, less);
        }
    } else if (less(root, node)) {
        if (!root->right) {
            root->right = node;
        } else {
            rc = bstree_insert_node(root->right, node, less);
        }
    } else { // node == root
             // TODO: 替换 node 而非 data
        if (upgrade && upgrade(root, node, ctx)) {
            root->key = node->key;
            root->val = node->val;
        }
    }
    return rc;
}

uint32_t bstree_depth(bstree_node_t *root)
{
    uint32_t left_depth, right_depth;
    if (!root) {
        return 0;
    }
    left_depth = bstree_depth(root->left);
    right_depth = bstree_depth(root->right);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void bstree_preorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    if (cb) {
        cb(root, ctx);
    }
    bstree_preorder(root->left, cb, ctx);
    bstree_preorder(root->right, cb, ctx);
}

void bstree_inorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    bstree_inorder(root->left, cb, ctx);
    if (cb) {
        cb(root, ctx);
    }
    bstree_inorder(root->right, cb, ctx);
}

void bstree_postorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    bstree_postorder(root->left, cb, ctx);
    bstree_postorder(root->right, cb, ctx);
    if (cb) {
        cb(root, ctx);
    }
}

void bstree_levelorder(bstree_node_t *root, int (*cb)(bstree_node_t *node, void *ctx), void *ctx)
{
    queue_t *q;
    queue_node_t *qnode;
    if (!root) {
        return;
    }

    q = queue_create();
    if (!q) {
        return;
    }

    do {
        qnode = queue_node_create(root);
        if (!qnode) {
            break;
        }
        if (queue_enqueue(q, qnode) < 0) {
            queue_node_free(qnode);
            break;
        }

        while (!queue_is_empty(q)) {
            qnode = queue_dequeue(q);
            if (!qnode) {
                continue;
            }
            root = qnode->data;
            queue_node_free(qnode);

            if (cb(root, ctx)) {
                break;
            }

            if (root->left) {
                qnode = queue_node_create(root->left);
                queue_enqueue(q, qnode);
            }
            if (root->right) {
                qnode = queue_node_create(root->right);
                queue_enqueue(q, qnode);
            }
        }

    } while (0);

    queue_free(q, NULL, NULL);
}

void bstree_levelorder2(bstree_node_t *root, int (*cb)(bstree_node_t *node, int depth, void *ctx),
                        void *ctx)
{
    queue_t *q = NULL;
    queue_t *q1;
    queue_t *q2;
    queue_node_t *qnode;
    int depth = 0;

    if (!root) {
        return;
    }

    q1 = queue_create();
    if (!q1) {
        return;
    }
    q2 = queue_create();
    if (!q2) {
        queue_free(q1, NULL, NULL);
        return;
    }
    q = q1;

    do {
        qnode = queue_node_create(root);
        if (!qnode) {
            break;
        }
        if (queue_enqueue(q, qnode) < 0) {
            queue_node_free(qnode);
            break;
        }

        do {
            while (!queue_is_empty(q)) {
                qnode = queue_dequeue(q);
                if (!qnode) {
                    continue;
                }
                root = qnode->data;
                queue_node_free(qnode);

                if (cb(root, depth, ctx)) {
                    break;
                }

                if (root->left) {
                    qnode = queue_node_create(root->left);
                    queue_enqueue(q == q1 ? q2 : q1, qnode);
                }
                if (root->right) {
                    qnode = queue_node_create(root->right);
                    queue_enqueue(q == q1 ? q2 : q1, qnode);
                }
            }
            depth++;
            q = q == q1 ? q2 : q1;
        } while (!queue_is_empty(q));
    } while (0);

    queue_free(q1, NULL, NULL);
    queue_free(q2, NULL, NULL);
}

void bstree_print(bstree_node_t *root, void (*cb_print)(void *data))
{
    int *arr_flag = NULL;
    uint32_t depth = bstree_depth(root);
    if (depth == 0) {
        return;
    }

    arr_flag = calloc(depth, sizeof(uint32_t));
    if (!arr_flag) {
        return;
    }
    arr_flag[0] = 1;
    bstree_print_helper(root, 0, arr_flag, cb_print);

    free(arr_flag);
}
