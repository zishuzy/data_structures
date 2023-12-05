#include "common/log/log.h"

#include "list_double.h"

int print_node(struct dlist_node *node, void *ctx)
{
    LOG_INFO("node: data[%ld]", (long)node->data);
    return 0;
}

int print_less5(struct dlist_node *node, void *ctx)
{
    LOG_INFO("node: data[%ld]", (long)node->data);
    if ((long)node->data >= 5) {
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
    long i;

    if (!head) {
        LOG_ERROR("Failed to make dlist_head!");
        return 0;
    }

    for (i = 0; i < 10; i++) {
        add_node_last(head, (void *)i);
    }

    LOG_INFO("forward:");
    walk_dlist_forward(head, print_node, NULL);
    LOG_INFO("reverse:");
    walk_dlist_reverse(head, print_node, NULL);
    LOG_INFO("forward less 5:");
    walk_dlist_forward(head, print_less5, NULL);

    struct find_equel_ctx fe_ctx = {
        .val = 5,
        .node = NULL,
    };
    LOG_INFO("find: val[%ld]", fe_ctx.val);
    walk_dlist_reverse(head, find_equel, &fe_ctx);
    if (fe_ctx.node) {
        LOG_INFO("Found val: node[0x%08lx]", (long)fe_ctx.node);
    }

    LOG_INFO("insert 50 to node prev! node->data[%ld]", (long)fe_ctx.node->data);
    add_node_prev(head, fe_ctx.node, (void *)50);
    LOG_INFO("insert 51 to node next! node->data[%ld]", (long)fe_ctx.node->data);
    add_node_next(head, fe_ctx.node, (void *)51);

    LOG_INFO("forward:");
    walk_dlist_forward(head, print_node, NULL);
    return 0;
}
