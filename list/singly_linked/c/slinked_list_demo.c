/**
 * @file slinked_list_test.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Test singly-linked list using C.
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "common/log/log.h"

#include "slinked_list.h"

int print_node(struct slinked_list_node *node, void *ctx)
{
    LOG_INFO("node: data[%ld] ptr[0x%08lx] next[0x%08lx]", (long)node->data, (long)node,
             (long)node->next);
    return 0;
}

int print_stop(struct slinked_list_node *node, void *ctx)
{
    LOG_INFO("node: data[%ld]", (long)node->data);
    if ((long)node->data == 5) {
        return 1;
    }
    return 0;
}

struct find_equel_ctx {
    long val;
    struct slinked_list_node *node;
};

int find_equel(struct slinked_list_node *node, void *ctx)
{
    struct find_equel_ctx *fe_ctx;
    if (!ctx) {
        LOG_WARN("ctx is NULL!");
        return 1;
    }
    fe_ctx = (struct find_equel_ctx *)ctx;
    if ((long)node->data == fe_ctx->val) {
        fe_ctx->node = node;
        LOG_INFO("Found node! ptr[0x%08lx] val[%ld]", (long)node, (long)node->data);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct slinked_list_head *head = slist_create();
    struct slinked_list_node *node;
    long i;

    if (!head) {
        LOG_ERROR("Failed to make the head of singly-linked list!");
        return 0;
    }

    for (i = 0; i < 10; i++) {
        node = slist_node_create((void *)i);
        LOG_INFO("create node: val[%ld] ptr[0x%08lx] next[0x%08lx]", (long)node->data, (long)node,
                 (long)node->next);
        if (!node) {
            LOG_ERROR("Failed to create node! i[%ld]", i);
            continue;
        }
        slist_insert_node_in_end(head, node);
    }

    LOG_INFO("Forward traverse:");
    slist_forward_traverse(head, print_node, NULL);
    LOG_INFO("Forward stop:");
    slist_forward_traverse(head, print_stop, NULL);

    struct find_equel_ctx fe_ctx = {
        .val = 5,
        .node = NULL,
    };
    LOG_INFO("Find: val[%ld]", fe_ctx.val);
    slist_forward_traverse(head, find_equel, &fe_ctx);
    if (fe_ctx.node) {
        LOG_INFO("Found val: node[0x%08lx]", (long)fe_ctx.node);
    }

    node = slist_node_create((void *)50);
    if (node) {
        LOG_INFO("Insert node before! node->data[%ld]", (long)fe_ctx.node->data);
        slist_insert_node_before(head, fe_ctx.node, node);
    } else {
        LOG_ERROR("Failed to create node!");
    }

    node = slist_node_create((void *)51);
    if (node) {
        LOG_INFO("Insert node after! node->data[%ld]", (long)fe_ctx.node->data);
        slist_insert_node_after(head, fe_ctx.node, node);
    } else {
        LOG_ERROR("Failed to create node!");
    }

    LOG_INFO("Forward traverse:");
    slist_forward_traverse(head, print_node, NULL);

    LOG_INFO("Move node to start! node->data[%ld]", (long)fe_ctx.node->data);
    slist_move_node_to_start(head, fe_ctx.node);

    LOG_INFO("Forward traverse:");
    slist_forward_traverse(head, print_node, NULL);

    LOG_INFO("Move node to end! node->data[%ld]", (long)fe_ctx.node->data);
    slist_move_node_to_end(head, fe_ctx.node);

    LOG_INFO("Forward traverse:");
    slist_forward_traverse(head, print_node, NULL);

    LOG_INFO("Move node to start! node->data[%ld]", (long)fe_ctx.node->data);
    slist_move_node_to_start(head, fe_ctx.node);

    LOG_INFO("Forward traverse:");
    slist_forward_traverse(head, print_node, NULL);

    slist_free(head, NULL, NULL);

    return 0;
}
