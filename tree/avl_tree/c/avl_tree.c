/**
 * @file avl_tree.c
 * @author zishu (zishuzy@gmail.com)
 * @brief AVL tree implemented in C.
 * @version 0.1
 * @date 2023-12-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdlib.h>
#include <stdio.h>

#include "queue/c/queue.h"

#include "avl_tree.h"

static void avltree_print_helper_(avltree_node_t *node, int depth, int *arr_flag,
                                  void (*cb_print)(avltree_node_t *node, void *ctx), void *ctx)
{
    int i;

    for (i = 0; i < depth - 1; i++) {
        printf("%s", arr_flag[i] ? "│   " : "    ");
    }
    if (depth) {
        printf("%s", arr_flag[depth - 1] ? "├── " : "└── ");
    }

    if (node == NULL) {
        printf("(null)\n");
        return;
    }
    cb_print(node, ctx);
    printf("\n");
    // printf("%s\n", (char *)node->data);

    arr_flag[depth] = 1;
    avltree_print_helper_(node->right, depth + 1, arr_flag, cb_print, ctx);
    arr_flag[depth] = 0;
    avltree_print_helper_(node->left, depth + 1, arr_flag, cb_print, ctx);
}

static int avltree_balance_factor_(avltree_node_t *root)
{
    return avltree_depth(root->left) - avltree_depth(root->right);
}

static avltree_node_t *avltree_rotation_rr_(avltree_node_t *root)
{
    avltree_node_t *node;
    node = root->right;
    root->right = node->left;
    node->left = root;
    return node;
}

static avltree_node_t *avltree_rotation_ll_(avltree_node_t *root)
{
    avltree_node_t *node;
    node = root->left;
    root->left = node->right;
    node->right = root;
    return node;
}

static avltree_node_t *avltree_rotation_lr_(avltree_node_t *root)
{
    root->left = avltree_rotation_rr_(root->left);
    return avltree_rotation_ll_(root);
}

static avltree_node_t *avltree_rotation_rl_(avltree_node_t *root)
{
    root->right = avltree_rotation_ll_(root->right);
    return avltree_rotation_rr_(root);
}

static avltree_node_t *avltree_balance_(avltree_node_t *root)
{
    int diff_depth = avltree_balance_factor_(root); // 计算平衡因子（左右子树高度差）
    if (diff_depth > 1) {                           // 左子树高于右子树
        if (avltree_balance_factor_(root->left) > 0) { // 左左外侧
            root = avltree_rotation_ll_(root);         // 右旋
        } else {                                       // 左右内侧
            root = avltree_rotation_lr_(root);         // 先左旋后右旋
        }
    } else if (diff_depth < -1) {                       // 右子树高于左子树
        if (avltree_balance_factor_(root->right) > 0) { // 右左内侧
            root = avltree_rotation_rl_(root);          // 先右旋后左旋
        } else {                                        // 右右外侧
            root = avltree_rotation_rr_(root);          // 左旋
        }
    }
    return root;
}

avltree_node_t *avltree_node_create(void *key, uint32_t key_len, void *val, uint32_t val_len)
{
    avltree_node_t *node = malloc(sizeof(avltree_node_t));
    if (!node) {
        return NULL;
    }

    node->key = key;
    node->key_len = key_len;
    node->val = val;
    node->val_len = val_len;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void avltree_node_free(avltree_node_t *node,
                       void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                  void *ctx),
                       void *ctx)
{
    if (!node) {
        return;
    }
    if (cb) {
        cb(node->key, node->key_len, node->val, node->val_len, ctx);
    }
    free(node);
}

void avltree_destroy(avltree_node_t *root,
                     void (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len,
                                void *ctx),
                     void *ctx)
{
    if (!root) {
        return;
    }
    avltree_destroy(root->left, cb, ctx);
    avltree_destroy(root->right, cb, ctx);
    avltree_node_free(root, cb, ctx);
}

avltree_node_t *
avltree_insert(avltree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
               int (*less)(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len))
{
    avltree_node_t *node;
    if (!root) {
        return avltree_node_create(key, key_len, val, val_len);
    }
    if (less(key, key_len, root->key, root->key_len)) {
        node = avltree_insert(root->left, key, key_len, val, val_len, less);
        if (node) {
            root->left = node;
            root = avltree_balance_(root);
        }
    } else if (less(root->key, root->key_len, key, key_len)) {
        node = avltree_insert(root->right, key, key_len, val, val_len, less);
        if (node) {
            root->right = node;
            root = avltree_balance_(root);
        }
    } else {
        root = NULL;
    }

    return root;
}

avltree_node_t *
avltree_insert2(avltree_node_t *root, void *key, uint32_t key_len, void *val, uint32_t val_len,
                int (*less)(void *left_key, uint32_t left_len, void *right_key, uint32_t right_len),
                int (*cb)(void *key, uint32_t key_len, void *val, uint32_t val_len, void *ctx),
                void *ctx)
{
    if (!root) {
        return avltree_node_create(key, key_len, val, val_len);
    }
    if (less(key, key_len, root->key, root->key_len)) {
        root->left = avltree_insert2(root->left, key, key_len, val, val_len, less, cb, ctx);
        root = avltree_balance_(root);
    } else if (less(root->key, root->key_len, key, key_len)) {
        root->right = avltree_insert2(root->right, key, key_len, val, val_len, less, cb, ctx);
        root = avltree_balance_(root);
    } else {
        if (cb(root->key, root->key_len, root->val, root->val_len, ctx)) {
            root->key = key;
            root->key_len = key_len;
            root->val = val;
            root->val_len = val_len;
        }
    }

    return root;
}

int avltree_is_exists(avltree_node_t *root, void *key, uint32_t key_len,
                      int (*less)(void *left_key, uint32_t left_len, void *right_key,
                                  uint32_t right_len))
{
    int rc = 0;
    if (!root) {
        return rc;
    }

    if (less(key, key_len, root->key, root->key_len)) {
        rc = avltree_is_exists(root->left, key, key_len, less);
    } else if (less(root->key, root->key_len, key, key_len)) {
        rc = avltree_is_exists(root->right, key, key_len, less);
    } else { // node == root
        rc = 1;
    }
    return rc;
}

avltree_node_t *avltree_find(avltree_node_t *root, void *key, uint32_t key_len,
                             int (*less)(void *left_key, uint32_t left_len, void *right_key,
                                         uint32_t right_len))
{
    avltree_node_t *fnode = NULL;
    if (!root) {
        return NULL;
    }
    if (less(key, key_len, root->key, root->key_len)) {
        fnode = avltree_find(root->left, key, key_len, less);
    } else if (less(root->key, root->key_len, key, key_len)) {
        fnode = avltree_find(root->right, key, key_len, less);
    } else { // node == root
        fnode = root;
    }

    return fnode;
}

uint32_t avltree_depth(avltree_node_t *root)
{
    uint32_t left_depth, right_depth;
    if (!root) {
        return 0;
    }
    left_depth = avltree_depth(root->left);
    right_depth = avltree_depth(root->right);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

void avltree_preorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    if (cb) {
        cb(root, ctx);
    }
    avltree_preorder(root->left, cb, ctx);
    avltree_preorder(root->right, cb, ctx);
}

void avltree_inorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    avltree_inorder(root->left, cb, ctx);
    if (cb) {
        cb(root, ctx);
    }
    avltree_inorder(root->right, cb, ctx);
}

void avltree_postorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx)
{
    if (!root) {
        return;
    }

    avltree_postorder(root->left, cb, ctx);
    avltree_postorder(root->right, cb, ctx);
    if (cb) {
        cb(root, ctx);
    }
}

void avltree_levelorder(avltree_node_t *root, int (*cb)(avltree_node_t *node, void *ctx), void *ctx)
{
    queue_t *q;
    queue_node_t *qnode;
    if (!root) {
        return;
    }

    q = queue_create();
    if (!q) {
        return;
    }

    do {
        qnode = queue_node_create(root);
        if (!qnode) {
            break;
        }
        if (queue_enqueue(q, qnode) < 0) {
            queue_node_free(qnode);
            break;
        }

        while (!queue_is_empty(q)) {
            qnode = queue_dequeue(q);
            if (!qnode) {
                continue;
            }
            root = qnode->data;
            queue_node_free(qnode);

            if (cb(root, ctx)) {
                break;
            }

            if (root->left) {
                qnode = queue_node_create(root->left);
                queue_enqueue(q, qnode);
            }
            if (root->right) {
                qnode = queue_node_create(root->right);
                queue_enqueue(q, qnode);
            }
        }

    } while (0);

    queue_free(q, NULL, NULL);
}

void avltree_levelorder2(avltree_node_t *root,
                         int (*cb)(avltree_node_t *node, int depth, void *ctx), void *ctx)
{
    queue_t *q = NULL;
    queue_t *q1;
    queue_t *q2;
    queue_node_t *qnode;
    int depth = 0;

    if (!root) {
        return;
    }

    q1 = queue_create();
    if (!q1) {
        return;
    }
    q2 = queue_create();
    if (!q2) {
        queue_free(q1, NULL, NULL);
        return;
    }
    q = q1;

    do {
        qnode = queue_node_create(root);
        if (!qnode) {
            break;
        }
        if (queue_enqueue(q, qnode) < 0) {
            queue_node_free(qnode);
            break;
        }

        do {
            while (!queue_is_empty(q)) {
                qnode = queue_dequeue(q);
                if (!qnode) {
                    continue;
                }
                root = qnode->data;
                queue_node_free(qnode);

                if (cb(root, depth, ctx)) {
                    break;
                }

                if (root->left) {
                    qnode = queue_node_create(root->left);
                    queue_enqueue(q == q1 ? q2 : q1, qnode);
                }
                if (root->right) {
                    qnode = queue_node_create(root->right);
                    queue_enqueue(q == q1 ? q2 : q1, qnode);
                }
            }
            depth++;
            q = q == q1 ? q2 : q1;
        } while (!queue_is_empty(q));
    } while (0);

    queue_free(q1, NULL, NULL);
    queue_free(q2, NULL, NULL);
}

void avltree_print(avltree_node_t *root, void (*cb_print)(avltree_node_t *node, void *ctx),
                   void *ctx)
{
    int *arr_flag = NULL;

    uint32_t depth = avltree_depth(root);
    if (depth == 0) {
        return;
    }

    arr_flag = calloc(depth, sizeof(uint32_t));
    if (!arr_flag) {
        return;
    }
    arr_flag[0] = 1;
    avltree_print_helper_(root, 0, arr_flag, cb_print, ctx);

    free(arr_flag);
}
