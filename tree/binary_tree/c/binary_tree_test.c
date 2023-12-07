/**
 * @file binary_tree_test.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Test binary tree implemented in C.
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdlib.h>
#include "common/log/log.h"
#include "common/utils/utils_string.h"

#include "queue/c/queue.h"

#include "binary_tree.h"

struct btree_ctx {
    btree_node_t *root;
    queue_t *q;
};

int walk_str(const char *str, void *ctx)
{
    static int flag = 0;
    struct btree_ctx *btctx = ctx;
    queue_node_t *qnode;
    btree_node_t *btnode;
    char *data = strndup(str, strlen(str));

    LOG_INFO("get str: %s, data[%s]", str, data);

    btnode = btree_node_create(data);
    if (!btnode) {
        goto err1;
    }

    if (!btctx->root) {
        btctx->root = btnode;
    } else {
        if (!flag) {
            qnode = queue_front(btctx->q);
            ((btree_node_t *)qnode->data)->left = btnode;
            flag = 1;
        } else {
            qnode = queue_dequeue(btctx->q);
            ((btree_node_t *)qnode->data)->right = btnode;
            flag = 0;
        }
    }

    if (strcmp(str, "null")) {
        qnode = queue_node_create(btnode);
        if (!qnode) {
            goto err2;
        }
        queue_enqueue(btctx->q, qnode);
    }

    return 0;
err2:
    btree_node_free(btnode);
err1:
    free(data);
    return 1;
}

int print_btree_node(btree_node_t *node, void *ctx)
{
    LOG_INFO("node: %s", (const char *)node->data);

    return 0;
}

int main(void)
{
    char tmp[] = {"1,2,3,4,null,5,null,6,7,8"};
    struct btree_ctx ctx;

    ctx.root = NULL;
    ctx.q = queue_create();
    if (!ctx.q) {
        LOG_ERROR("Failed to create queue!");
        return 1;
    }

    split_string(tmp, ',', walk_str, &ctx);

    LOG_INFO("root: %s", (char *)ctx.root->data);
    LOG_INFO("left: %s", (char *)ctx.root->left->data);
    LOG_INFO("right: %s", (char *)ctx.root->right->data);

    btree_levelorder(ctx.root, print_btree_node, NULL);

    return 0;
}