#include "rb_tree.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// ------------------------ define ------------------------

#define RB_NODE_RED   0 // 红色节点
#define RB_NODE_BLACK 1 // 黑色节点

#define RB_PRINT_MAX   1024 // 打印红黑数最大深度
#define RB_PRINT_COLOR 1    // 打印是否显示颜色

// 红黑树的节点
typedef struct rbtree_node {
    void *key;
    void *value;
    bool key_free_need;
    bool value_free_need;
    unsigned char color;
    struct rbtree_node *left;
    struct rbtree_node *right;
    struct rbtree_node *parent;
} node_t, *rbtree_t;

// 红黑树的根
typedef struct rbtree_root {
    node_t *node;
    int (*cmp_key)(void *key0, void *key1);
    void *(*copy_key)(void *key);
    void (*free_key)(void *key);
    void *(*copy_value)(void *key);
    void (*free_value)(void *value);
    bool is_thread_safe;
    pthread_rwlock_t rwlocker;
} rbroot_t;

#define KEY_LESS(k0, k1, cmp_key)    (cmp_key(k0, k1) < 0)
#define KEY_GREATER(k0, k1, cmp_key) (cmp_key(k0, k1) > 0)
#define KEY_EQUAL(k0, k1, cmp_key)   (cmp_key(k0, k1) == 0)

// clang-format off
#define rb_parent(r)       ((r)->parent)
#define rb_color(r)        ((r)->color)
#define rb_is_red(r)       ((r)->color == RB_NODE_RED)
#define rb_is_black(r)     ((r)->color == RB_NODE_BLACK)
#define rb_set_black(r)    do { (r)->color = RB_NODE_BLACK; } while (0)
#define rb_set_red(r)      do { (r)->color = RB_NODE_RED; } while (0)
#define rb_set_parent(r,p) do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)
#ifndef swap
#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)      ( (sizeof(a)) / (sizeof(a[0])) )
#endif
// clang-format on
#define LOCK_RBTREE_WR(r)                        \
    {                                            \
        if ((r)->is_thread_safe)                 \
            pthread_rwlock_wrlock(&r->rwlocker); \
    }
#define LOCK_RBTREE_RD(r)                        \
    {                                            \
        if ((r)->is_thread_safe)                 \
            pthread_rwlock_rdlock(&r->rwlocker); \
    }
#define UNLOCK_RBTREE(r)                         \
    {                                            \
        if ((r)->is_thread_safe)                 \
            pthread_rwlock_unlock(&r->rwlocker); \
    }

// ------------------------ declaration ------------------------
static int default_cmp_key(void *key0, void *key1);
static void *default_copy_key(void *key);
static void default_free_key(void *key);
static void *default_copy_val(void *val);
static void default_free_val(void *val);
static node_t *rbtree_create_node(void *key, void *value, node_t *parent, node_t *left,
                                  node_t *right, bool k_copy, bool v_copy,
                                  void *(*copy_key)(void *key), void *(*copy_val)(void *val),
                                  void (*free_key)(void *key), void (*free_val)(void *val));
static void rbtree_left_rotate(rbroot_t *root, node_t *x);
static void rbtree_right_rotate(rbroot_t *root, node_t *y);
static void rbtree_insert_node(rbroot_t *root, node_t *node);
static void rbtree_insert_fixup(rbroot_t *root, node_t *node);
static void *rbtree_delete_node(rbroot_t *root, node_t *node);
static void rbtree_delete_fixup(rbroot_t *root, node_t *node, node_t *parent);
static void preorder(rbtree_t tree, void (*cb)(void *key, void *value));
static void inorder(rbtree_t tree, void (*cb)(void *key, void *value));
static void postorder(rbtree_t tree, void (*cb)(void *key, void *value));
static void levelorder(rbtree_t tree, void (*cb)(void *key, void *value));
static node_t *search(rbtree_t x, void *key, int (*cmp_key)(void *key0, void *key1));
static node_t *search_iterative(rbtree_t x, void *key, int (*cmp_key)(void *key0, void *key1));
static node_t *min_node(rbtree_t tree);
static node_t *max_node(rbtree_t tree);
static void __rbtree_destroy(rbtree_t tree, void (*free_key)(void *key),
                             void (*free_val)(void *key));
static void print_rbtree_inner(node_t *node, size_t n_deepth, uint8_t *arr_flag);

// ------------------------ public ------------------------
rbroot_t *rbtree_init(struct rbtree_arg arg)
{
    rbroot_t *root = (rbroot_t *)calloc(1, sizeof(rbroot_t));
    if (!root) {
        return NULL;
    }
    root->node = NULL;
    root->is_thread_safe = arg.is_thread_safe;

    root->cmp_key = arg.cmp_key ? arg.cmp_key : default_cmp_key;
    root->copy_key = arg.copy_key ? arg.copy_key : default_copy_key;
    root->free_key = arg.free_key ? arg.free_key : default_free_key;
    root->copy_value = arg.copy_value ? arg.copy_value : default_copy_val;
    root->free_value = arg.free_value ? arg.free_value : default_free_val;

    if (root->is_thread_safe) {
        if (pthread_rwlock_init(&root->rwlocker, NULL) != 0) {
            goto err0;
        }
    }

    return root;
err0:
    free(root);
    return NULL;
}
// 销毁红黑数
void rbtree_destroy(rbroot_t *root)
{
    if (root == NULL)
        return;

    LOCK_RBTREE_WR(root);
    __rbtree_destroy(root->node, root->free_key, root->free_value);
    UNLOCK_RBTREE(root);
    free(root);
}

bool rbtree_is_exist(rbroot_t *root, void *key)
{
    bool rc = false;
    if (root == NULL) {
        return false;
    }
    LOCK_RBTREE_RD(root);
    rc = search_iterative(root->node, key, root->cmp_key) ? true : false;
    UNLOCK_RBTREE(root);
    return rc;
}
// 查找一个key对应的data(递归)，如果没有返回NULL
void *rbtree_search(rbroot_t *root, void *key)
{
    node_t *node;
    void *value;
    if (root == NULL) {
        return NULL;
    }
    LOCK_RBTREE_RD(root);
    node = search(root->node, key, root->cmp_key);
    if (node == NULL) {
        UNLOCK_RBTREE(root);
        return NULL;
    }
    value = node->value;
    UNLOCK_RBTREE(root);
    return value;
}
// 插入一个节点
int rbtree_insert(struct rbtree_root *root, void *key, void *value, bool key_copy, bool val_copy)
{
    node_t *node; // 新建结点
    void *val_free = NULL;

    LOCK_RBTREE_WR(root);
    if ((node = search(root->node, key, root->cmp_key)) != NULL) {
        if (node->value_free_need) {
            val_free = node->value;
        }
        node->value = value;
        node->value_free_need = key_copy;
    } else {
        node = rbtree_create_node(key, value, NULL, NULL, NULL, key_copy, val_copy, root->copy_key,
                                  root->copy_value, root->free_key, root->free_value);
        if (node == NULL) {
            goto out;
        }
        rbtree_insert_node(root, node);
    }
out:
    UNLOCK_RBTREE(root);
    if (val_free) {
        root->free_value(val_free);
    }
    return 0;
}
// 删除一个节点
void rbtree_delete(rbroot_t *root, void *key)
{
    node_t *z;
    void *key_free = NULL;
    void *val_free = NULL;
    LOCK_RBTREE_WR(root);
    if ((z = search(root->node, key, root->cmp_key)) == NULL) {
        UNLOCK_RBTREE(root);
        return;
    }
    if (z->key_free_need) {
        key_free = z->key;
    }
    if (z->value_free_need) {
        val_free = z->value;
    }

    rbtree_delete_node(root, z);
    UNLOCK_RBTREE(root);

    if (key_free) {
        root->free_key(key_free);
    }
    if (val_free) {
        root->free_value(val_free);
    }
}

void rbtree_preorder(struct rbtree_root *root, void (*cb)(void *key, void *value))
{
    if (!root || !root->node) {
        return;
    }

    preorder(root->node, cb);
}

void rbtree_inorder(struct rbtree_root *root, void (*cb)(void *key, void *value))
{
    if (!root || !root->node) {
        return;
    }

    inorder(root->node, cb);
}

void rbtree_postorder(struct rbtree_root *root, void (*cb)(void *key, void *value))
{
    if (!root || !root->node) {
        return;
    }

    postorder(root->node, cb);
}
// 后序遍历
// TODO: 等待实现队列做
void rbtree_levelorder(struct rbtree_root *root, void (*cb)(void *key, void *value))
{
    if (!root || !root->node) {
        return;
    }
}

// ------------------------ private ------------------------
static int default_cmp_key(void *key0, void *key1)
{
    if ((uint64_t)key0 < (uint64_t)key1) {
        return -1;
    } else if ((uint64_t)key0 > (uint64_t)key1) {
        return 1;
    } else {
        return 0;
    }
    return 0;
}

static void *default_copy_key(void *key)
{
    void *k = calloc(1, sizeof(uint64_t));
    if (!k) {
        return NULL;
    }
    memcpy(k, key, sizeof(uint64_t));
    return k;
}

static void default_free_key(void *key)
{
    if (key) {
        free(key);
    }
}

static void *default_copy_val(void *val)
{
    void *v = calloc(1, sizeof(uint64_t));
    if (!v) {
        return NULL;
    }
    memcpy(v, val, sizeof(uint64_t));
    return v;
}

static void default_free_val(void *val)
{
    if (val) {
        free(val);
    }
}

// 创建红黑树中一个节点
static node_t *rbtree_create_node(void *key, void *value, node_t *parent, node_t *left,
                                  node_t *right, bool k_copy, bool v_copy,
                                  void *(*copy_key)(void *key), void *(*copy_val)(void *val),
                                  void (*free_key)(void *key), void (*free_val)(void *val))
{
    node_t *p;
    if ((p = (node_t *)calloc(1, sizeof(node_t))) == NULL)
        return NULL;
    if (k_copy) {
        p->key = copy_key(key);
        if (!p->key) {
            goto err;
        }
    } else {
        p->key = key;
    }
    if (v_copy) {
        p->value = copy_val(value);
        if (!p->value) {
            goto err;
        }
    } else {
        p->value = value;
    }
    p->left = left;
    p->right = right;
    p->parent = parent;
    p->color = RB_NODE_BLACK; // 默认为黑色
    return p;
err:
    if (p->key && k_copy) {
        free_key(p->key);
    }
    if (p->value && v_copy) {
        free_val(p->value);
    }
    if (p) {
        free(p);
    }
    return NULL;
}

// 左旋
/*      p              p
 *      |              |
 *      x              y
 *     / \            / \
 *    a   y    -->   x   c
 *       / \        / \
 *      b   c      a   b
 */
static void rbtree_left_rotate(rbroot_t *root, node_t *x)
{
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        root->node = y;
    } else {
        if (x->parent->left == x) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
}
// 右旋
/*        p             p
 *        |             |
 *        y             x
 *       / \           / \
 *      x   c  -->    a   y
 *     / \               / \
 *    a   b             b   c
 */
static void rbtree_right_rotate(rbroot_t *root, node_t *y)
{
    node_t *x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        root->node = x;
    } else {
        if (y->parent->right == y) {
            y->parent->right = x;
        } else {
            y->parent->left = x;
        }
    }
    x->right = y;
    y->parent = x;
}

// 红黑树插入
static void rbtree_insert_node(rbroot_t *root, node_t *node)
{
    node_t *y = NULL;
    node_t *x = root->node;
    while (x != NULL) {
        y = x;

        if (KEY_LESS(node->key, x->key, root->cmp_key)) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    rb_parent(node) = y;
    if (y == NULL) {
        // 插入根节点
        root->node = node;
    } else if (KEY_LESS(node->key, y->key, root->cmp_key)) {
        y->left = node;
    } else {
        // 包含了相同值的情况，其实也就是更新
        y->right = node;
    }
    node->color = RB_NODE_RED;
    rbtree_insert_fixup(root, node);
}

// 红黑树插入节点后修正
static void rbtree_insert_fixup(rbroot_t *root, node_t *node)
{
    node_t *parent, *gparent;
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);
        if (parent == gparent->left) { // 父节点是祖父节点的左孩子
            node_t *uncle = gparent->right;
            if (uncle && rb_is_red(uncle)) { // case 1: 叔叔节点是红节点
                rb_set_black(parent);        // 将“父节点”设为黑色
                rb_set_black(uncle);         // 将“叔叔节点”设为黑色
                rb_set_red(gparent);         // 将“祖父节点”设为“红色”
                node = gparent; // 将“祖父节点”设为“当前节点”(红色节点)
                continue;       // 之后继续对“当前节点”进行操作。
            }
            // 到这表示叔叔节点是黑节点
            if (parent->right == node) { // case 2: 叔叔是黑色，当前节点是父节点的右孩子
                swap(parent, node);             // 将“父节点”作为“新的当前节点”
                rbtree_left_rotate(root, node); // 以“新的当前节点”为支点进行左旋
                // 经过上面的左旋后，当前节点一定是左孩子了
            }

            // case 3：叔叔是黑色，当前节点是左孩子
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);

        } else { // 父节点是祖父节点的右孩子
            node_t *uncle = gparent->left;
            if (uncle && rb_is_red(uncle)) { // case 1: 叔叔节点是红节点
                rb_set_black(parent);        // 将“父节点”设为黑色
                rb_set_black(uncle);         // 将“叔叔节点”设为黑色
                rb_set_red(gparent);         // 将“祖父节点”设为“红色”
                node = gparent; // 将“祖父节点”设为“当前节点”(红色节点)
                continue;       // 之后继续对“当前节点”进行操作。
            }
            // 到这表示叔叔节点是黑节点
            if (parent->left == node) { // case 2: 叔叔是黑色，当前节点是父节点的左孩子
                swap(parent, node);              // 将“父节点”作为“新的当前节点”
                rbtree_right_rotate(root, node); // 以“新的当前节点”为支点进行右旋
                // 经过上面的右旋后，当前节点一定是右孩子了
            }
            // case 3：叔叔是黑色，当前节点是右孩子
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }
    rb_set_black(root->node);
}
// 红黑树删除
static void *rbtree_delete_node(rbroot_t *root, node_t *node)
{
    node_t *child, *parent;
    void *result = NULL;
    int color;
    if (node->left != NULL && node->right != NULL) { // 节点左右孩子都不为空
        // 替代被删除的节点
        node_t *replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        if (rb_parent(node)) { // 不是根节点
            if (rb_parent(node)->left == node) {
                rb_parent(node)->left = replace;
            } else {
                rb_parent(node)->right = replace;
            }
        } else { // 被删除的节点是根节点
            root->node = replace;
        }
        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        color = rb_color(replace);

        if (parent == node) { // 取代节点就是删除节点的右孩子
            parent = replace;
        } else {
            if (child) {
                rb_set_parent(child, parent);
            }
            parent->left = child;
            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == RB_NODE_BLACK) {
            rbtree_delete_fixup(root, child, parent);
        }
        result = node->value;
        free(node);
        return result;
    }
    if (node->left != NULL) {
        child = node->left;
    } else {
        child = node->right;
    }
    parent = node->parent;
    color = node->color;

    if (child) {
        child->parent = parent;
    }
    if (parent) { // "node"节点不是根节点
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        root->node = child;
    }

    if (color == RB_NODE_BLACK) {
        rbtree_delete_fixup(root, child, parent);
    }
    result = node->value;
    free(node);
    return result;
}
// 红黑树删除后的修正
static void rbtree_delete_fixup(rbroot_t *root, node_t *node, node_t *parent)
{
    node_t *other;
    while ((!node || rb_is_black(node)) && node != root->node) { // 调整节点不是根节点
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                // case 1: x的兄弟节点是红色
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // case 2: x的兄弟节点是黑色，且兄弟节点的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->right || rb_is_black(other->right)) {
                    // case 3: x的兄弟节点是黑色的，并且兄弟节点的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                // case 4: x的兄弟节点是黑色的；并且兄弟节点的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        } else {
            other = parent->left;
            if (rb_is_red(other)) {
                // case 1: x的兄弟节点是红色
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // case 2: x的兄弟节点是黑色，且兄弟节点的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->left || rb_is_black(other->left)) {
                    // case 3: x的兄弟节点是黑色的，并且兄弟节点的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                // case 4: x的兄弟节点是黑色的；并且兄弟节点的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}
// 前序遍历
static void preorder(rbtree_t tree, void (*cb)(void *key, void *value))
{
    if (tree != NULL) {
        if (cb) {
            cb(tree->key, tree->value);
        }
        preorder(tree->left, cb);
        preorder(tree->right, cb);
    }
}
// 中序遍历
static void inorder(rbtree_t tree, void (*cb)(void *key, void *value))
{
    if (tree != NULL) {
        inorder(tree->left, cb);
        if (cb) {
            cb(tree->key, tree->value);
        }
        inorder(tree->right, cb);
    }
}
// 后序遍历
static void postorder(rbtree_t tree, void (*cb)(void *key, void *value))
{
    if (tree != NULL) {
        postorder(tree->left, cb);
        postorder(tree->right, cb);
        if (cb) {
            cb(tree->key, tree->value);
        }
    }
}
// 后序遍历
// TODO: 等待实现队列做
static void levelorder(rbtree_t tree, void (*cb)(void *key, void *value))
{
    if (tree == NULL) {
        return;
    }
}

// 红黑数查找(递归)
static node_t *search(rbtree_t x, void *key, int (*cmp_key)(void *key0, void *key1))
{
    if (x == NULL || KEY_EQUAL(x->key, key, cmp_key))
        return x;

    if (KEY_LESS(key, x->key, cmp_key))
        return search(x->left, key, cmp_key);
    else
        return search(x->right, key, cmp_key);
}
// 红黑数查找(迭代)
static node_t *search_iterative(rbtree_t x, void *key, int (*cmp_key)(void *key0, void *key1))
{

    while ((x != NULL) && !KEY_EQUAL(x->key, key, cmp_key)) {
        if (KEY_LESS(key, x->key, cmp_key))
            x = x->left;
        else
            x = x->right;
    }
    return x;
}
// 查找最小节点
static node_t *min_node(rbtree_t tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}
// 查找最大节点
static node_t *max_node(rbtree_t tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}

static void __rbtree_destroy(rbtree_t tree, void (*free_key)(void *key),
                             void (*free_val)(void *key))
{
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        __rbtree_destroy(tree->left, free_key, free_val);
    if (tree->right != NULL)
        __rbtree_destroy(tree->right, free_key, free_val);

    if (tree->key_free_need) {
        free_key(tree->key);
    }
    if (tree->value_free_need) {
        free_val(tree->value);
    }
    free(tree);
}

// 打印红黑数（类似tree命令）
void print_rbtree(rbroot_t *root)
{
    uint8_t arr_flag[RB_PRINT_MAX] = {0};
    print_rbtree_inner(root->node, 0, arr_flag);
}

static void print_rbtree_inner(node_t *node, size_t n_deepth, uint8_t *arr_flag)
{
    if (n_deepth >= RB_PRINT_MAX) {
        return;
    }

    if (n_deepth > 0) {
        for (size_t i = 0; i < n_deepth - 1; i++) {
            printf(arr_flag[i] ? "│   " : "    ");
        }
        printf(arr_flag[n_deepth - 1] ? "├── " : "└── ");
    }
    if (node == NULL) {
        printf("(null)\n");
        return;
    } else {
        if (rb_is_red(node)) {
            printf("%s%ld:red%s\n", RB_PRINT_COLOR ? "\033[31m" : "", (long)node->key,
                   RB_PRINT_COLOR ? "\033[0m" : "");
        } else {
            printf("%s%ld:black%s\n", RB_PRINT_COLOR ? "\033[37m" : "", (long)node->key,
                   RB_PRINT_COLOR ? "\033[0m" : "");
        }
        // printf("%d:%s\n", node->key, rb_is_red(node) ? "red" : "black");
    }

    if (node->left == NULL && node->right == NULL) {
        return;
    }
    arr_flag[n_deepth] = 1;
    print_rbtree_inner(node->right, n_deepth + 1, arr_flag);
    arr_flag[n_deepth] = 0;
    print_rbtree_inner(node->left, n_deepth + 1, arr_flag);
}
