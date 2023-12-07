/**
 * @file slinked_list_test.cpp
 * @author zishu (zishuzy@gmail.com)
 * @brief Test singly-linked list using C++.
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "slinked_list.hpp"
#include "common/utils/utils_string.hpp"

int print_node(llist::singly_linked::Node<long> *node, void *ctx)
{
    LOG_INFO("node: data[%ld] ptr[0x%08lx] next[0x%08lx]", node->data, (long)node,
             (long)node->next);
    return 0;
}

int print_stop(llist::singly_linked::Node<long> *node, void *ctx)
{
    LOG_INFO("node: data[%ld]", (long)node->data);
    if ((long)node->data == 5) {
        return 1;
    }
    return 0;
}

struct find_equel_ctx {
    long val;
    llist::singly_linked::Node<long> *node;
};

int find_equel(llist::singly_linked::Node<long> *node, void *ctx)
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
    llist::singly_linked::SlinkedList<long> slinkedlist;
    llist::singly_linked::Node<long> *node;
    long i;
    bool ok;

    for (i = 0; i < 10; i++) {
        ok = slinkedlist.InsertEnd(i);
        LOG_INFO("insert ok[%s]: val[%ld] ", utils::uts_string::Bool2Str(ok).c_str(), i);
    }

    LOG_INFO("Forward traverse:");
    slinkedlist.ForwardTraverse(print_node, nullptr);
    LOG_INFO("Forward stop:");
    slinkedlist.ForwardTraverse(print_stop, nullptr);

    struct find_equel_ctx fe_ctx = {
        .val = 5,
        .node = NULL,
    };
    LOG_INFO("Find: val[%ld]", fe_ctx.val);
    slinkedlist.ForwardTraverse(find_equel, &fe_ctx);
    if (fe_ctx.node) {
        LOG_INFO("Found val: node[0x%08lx]", (long)fe_ctx.node);
    }

    node = new (std::nothrow) llist::singly_linked::Node<long>(50);
    if (node == nullptr) {
        LOG_ERROR("Failed to create node!");
    } else {
        slinkedlist.InsertNodeBefore(fe_ctx.node, node);
        LOG_INFO("Insert node before! node->data[%ld]", (long)fe_ctx.node->data);
    }

    node = new (std::nothrow) llist::singly_linked::Node<long>(51);
    if (node == nullptr) {
        LOG_ERROR("Failed to create node!");
    } else {
        slinkedlist.InsertNodeAfter(fe_ctx.node, node);
        LOG_INFO("Insert node after! node->data[%ld]", (long)fe_ctx.node->data);
    }

    LOG_INFO("Forward traverse:");
    slinkedlist.ForwardTraverse(print_node, nullptr);

    LOG_INFO("Move node to start! node->data[%ld]", (long)fe_ctx.node->data);
    slinkedlist.MoveStart(fe_ctx.node);

    LOG_INFO("Forward traverse:");
    slinkedlist.ForwardTraverse(print_node, nullptr);

    LOG_INFO("Move node to end! node->data[%ld]", (long)fe_ctx.node->data);
    slinkedlist.MoveEnd(fe_ctx.node);

    LOG_INFO("Forward traverse:");
    slinkedlist.ForwardTraverse(print_node, nullptr);

    LOG_INFO("Move node to start! node->data[%ld]", (long)fe_ctx.node->data);
    slinkedlist.MoveStart(fe_ctx.node);

    LOG_INFO("Forward traverse:");
    slinkedlist.ForwardTraverse(print_node, nullptr);

    return 0;
}
