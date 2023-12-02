#ifndef RB_TREE_RB_TREE_C
#define RB_TREE_RB_TREE_C

#define RB_NODE_RED   0 // 红色节点
#define RB_NODE_BLACK 1 // 黑色节点

#define RB_PRINT_MAX   1024 // 打印红黑数最大深度
#define RB_PRINT_COLOR 1    // 打印是否显示颜色

typedef int KeyType;

// 红黑树的节点
typedef struct RBTreeNode {
    KeyType key;
    void *data;
    unsigned char color;
    struct RBTreeNode *left;
    struct RBTreeNode *right;
    struct RBTreeNode *parent;
} Node, *RBTree;

// 红黑树的根
typedef struct RBTreeRoot {
    Node *node;
} RBRoot;

// 创建一个红黑树，返回根节点
RBRoot *rbtree_create();
// 创建红黑树中一个节点
Node *rbtree_create_node(KeyType key, void *data, Node *parent, Node *left, Node *right);
// 前序遍历
void rbtree_preorder(RBRoot *root);
// 中序遍历
void rbtree_inorder(RBRoot *root);
// 后序遍历
void rbtree_postorder(RBRoot *root);
// 层序遍历
void rbtree_levelorder(RBRoot *root);
// 打印红黑数（类似tree命令）
void print_rbtree(RBRoot *root);
// 判断一个key是否存在于树中
int rbtree_is_exist(RBRoot *root, KeyType key);
// 查找一个key对应的Node(递归)，如果没有返回NULL
Node *rbtree_search(RBRoot *root, KeyType key);
// 查找一个key对应的Node(迭代)，如果没有返回NULL
Node *rbtree_search_iterative(RBRoot *root, KeyType key);
// 查找红黑数最小节点
Node *rbtree_min(RBRoot *root);
// 查找红黑数最大节点
Node *rbtree_max(RBRoot *root);
// 插入一个节点
int rbtree_insert(RBRoot *root, KeyType key, void *data);
// 删除一个节点，返回删除节点的data地址，如果没有返回NULL
void *rbtree_delete(RBRoot *root, KeyType key);

#endif /* RB_TREE_RB_TREE_C */
