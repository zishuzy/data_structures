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

void print_node_data(bstree_node_t *node, void *ctx)
{
    printf("%ld", (long)node->key);
}

int less(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len)
{
    return (long)left_key < (long)right_key;
}

int cb_free(void *key, uint32_t key_len, void *val, uint32_t val_len, void *ctx)
{
    LOG_INFO("Found node is exist!, key[%ld]", (long)key);
    return 1;
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
            root = bstree_insert(root, (void *)tmp, 0, NULL, 0, less);
        } else {
            bstree_insert(root, (void *)tmp, 0, NULL, 0, less);
        }
    }

    printf("tree print:\n");
    bstree_print(root, print_node_data, NULL);

    bstree_insert2(root, root->key, root->key_len, root->val, root->val_len, less, cb_free, NULL);

    printf("tree print:\n");
    bstree_print(root, print_node_data, NULL);

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