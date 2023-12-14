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
                                void (*cb_print)(bstree_node_t *node, void *ctx), void *ctx)
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
    cb_print(node, ctx);
    printf("\n");
    // printf("%s\n", (char *)node->data);

    arr_flag[depth] = 1;
    bstree_print_helper(node->right, depth + 1, arr_flag, cb_print, ctx);
    arr_flag[depth] = 0;
    bstree_print_helper(node->left, depth + 1, arr_flag, cb_print, ctx);
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
    node->parent = NULL;
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
    if (cb) {
        cb(node->key, node->key_len, node->val, node->val_len, ctx);
    }
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

bstree_node_t *
bstree_insert(bstree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
              int (*less)(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len))
{
    if (!root) {
        return bstree_node_create(key, key_len, val, val_len);
    }
    if (less(key, key_len, root->key, root->key_len)) {
        root->left = bstree_insert(root->left, key, key_len, val, val_len, less);
    } else if (less(root->key, root->key_len, key, key_len)) {
        root->right = bstree_insert(root->right, key, key_len, val, val_len, less);
    }

    return root;
}

bstree_node_t *
bstree_insert2(bstree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
               int (*less)(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len),
               int (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len, void *ctx),
               void *ctx)
{
    if (!root) {
        return bstree_node_create(key, key_len, val, val_len);
    }
    if (less(key, key_len, root->key, root->key_len)) {
        root->left = bstree_insert(root->left, key, key_len, val, val_len, less);
    } else if (less(root->key, root->key_len, key, key_len)) {
        root->right = bstree_insert(root->right, key, key_len, val, val_len, less);
    } else {
        if (cb(root->key, root->key_len, root->val, root->val_len, ctx)) {
            root->key = key;
            root->key_len = key_len;
            root->val = val;
            root->val_len = val_len;
        }
    }

    return root;
}

int bstree_is_exists(bstree_node_t *root, void *key, uint32_t key_len,
                     int (*less)(void *left_key, uint32_t left_len, void *right_key,
                                 uint32_t right_len))
{
    int rc = 0;
    if (!root) {
        return rc;
    }

    if (less(key, key_len, root->key, root->key_len)) {
        rc = bstree_is_exists(root->left, key, key_len, less);
    } else if (less(root->key, root->key_len, key, key_len)) {
        rc = bstree_is_exists(root->right, key, key_len, less);
    } else { // node == root
        rc = 1;
    }
    return rc;
}

bstree_node_t *bstree_find(bstree_node_t *root, void *key, uint32_t key_len,
                           int (*less)(void *left_key, uint32_t left_len, void *right_key,
                                       uint32_t right_len))
{
    bstree_node_t *fnode = NULL;
    if (!root) {
        return NULL;
    }
    if (less(key, key_len, root->key, root->key_len)) {
        fnode = bstree_find(root->left, key, key_len, less);
    } else if (less(root->key, root->key_len, key, key_len)) {
        fnode = bstree_find(root->right, key, key_len, less);
    } else { // node == root
        fnode = root;
    }

    return fnode;
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

void bstree_print(bstree_node_t *root, void (*cb_print)(bstree_node_t *node, void *ctx), void *ctx)
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
    bstree_print_helper(root, 0, arr_flag, cb_print, ctx);

    free(arr_flag);
}
