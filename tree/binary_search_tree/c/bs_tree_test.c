/**
 * @file bs_tree_test.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Test binary search tree implemented in C.
 * @version 0.1
 * @date 2023-12-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdlib.h>
#include "common/log/log.h"
#include "common/utils/utils_string.h"

#include "queue/c/queue.h"

#include "bs_tree.h"

struct bstree_ctx {
    bstree_node_t *root;
    queue_t *q;
};

int walk_str(const char *str, void *ctx)
{
    static int flag = 0;
    struct bstree_ctx *btctx = ctx;
    queue_node_t *qnode;
    bstree_node_t *btnode = NULL;
    long data;

    LOG_INFO("get str: %s", str);
    if (strcmp(str, "null")) {
        data = atol(str);
        btnode = bstree_node_create((void *)data);
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
                ((bstree_node_t *)qnode->data)->left = btnode;
            }
            flag = 1;
        } else {
            qnode = queue_dequeue(btctx->q);
            if (btnode) {
                ((bstree_node_t *)qnode->data)->right = btnode;
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
    bstree_node_free(btnode);
err1:
    return 1;
}

int print_bstree_node(bstree_node_t *node, void *ctx)
{
    printf("%ld, ", (long)node->val);

    return 0;
}

int print_bstree_node2(bstree_node_t *node, int depth, void *ctx)
{
    static int s_last_depth = -1;

    if (depth != s_last_depth) {
        s_last_depth = depth;
        printf("\ndepth[%d]: ", depth);
    }

    printf("%ld, ", (long)node->val);

    return 0;
}

void print_node_data(void *data)
{
    printf("%ld", (long)data);
}

int main(void)
{
    char tmp[] = {"1,2,3,4,null,5,null,6,7,8,9,10,11,12,13"};
    struct bstree_ctx ctx;

    ctx.root = NULL;
    ctx.q = queue_create();
    if (!ctx.q) {
        LOG_ERROR("Failed to create queue!");
        return 1;
    }

    split_string(tmp, ',', walk_str, &ctx);

    printf("levelorder traverse:\n");
    bstree_levelorder(ctx.root, print_bstree_node, NULL);
    printf("\n");

    printf("levelorder traverse:\n");
    bstree_levelorder2(ctx.root, print_bstree_node2, NULL);
    printf("\n");

    printf("tree print:\n");
    bstree_print(ctx.root, print_node_data);

    queue_free(ctx.q, NULL, NULL);
    bstree_destroy(ctx.root);

    return 0;
}