/**
 * @file avl_tree_test.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Test avl tree implemented in C.
 * @version 0.1
 * @date 2023-12-18
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

#include "avl_tree.h"

void print_node_data(avltree_node_t *node, void *ctx)
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
    avltree_node_t *root = NULL;
    avltree_node_t *node;
    int result;

    srand((int)time(NULL));

    for (i = 0; i < 10; i++) {
        tmp = rand() % 100;
        LOG_INFO("Insert val: %ld", tmp);

        root = avltree_insert(root, (void *)tmp, 0, NULL, 0, less);
    }

    printf("tree print:\n");
    avltree_print(root, print_node_data, NULL);

    LOG_INFO("Insert node: key[%ld]", (long)root->key);
    avltree_insert2(root, root->key, root->key_len, root->val, root->val_len, less, cb_free, NULL);

    printf("tree print:\n");
    avltree_print(root, print_node_data, NULL);

    LOG_INFO("Insert node: key[%ld]", (long)tmp);
    avltree_insert2(root, (void *)tmp, 0, NULL, 0, less, cb_free, NULL);

    printf("tree print:\n");
    avltree_print(root, print_node_data, NULL);

    result = avltree_is_exists(root, (void *)tmp, 0, less);
    LOG_INFO("find key[%ld], result[%d]", tmp, result);

    result = avltree_is_exists(root, (void *)(tmp + 1), 0, less);
    LOG_INFO("find key[%ld], result[%d]", (tmp + 1), result);

    node = avltree_find(root, (void *)tmp, 0, less);
    LOG_INFO("find node key[%ld], node[0x%08lx]", tmp, (long)node);

    node = avltree_find(root, (void *)(tmp + 1), 0, less);
    LOG_INFO("find node key[%ld], node[0x%08lx]", (tmp + 1), (long)node);

    avltree_destroy(root, NULL, NULL);

    return 0;
}