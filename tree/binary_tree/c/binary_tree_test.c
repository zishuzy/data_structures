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
    btree_node_t *btnode = NULL;
    long data;

    LOG_INFO("get str: %s", str);
    if (strcmp(str, "null")) {
        data = atol(str);
        btnode = btree_node_create((void *)data);
        if (!btnode) {
            goto err1;
        }
    }

    if (!btctx->root) {
        btctx->root = btnode;
    } else {
        if (!flag) {
            qnode = queue_front(btctx->q);
            if (btnode) {
                ((btree_node_t *)qnode->data)->left = btnode;
            }
            flag = 1;
        } else {
            qnode = queue_dequeue(btctx->q);
            if (btnode) {
                ((btree_node_t *)qnode->data)->right = btnode;
            }
            flag = 0;
            queue_node_free(qnode);
        }
    }

    if (btnode) {
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
    return 1;
}

int print_btree_node(btree_node_t *node, void *ctx)
{
    printf("%ld, ", (long)node->data);

    return 0;
}

int print_btree_node2(btree_node_t *node, int depth, void *ctx)
{
    static int s_last_depth = -1;

    if (depth != s_last_depth) {
        s_last_depth = depth;
        printf("\ndepth[%d]: ", depth);
    }

    printf("%ld, ", (long)node->data);

    return 0;
}

void print_node_data(void *data)
{
    printf("%ld", (long)data);
}

int main(void)
{
    char tmp[] = {"1,2,3,4,null,5,null,6,7,8,9,10,11,12,13"};
    struct btree_ctx ctx;

    ctx.root = NULL;
    ctx.q = queue_create();
    if (!ctx.q) {
        LOG_ERROR("Failed to create queue!");
        return 1;
    }

    split_string(tmp, ',', walk_str, &ctx);

    printf("levelorder traverse:\n");
    btree_levelorder(ctx.root, print_btree_node, NULL);
    printf("\n");

    printf("levelorder traverse:\n");
    btree_levelorder2(ctx.root, print_btree_node2, NULL);
    printf("\n");

    printf("tree print:\n");
    btree_print(ctx.root, print_node_data);

    queue_free(ctx.q, NULL, NULL);
    btree_destroy(ctx.root);

    return 0;
}