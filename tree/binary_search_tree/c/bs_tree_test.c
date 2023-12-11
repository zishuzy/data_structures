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
#include <stdio.h>
#include <time.h>

#include "common/log/log.h"
#include "common/utils/utils_string.h"

#include "queue/c/queue.h"

#include "bs_tree.h"

void print_node_data(bstree_node_t *node)
{
    printf("%ld", (long)node->key);
}

int less(bstree_node_t *left, bstree_node_t *right)
{
    return (long)left->key < (long)right->key;
}

int main(void)
{
    long i, tmp;
    bstree_node_t *root = NULL;
    bstree_node_t *node, *old = NULL;
    int result;

    srand((int)time(NULL));
    for (i = 0; i < 10; i++) {
        tmp = rand() % 100;
        LOG_INFO("Insert val: %ld", tmp);

        if (!root) {
            root = bstree_node_create((void *)tmp, 0, (void *)tmp, 0);
        } else {
            node = bstree_node_create((void *)tmp, 0, (void *)tmp, 0);
            bstree_insert(root, node, less);
        }
    }

    printf("tree print:\n");
    bstree_print(root, print_node_data);

    node = bstree_node_create(root->key, root->key_len, root->val, root->val_len);
    bstree_insert2(root, node, &old, less);
    if (old) {
        LOG_INFO("Found node is exist! old[0x%08lx] data[%ld]", (long)old, (long)old->val);

        if (old == root) {
            LOG_INFO("Insert node is root! root[0x%08lx] old[0x%08lx]", (long)root, (long)old);
            root = node;
        }
        bstree_node_free(old, NULL, NULL);
    }

    printf("tree print:\n");
    bstree_print(root, print_node_data);

    result = bstree_is_exists(root, (void *)tmp, 0, less);
    LOG_INFO("find key[%ld], result[%d]", tmp, result);

    result = bstree_is_exists(root, (void *)(tmp + 1), 0, less);
    LOG_INFO("find key[%ld], result[%d]", (tmp + 1), result);

    node = bstree_find(root, (void *)tmp, 0, less);
    LOG_INFO("find node key[%ld], node[0x%08lx]", tmp, (long)node);

    node = bstree_find(root, (void *)(tmp + 1), 0, less);
    LOG_INFO("find node key[%ld], node[0x%08lx]", (tmp + 1), (long)node);

    bstree_destroy(root, NULL, NULL);

    return 0;
}