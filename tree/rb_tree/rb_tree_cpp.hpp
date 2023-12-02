#ifndef RB_TREE_RB_TREE_CPP
#define RB_TREE_RB_TREE_CPP

#include <iostream>
#include <list>
#include <queue>

namespace tree
{

enum RBTColor { RBT_RED, RBT_BLACK };

template <typename Tk, typename Tv>
struct SRBTreeNode {
    Tk key;
    Tv data;
    RBTColor color;
    SRBTreeNode *parent;
    SRBTreeNode *left;
    SRBTreeNode *right;

    SRBTreeNode(Tk k, Tv v, RBTColor c, SRBTreeNode *p, SRBTreeNode *l, SRBTreeNode *r)
        : key(k), data(v), color(c), parent(p), left(l), right(r)
    {
    }
};

template <typename Tk, typename Tv>
class CRBTree
{
public:
    CRBTree();
    ~CRBTree();
    // 前序遍历
    std::list<Tk> Preorder(bool b_print);
    // 中序遍历
    std::list<Tk> Inorder(bool b_print);
    // 后序遍历
    std::list<Tk> Postorder(bool b_print);
    // 层序遍历
    std::list<Tk> Levelorder(bool b_print);
    // 打印红黑数（类似tree命令）
    void Print(bool b_color);
    // 判断一个key是否存在于树中
    bool IsExist(Tk key);
    // 查找一个key对应的Node(递归)，如果没有返回NULL
    SRBTreeNode<Tk, Tv> *Search(Tk key);
    // 查找一个key对应的Node(迭代)，如果没有返回NULL
    SRBTreeNode<Tk, Tv> *SearchIterative(Tk key);
    // 查找红黑数最小节点
    SRBTreeNode<Tk, Tv> *Min();
    // 查找红黑数最大节点
    SRBTreeNode<Tk, Tv> *Max();
    // 插入一个节点
    bool Insert(Tk key, Tv value);
    // 删除一个节点
    bool Remove(Tk key, Tv &value);

private:
    void preorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print);
    void inorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print);
    void postorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print);
    void levelorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print);
    void print(SRBTreeNode<Tk, Tv> *node, size_t n_deepth, std::vector<bool> &vec_flag,
               bool b_color);

    SRBTreeNode<Tk, Tv> *search(SRBTreeNode<Tk, Tv> *node, Tk key);
    SRBTreeNode<Tk, Tv> *searchIterative(SRBTreeNode<Tk, Tv> *node, Tk key);
    SRBTreeNode<Tk, Tv> *min(SRBTreeNode<Tk, Tv> *node);
    SRBTreeNode<Tk, Tv> *max(SRBTreeNode<Tk, Tv> *node);

    void leftRotate(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *x);
    void rightRotate(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *y);
    void insert(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node);
    void insertFixUp(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node);
    void remove(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node);
    void removeFixUp(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node,
                     SRBTreeNode<Tk, Tv> *parent);

    void destroy(SRBTreeNode<Tk, Tv> *tree);

private:
    SRBTreeNode<Tk, Tv> *m_pNodeRoot; // 根节点
};

template <typename Tk, typename Tv>
inline SRBTreeNode<Tk, Tv> *rb_parent(SRBTreeNode<Tk, Tv> *r)
{
    return r->parent;
}

template <typename Tk, typename Tv>
inline void rb_set_parent(SRBTreeNode<Tk, Tv> *r, SRBTreeNode<Tk, Tv> *p)
{
    r->parent = p;
}

template <typename Tk, typename Tv>
inline RBTColor rb_color(SRBTreeNode<Tk, Tv> *r)
{
    return r->color;
}

template <typename Tk, typename Tv>
inline bool rb_is_red(SRBTreeNode<Tk, Tv> *r)
{
    return r->color == RBT_RED;
}

template <typename Tk, typename Tv>
inline bool rb_is_black(SRBTreeNode<Tk, Tv> *r)
{
    return r->color == RBT_BLACK;
}

template <typename Tk, typename Tv>
inline void rb_set_red(SRBTreeNode<Tk, Tv> *r)
{
    r->color = RBT_RED;
}

template <typename Tk, typename Tv>
inline void rb_set_black(SRBTreeNode<Tk, Tv> *r)
{
    r->color = RBT_BLACK;
}

template <typename Tk, typename Tv>
inline void rb_set_color(SRBTreeNode<Tk, Tv> *r, RBTColor c)
{
    r->color = c;
}

template <typename Tk, typename Tv>
CRBTree<Tk, Tv>::CRBTree() : m_pNodeRoot(nullptr)
{
}

template <typename Tk, typename Tv>
CRBTree<Tk, Tv>::~CRBTree()
{
    destroy(m_pNodeRoot);
}

template <typename Tk, typename Tv>
std::list<Tk> CRBTree<Tk, Tv>::Preorder(bool b_print)
{
    std::list<Tk> listOut;
    preorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename Tk, typename Tv>
std::list<Tk> CRBTree<Tk, Tv>::Inorder(bool b_print)
{
    std::list<Tk> listOut;
    inorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename Tk, typename Tv>
std::list<Tk> CRBTree<Tk, Tv>::Postorder(bool b_print)
{
    std::list<Tk> listOut;
    postorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename Tk, typename Tv>
std::list<Tk> CRBTree<Tk, Tv>::Levelorder(bool b_print)
{
    std::list<Tk> listOut;
    levelorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::Print(bool b_color)
{
    std::vector<bool> vecFlag;
    print(m_pNodeRoot, 0, vecFlag, b_color);
}

template <typename Tk, typename Tv>
bool CRBTree<Tk, Tv>::IsExist(Tk key)
{
    if (m_pNodeRoot == NULL) {
        return 0;
    }
    return SearchIterative(key) == nullptr ? false : true;
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::Search(Tk key)
{
    return search(m_pNodeRoot, key);
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::SearchIterative(Tk key)
{
    return searchIterative(m_pNodeRoot, key);
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::Min()
{
    return min(m_pNodeRoot);
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::Max()
{
    return max(m_pNodeRoot);
}

template <typename Tk, typename Tv>
bool CRBTree<Tk, Tv>::Insert(Tk key, Tv value)
{
    SRBTreeNode<Tk, Tv> *node =
        new (std::nothrow) SRBTreeNode<Tk, Tv>(key, value, RBT_BLACK, nullptr, nullptr, nullptr);
    if (node == nullptr) {
        return false;
    }
    insert(m_pNodeRoot, node);
    return true;
}

template <typename Tk, typename Tv>
bool CRBTree<Tk, Tv>::Remove(Tk key, Tv &value)
{
    SRBTreeNode<Tk, Tv> *node = search(m_pNodeRoot, key);

    if (node == NULL) {
        return false;
    }
    value = node->data;
    remove(m_pNodeRoot, node);
    return true;
}

// --------------------------- private ---------------------------
template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::preorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print)
{
    if (tree != nullptr) {
        list_out.push_back(tree->key);
        if (b_print) {
            std::cout << tree->key << " ";
        }
        preorder(tree->left);
        preorder(tree->right);
    }
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::inorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print)
{
    if (tree != nullptr) {
        preorder(tree->left);
        list_out.push_back(tree->key);
        if (b_print) {
            std::cout << tree->key << " ";
        }
        preorder(tree->right);
    }
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::postorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print)
{
    if (tree != nullptr) {
        preorder(tree->left);
        preorder(tree->right);
        list_out.push_back(tree->key);
        if (b_print) {
            std::cout << tree->key << " ";
        }
    }
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::levelorder(SRBTreeNode<Tk, Tv> *tree, std::list<Tk> &list_out, bool b_print)
{
    if (tree == nullptr) {
        return;
    }
    std::queue<SRBTreeNode<Tk, Tv> *> queueNode;
    queueNode.push(tree);
    while (!queueNode.empty()) {
        SRBTreeNode<Tk, Tv> *pNode = queueNode.front();
        queueNode.pop();
        list_out.push_back(pNode->val);
        if (b_print) {
            std::cout << pNode->key << " ";
        }
        if (pNode->pLeftNode != nullptr) {
            queueNode.push(pNode->pLeftNode);
        }
        if (pNode->pRightNode != nullptr) {
            queueNode.push(pNode->pRightNode);
        }
    }
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::print(SRBTreeNode<Tk, Tv> *node, size_t n_deepth, std::vector<bool> &vec_flag,
                            bool b_color)
{
    if (n_deepth > 0) {
        for (size_t i = 0; i < n_deepth - 1; i++) {
            printf(vec_flag[i] ? "│   " : "    ");
        }
        printf(vec_flag[n_deepth - 1] ? "├── " : "└── ");
    }
    if (node == NULL) {
        printf("(null)\n");
        return;
    } else {
        if (rb_is_red(node)) {
            std::cout << node->key << ":red" << std::endl;
        } else {
            std::cout << node->key << ":black" << std::endl;
        }
    }

    if (node->left == NULL && node->right == NULL) {
        return;
    }
    if (vec_flag.size() <= n_deepth) {
        vec_flag.push_back(true);
    } else {
        vec_flag[n_deepth] = true;
    }
    print(node->right, n_deepth + 1, vec_flag, b_color);
    vec_flag[n_deepth] = false;
    print(node->left, n_deepth + 1, vec_flag, b_color);
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::search(SRBTreeNode<Tk, Tv> *node, Tk key)
{
    if (node == nullptr || node->key == key)
        return node;

    if (key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::searchIterative(SRBTreeNode<Tk, Tv> *node, Tk key)
{
    while ((node != NULL) && (node->key != key)) {
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::min(SRBTreeNode<Tk, Tv> *node)
{
    if (node == NULL)
        return NULL;

    while (node->left != NULL)
        node = node->left;
    return node;
}

template <typename Tk, typename Tv>
SRBTreeNode<Tk, Tv> *CRBTree<Tk, Tv>::max(SRBTreeNode<Tk, Tv> *node)
{
    if (node == NULL)
        return NULL;

    while (node->right != NULL)
        node = node->right;
    return node;
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::leftRotate(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *x)
{
    SRBTreeNode<Tk, Tv> *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        root = y;
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

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::rightRotate(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *y)
{
    SRBTreeNode<Tk, Tv> *x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        root = y;
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

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::insert(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node)
{
    SRBTreeNode<Tk, Tv> *y = NULL;
    SRBTreeNode<Tk, Tv> *x = root;
    while (x != NULL) {
        y = x;
        if (node->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    rb_set_parent(node, y);
    // rb_parent(node) =s y;
    if (y == NULL) {
        // 插入根节点
        root = node;
    } else if (node->key < y->key) {
        y->left = node;
    } else {
        // 包含了相同值的情况，其实也就是更新
        y->right = node;
    }
    node->color = RBT_RED;
    insertFixUp(root, node);
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::insertFixUp(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node)
{
    SRBTreeNode<Tk, Tv> *parent, *gparent;
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);
        if (parent == gparent->left) { // 父节点是祖父节点的左孩子
            SRBTreeNode<Tk, Tv> *uncle = gparent->right;
            if (uncle && rb_is_red(uncle)) { // case 1: 叔叔节点是红节点
                rb_set_black(parent);        // 将“父节点”设为黑色
                rb_set_black(uncle);         // 将“叔叔节点”设为黑色
                rb_set_red(gparent);         // 将“祖父节点”设为“红色”
                node = gparent; // 将“祖父节点”设为“当前节点”(红色节点)
                continue;       // 之后继续对“当前节点”进行操作。
            }
            // 到这表示叔叔节点是黑节点
            if (parent->right == node) { // case 2: 叔叔是黑色，当前节点是父节点的右孩子
                swap(parent, node);     // 将“父节点”作为“新的当前节点”
                leftRotate(root, node); // 以“新的当前节点”为支点进行左旋
                // 经过上面的左旋后，当前节点一定是左孩子了
            }

            // case 3：叔叔是黑色，当前节点是左孩子
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);

        } else { // 父节点是祖父节点的右孩子
            SRBTreeNode<Tk, Tv> *uncle = gparent->left;
            if (uncle && rb_is_red(uncle)) { // case 1: 叔叔节点是红节点
                rb_set_black(parent);        // 将“父节点”设为黑色
                rb_set_black(uncle);         // 将“叔叔节点”设为黑色
                rb_set_red(gparent);         // 将“祖父节点”设为“红色”
                node = gparent; // 将“祖父节点”设为“当前节点”(红色节点)
                continue;       // 之后继续对“当前节点”进行操作。
            }
            // 到这表示叔叔节点是黑节点
            if (parent->left == node) { // case 2: 叔叔是黑色，当前节点是父节点的左孩子
                swap(parent, node);      // 将“父节点”作为“新的当前节点”
                rightRotate(root, node); // 以“新的当前节点”为支点进行右旋
                // 经过上面的右旋后，当前节点一定是右孩子了
            }
            // case 3：叔叔是黑色，当前节点是右孩子
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }
    rb_set_black(root);
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::remove(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node)
{
    SRBTreeNode<Tk, Tv> *child, *parent;
    int color;
    if (node->left != NULL && node->right != NULL) { // 节点左右孩子都不为空
        // 替代被删除的节点
        SRBTreeNode<Tk, Tv> *replace = node;

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
            root = replace;
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

        if (color == RBT_BLACK) {
            removeFixUp(root, child, parent);
        }
        free(node);
        return;
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
        root = child;
    }

    if (color == RBT_BLACK) {
        removeFixUp(root, child, parent);
    }
    free(node);
    return;
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::removeFixUp(SRBTreeNode<Tk, Tv> *&root, SRBTreeNode<Tk, Tv> *node,
                                  SRBTreeNode<Tk, Tv> *parent)
{
    SRBTreeNode<Tk, Tv> *other;
    while ((!node || rb_is_black(node)) && node != root) { // 调整节点不是根节点
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                // case 1: x的兄弟节点是红色
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
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
                    rightRotate(root, other);
                    other = parent->right;
                }
                // case 4: x的兄弟节点是黑色的；并且兄弟节点的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        } else {
            other = parent->left;
            if (rb_is_red(other)) {
                // case 1: x的兄弟节点是红色
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
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
                    leftRotate(root, other);
                    other = parent->left;
                }
                // case 4: x的兄弟节点是黑色的；并且兄弟节点的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

template <typename Tk, typename Tv>
void CRBTree<Tk, Tv>::destroy(SRBTreeNode<Tk, Tv> *tree)
{
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        return destroy(tree->left);
    if (tree->right != NULL)
        return destroy(tree->right);

    delete tree;
    tree = NULL;
}

} // namespace tree

#endif /* RB_TREE_RB_TREE_CPP */
