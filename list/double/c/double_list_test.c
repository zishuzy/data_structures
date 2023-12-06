#include "common/log/log.h"

#include "double_list.h"

int print_node(struct dlist_node *node, void *ctx)
{
    LOG_INFO("node: data[%ld] ptr[0x%08lx] prev[0x%08lx] next[0x%08lx]", (long)node->data,
             (long)node, (long)node->prev, (long)node->next);
    return 0;
}

int print_stop(struct dlist_node *node, void *ctx)
{
    LOG_INFO("node: data[%ld]", (long)node->data);
    if ((long)node->data == 5) {
        return 1;
    }
    return 0;
}

struct find_equel_ctx {
    long val;
    struct dlist_node *node;
};

int find_equel(struct dlist_node *node, void *ctx)
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
    struct dlist_head *head = create_dlist();
    struct dlist_node *node;
    long i;

    if (!head) {
        LOG_ERROR("Failed to make dlist_head!");
        return 0;
    }

    for (i = 0; i < 10; i++) {
        node = create_dlist_node((void *)i);
        LOG_INFO("create node: ptr[0x%08lx] prev[0x%08lx] next[0x%08lx]", (long)node,
                 (long)node->prev, (long)node->next);
        if (!node) {
            LOG_ERROR("Failed to create node! i[%ld]", i);
            continue;
        }
        insert_node_end(head, node);
    }

    LOG_INFO("Forward traverse:");
    walk_dlist_forward(head, print_node, NULL);
    LOG_INFO("Reverse traverse:");
    walk_dlist_reverse(head, print_node, NULL);
    LOG_INFO("Forward stop:");
    walk_dlist_forward(head, print_stop, NULL);

    struct find_equel_ctx fe_ctx = {
        .val = 5,
        .node = NULL,
    };
    LOG_INFO("Find: val[%ld]", fe_ctx.val);
    walk_dlist_reverse(head, find_equel, &fe_ctx);
    if (fe_ctx.node) {
        LOG_INFO("Found val: node[0x%08lx]", (long)fe_ctx.node);
    }

    node = create_dlist_node((void *)50);
    if (node) {
        LOG_INFO("Insert node before! node->data[%ld]", (long)fe_ctx.node->data);
        insert_node_before(head, fe_ctx.node, node);
    } else {
        LOG_ERROR("Failed to create node!");
    }

    node = create_dlist_node((void *)51);
    if (node) {
        LOG_INFO("Insert node after! node->data[%ld]", (long)fe_ctx.node->data);
        insert_node_after(head, fe_ctx.node, node);
    } else {
        LOG_ERROR("Failed to create node!");
    }

    LOG_INFO("Forward traverse:");
    walk_dlist_forward(head, print_node, NULL);
    LOG_INFO("Reverse traverse:");
    walk_dlist_reverse(head, print_node, NULL);

    LOG_INFO("Move node to start! node->data[%ld]", (long)fe_ctx.node->data);
    move_node_start(head, fe_ctx.node);

    LOG_INFO("Forward traverse:");
    walk_dlist_forward(head, print_node, NULL);
    LOG_INFO("Reverse traverse:");
    walk_dlist_reverse(head, print_node, NULL);

    LOG_INFO("Move node to end! node->data[%ld]", (long)fe_ctx.node->data);
    move_node_end(head, fe_ctx.node);

    LOG_INFO("Forward traverse:");
    walk_dlist_forward(head, print_node, NULL);
    LOG_INFO("Reverse traverse:");
    walk_dlist_reverse(head, print_node, NULL);

    LOG_INFO("Move node to start! node->data[%ld]", (long)fe_ctx.node->data);
    move_node_start(head, fe_ctx.node);

    LOG_INFO("Forward traverse:");
    walk_dlist_forward(head, print_node, NULL);
    LOG_INFO("Reverse traverse:");
    walk_dlist_reverse(head, print_node, NULL);

    free_dlist(head, NULL, NULL);

    return 0;
}
