#include "rb_tree_c.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "common/log/log.h"

// ------------------------ define ------------------------
// clang-format off
#define rb_parent(r)       ((r)->parent)
#define rb_color(r)        ((r)->color)
#define rb_is_red(r)       ((r)->color == RB_NODE_RED)
#define rb_is_black(r)     ((r)->color == RB_NODE_BLACK)
#define rb_set_black(r)    do { (r)->color = RB_NODE_BLACK; } while (0)
#define rb_set_red(r)      do { (r)->color = RB_NODE_RED; } while (0)
#define rb_set_parent(r,p) do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)
#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)
#define ARRAY_SIZE(a)      ( (sizeof(a)) / (sizeof(a[0])) )
// clang-format on

// ------------------------ declaration ------------------------
static void rbtree_left_rotate(RBRoot *root, Node *x);
static void rbtree_right_rotate(RBRoot *root, Node *y);
static void rbtree_insert_node(RBRoot *root, Node *node);
static void rbtree_insert_fixup(RBRoot *root, Node *node);
static void *rbtree_delete_node(RBRoot *root, Node *node);
static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent);
static void preorder(RBTree tree);
static void inorder(RBTree tree);
static void postorder(RBTree tree);
static void levelorder(RBTree tree);
static Node *search(RBTree x, KeyType key);
static Node *search_iterative(RBTree x, KeyType key);
static Node *min_node(RBTree tree);
static Node *max_node(RBTree tree);
static void destroy_rbtree(RBTree tree);
static void print_rbtree_inner(Node *node, size_t n_deepth, uint8_t *arr_flag);

// ------------------------ public ------------------------

// 创建一个红黑树，返回根节点
RBRoot *rbtree_create()
{
    RBRoot *root = (RBRoot *)malloc(sizeof(RBRoot));
    root->node = NULL;
    return root;
}
// 创建红黑树中一个节点
Node *rbtree_create_node(KeyType key, void *data, Node *parent, Node *left, Node *right)
{
    Node *p;
    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    p->key = key;
    p->data = data;
    p->left = left;
    p->right = right;
    p->parent = parent;
    p->color = RB_NODE_BLACK; // 默认为黑色
    return p;
}
// 销毁红黑数
void rbtree_destroy(RBRoot *root)
{
    if (root != NULL)
        destroy_rbtree(root->node);

    free(root);
}
// 前序遍历
void rbtree_preorder(RBRoot *root)
{
    if (root)
        preorder(root->node);
}
// 中序遍历
void rbtree_inorder(RBRoot *root)
{
    if (root)
        inorder(root->node);
}
// 后序遍历
void rbtree_postorder(RBRoot *root)
{
    if (root)
        postorder(root->node);
}
// 层序遍历
void rbtree_levelorder(RBRoot *root)
{
    if (root)
        levelorder(root->node);
}
// 打印红黑数（类似tree命令）
void print_rbtree(RBRoot *root)
{
    uint8_t arr_flag[RB_PRINT_MAX] = {0};
    print_rbtree_inner(root->node, 0, arr_flag);
}

// 判断一个key是否存在于树中
int rbtree_is_exist(RBRoot *root, KeyType key)
{
    if (root == NULL) {
        return 0;
    }
    return search_iterative(root->node, key) ? 1 : 0;
}
// 查找一个key对应的Node(递归)，如果没有返回NULL
Node *rbtree_search(RBRoot *root, KeyType key)
{
    if (root == NULL) {
        return NULL;
    }
    return search(root->node, key);
}
// 查找一个key对应的Node(迭代)，如果没有返回NULL
Node *rbtree_search_iterative(RBRoot *root, KeyType key)
{
    if (root == NULL) {
        return NULL;
    }
    return search_iterative(root->node, key);
}
// 查找红黑数最小节点
Node *rbtree_min(RBRoot *root)
{
    if (root == NULL) {
        return NULL;
    }
    return min_node(root->node);
}
// 查找红黑数最大节点
Node *rbtree_max(RBRoot *root)
{
    if (root == NULL) {
        return NULL;
    }
    return max_node(root->node);
}
// 插入一个节点
int rbtree_insert(RBRoot *root, KeyType key, void *data)
{
    Node *node; // 新建结点

    // 不允许插入相同键值的节点。
    // (若想允许插入相同键值的节点，注释掉下面两句话即可！)
    if (search(root->node, key) != NULL)
        return -1;

    // 如果新建结点失败，则返回。
    if ((node = rbtree_create_node(key, data, NULL, NULL, NULL)) == NULL)
        return -1;

    rbtree_insert_node(root, node);
    return 0;
}
// 删除一个节点
void *rbtree_delete(RBRoot *root, KeyType key)
{
    Node *z;
    if ((z = search(root->node, key)) == NULL) {
        return NULL;
    }
    return rbtree_delete_node(root, z);
}

// ------------------------ private ------------------------
// 左旋
/*      p              p
 *      |              |
 *      x              y
 *     / \            / \
 *    a   y    -->   x   c
 *       / \        / \
 *      b   c      a   b
 */
static void rbtree_left_rotate(RBRoot *root, Node *x)
{
    Node *y = x->right;
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
static void rbtree_right_rotate(RBRoot *root, Node *y)
{
    Node *x = y->left;
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
static void rbtree_insert_node(RBRoot *root, Node *node)
{
    Node *y = NULL;
    Node *x = root->node;
    while (x != NULL) {
        y = x;
        if (node->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    rb_parent(node) = y;
    if (y == NULL) {
        // 插入根节点
        root->node = node;
    } else if (node->key < y->key) {
        y->left = node;
    } else {
        // 包含了相同值的情况，其实也就是更新
        y->right = node;
    }
    node->color = RB_NODE_RED;
    rbtree_insert_fixup(root, node);
}

// 红黑树插入节点后修正
static void rbtree_insert_fixup(RBRoot *root, Node *node)
{
    Node *parent, *gparent;
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);
        if (parent == gparent->left) { // 父节点是祖父节点的左孩子
            Node *uncle = gparent->right;
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
            Node *uncle = gparent->left;
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
static void *rbtree_delete_node(RBRoot *root, Node *node)
{
    Node *child, *parent;
    void *result = NULL;
    int color;
    if (node->left != NULL && node->right != NULL) { // 节点左右孩子都不为空
        // 替代被删除的节点
        Node *replace = node;

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
        result = node->data;
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
    result = node->data;
    free(node);
    return result;
}
// 红黑树删除后的修正
static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
    Node *other;
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
static void preorder(RBTree tree)
{
    if (tree != NULL) {
        printf("%d ", tree->key);
        preorder(tree->left);
        preorder(tree->right);
    }
}
// 中序遍历
static void inorder(RBTree tree)
{
    if (tree != NULL) {
        inorder(tree->left);
        printf("%d ", tree->key);
        inorder(tree->right);
    }
}
// 后序遍历
static void postorder(RBTree tree)
{
    if (tree != NULL) {
        postorder(tree->left);
        postorder(tree->right);
        printf("%d ", tree->key);
    }
}
// 后序遍历
// TODO: 等待实现队列做
static void levelorder(RBTree tree)
{
    if (tree == NULL) {
        return;
    }
}
// 红黑数查找(递归)
static Node *search(RBTree x, KeyType key)
{
    if (x == NULL || x->key == key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}
// 红黑数查找(迭代)
static Node *search_iterative(RBTree x, KeyType key)
{
    while ((x != NULL) && (x->key != key)) {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}
// 查找最小节点
static Node *min_node(RBTree tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}
// 查找最大节点
static Node *max_node(RBTree tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}

static void destroy_rbtree(RBTree tree)
{
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        destroy_rbtree(tree->left);
    if (tree->right != NULL)
        destroy_rbtree(tree->right);

    free(tree);
}

static void print_rbtree_inner(Node *node, size_t n_deepth, uint8_t *arr_flag)
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
            printf("%s%d:red%s\n", RB_PRINT_COLOR ? "\033[31m" : "", node->key,
                   RB_PRINT_COLOR ? "\033[0m" : "");
        } else {
            printf("%s%d:black%s\n", RB_PRINT_COLOR ? "\033[37m" : "", node->key,
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
