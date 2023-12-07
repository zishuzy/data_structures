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

void btree_preorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    if (!node) {
        return;
    }

    if (cb) {
        cb(node, ctx);
    }
    btree_preorder(node->left, cb, ctx);
    btree_preorder(node->right, cb, ctx);
}

void btree_inorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    if (!node) {
        return;
    }

    btree_inorder(node->left, cb, ctx);
    if (cb) {
        cb(node, ctx);
    }
    btree_inorder(node->right, cb, ctx);
}

void btree_postorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    if (!node) {
        return;
    }

    btree_postorder(node->left, cb, ctx);
    btree_postorder(node->right, cb, ctx);
    if (cb) {
        cb(node, ctx);
    }
}

void btree_levelorder(btree_node_t *node, int (*cb)(btree_node_t *node, void *ctx), void *ctx)
{
    queue_t *q;
    queue_node_t *qnode;
    if (!node) {
        return;
    }

    q = queue_create();
    if (!q) {
        return;
    }

    do {
        qnode = queue_node_create(node);
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
            node = qnode->data;
            queue_node_free(qnode);

            if (cb(node, ctx)) {
                break;
            }

            if (node->left) {
                qnode = queue_node_create(node->left);
                queue_enqueue(q, qnode);
            }
            if (node->right) {
                qnode = queue_node_create(node->right);
                queue_enqueue(q, qnode);
            }
        }

    } while (0);

    queue_free(q, NULL, NULL);
}
