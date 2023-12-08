/**
 * @file binary_tree.c
 * @author zishu (zishuzy@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdlib.h>

#include "common/log/log.h"

#include "queue/c/queue.h"

#include "binary_tree.h"

static void btree_print_helper(btree_node_t *node, int depth, int *arr_flag,
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
    cb_print(node->data);
    printf("\n");
    // printf("%s\n", (char *)node->data);

    arr_flag[depth] = 1;
    btree_print_helper(node->right, depth + 1, arr_flag, cb_print);
    arr_flag[depth] = 0;
    btree_print_helper(node->left, depth + 1, arr_flag, cb_print);
}

btree_node_t *btree_node_create(void *data)
{
    btree_node_t *node = malloc(sizeof(btree_node_t));
    if (!node) {
        return NULL;
    }

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void btree_node_free(btree_node_t *node)
{
    if (!node) {
        return;
    }
    free(node);
}

void btree_destroy(btree_node_t *root)
{
    if (!root) {
        return;
    }
    btree_destroy(root->left);
    btree_destroy(root->right);
    btree_node_free(root);
}

uint32_t btree_depth(btree_node_t *root)
{
    uint32_t left_depth, right_depth;
    if (!root) {
        return 0;
    }
    left_depth = btree_depth(root->left);
    right_depth = btree_depth(root->right);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void btree_preorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    if (cb) {
        cb(root, ctx);
    }
    btree_preorder(root->left, cb, ctx);
    btree_preorder(root->right, cb, ctx);
}

void btree_inorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    btree_inorder(root->left, cb, ctx);
    if (cb) {
        cb(root, ctx);
    }
    btree_inorder(root->right, cb, ctx);
}

void btree_postorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    btree_postorder(root->left, cb, ctx);
    btree_postorder(root->right, cb, ctx);
    if (cb) {
        cb(root, ctx);
    }
}

void btree_levelorder(btree_node_t *root, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
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

void btree_levelorder2(btree_node_t *root, int (*cb)(btree_node_t *node, int depth, void *ctx),
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

void btree_print(btree_node_t *root, void (*cb_print)(void *data))
{
    int *arr_flag = NULL;
    uint32_t depth = btree_depth(root);
    if (depth == 0) {
        return;
    }

    arr_flag = calloc(depth, sizeof(uint32_t));
    if (!arr_flag) {
        return;
    }
    arr_flag[0] = 1;
    btree_print_helper(root, 0, arr_flag, cb_print);

    free(arr_flag);
}
