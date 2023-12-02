#ifndef AVL_TREE_AVL_TREE
#define AVL_TREE_AVL_TREE

#include <iostream>
#include <list>
#include <queue>
#include <vector>

namespace tree
{
namespace avl_tree
{
template <typename _Tp>
struct AVLTreeNode {
    _Tp val;
    AVLTreeNode *left;
    AVLTreeNode *right;

    AVLTreeNode(_Tp v) : AVLTreeNode(v, nullptr, nullptr) {}
    AVLTreeNode(_Tp v, AVLTreeNode *l, AVLTreeNode *r) : val(v), left(l), right(r) {}
};

template <typename _Tp>
class AVLTree
{
public:
    AVLTree();
    AVLTree(AVLTree &tree);
    ~AVLTree();
    // 前序遍历
    std::list<_Tp> Preorder(bool b_print);
    // 中序遍历
    std::list<_Tp> Inorder(bool b_print);
    // 后序遍历
    std::list<_Tp> Postorder(bool b_print);
    // 层序遍历
    std::list<_Tp> Levelorder(bool b_print);
    // 打印（类似tree命令）
    void Print(bool b_color);
    // 判断一个val是否存在于树中
    bool IsExist(_Tp val);
    // 查找一个key对应的Node(递归)，如果没有返回NULL
    AVLTreeNode<_Tp> *Search(_Tp val);
    // 查找一个key对应的Node(迭代)，如果没有返回NULL
    AVLTreeNode<_Tp> *SearchIterative(_Tp val);
    // 查找红黑数最小节点
    AVLTreeNode<_Tp> *Min();
    // 查找红黑数最大节点
    AVLTreeNode<_Tp> *Max();
    // 插入一个节点
    bool Insert(_Tp val);
    // 删除一个节点
    bool Remove(_Tp val);

private:
    void preorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void inorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void postorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void levelorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void print(AVLTreeNode<_Tp> *node, std::size_t n_deepth, std::vector<bool> &vec_flag,
               bool b_color);

    AVLTreeNode<_Tp> *search(AVLTreeNode<_Tp> *node, _Tp val);
    AVLTreeNode<_Tp> *searchIterative(AVLTreeNode<_Tp> *node, _Tp val);
    AVLTreeNode<_Tp> *min(AVLTreeNode<_Tp> *node);
    AVLTreeNode<_Tp> *max(AVLTreeNode<_Tp> *node);

    int height(AVLTreeNode<_Tp> *p_root);
    int balanceFactor(AVLTreeNode<_Tp> *p_root);
    AVLTreeNode<_Tp> *rotationRR(AVLTreeNode<_Tp> *p_root);
    AVLTreeNode<_Tp> *rotationLL(AVLTreeNode<_Tp> *p_root);
    AVLTreeNode<_Tp> *rotationLR(AVLTreeNode<_Tp> *p_root);
    AVLTreeNode<_Tp> *rotationRL(AVLTreeNode<_Tp> *p_root);
    AVLTreeNode<_Tp> *balance(AVLTreeNode<_Tp> *p_root);

    AVLTreeNode<_Tp> *insert(AVLTreeNode<_Tp> *root, _Tp val, bool &b_insert);
    AVLTreeNode<_Tp> *remove(AVLTreeNode<_Tp> *root, _Tp val, bool &b_del);
    AVLTreeNode<_Tp> *copy(AVLTreeNode<_Tp> *root);
    void destroy(AVLTreeNode<_Tp> *tree);

private:
    AVLTreeNode<_Tp> *m_pNodeRoot; // 根节点
};

// public on
template <typename _Tp>
AVLTree<_Tp>::AVLTree() : m_pNodeRoot(nullptr)
{
}

template <typename _Tp>
AVLTree<_Tp>::AVLTree(AVLTree &tree)
{
    m_pNodeRoot = copy(tree.m_pNodeRoot);
}

template <typename _Tp>
AVLTree<_Tp>::~AVLTree()
{
    destroy(m_pNodeRoot);
    m_pNodeRoot = nullptr;
}

template <typename _Tp>
std::list<_Tp> AVLTree<_Tp>::Preorder(bool b_print)
{
    std::list<_Tp> listOut;
    preorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> AVLTree<_Tp>::Inorder(bool b_print)
{
    std::list<_Tp> listOut;
    inorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> AVLTree<_Tp>::Postorder(bool b_print)
{
    std::list<_Tp> listOut;
    postorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> AVLTree<_Tp>::Levelorder(bool b_print)
{
    std::list<_Tp> listOut;
    levelorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}

template <typename _Tp>
void AVLTree<_Tp>::Print(bool b_color)
{
    std::vector<bool> vecFlag;
    print(m_pNodeRoot, 0, vecFlag, b_color);
}

template <typename _Tp>
bool AVLTree<_Tp>::IsExist(_Tp val)
{
    if (m_pNodeRoot == NULL) {
        return 0;
    }
    return SearchIterative(val) == nullptr ? false : true;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::Search(_Tp val)
{
    return search(m_pNodeRoot, val);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::SearchIterative(_Tp val)
{
    return searchIterative(m_pNodeRoot, val);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::Min()
{
    return min(m_pNodeRoot);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::Max()
{
    return max(m_pNodeRoot);
}

template <typename _Tp>
bool AVLTree<_Tp>::Insert(_Tp val)
{
    bool bResult = false;
    m_pNodeRoot = insert(m_pNodeRoot, val, bResult);
    return bResult;
}

template <typename _Tp>
bool AVLTree<_Tp>::Remove(_Tp val)
{
    if (m_pNodeRoot == nullptr) {
        return false;
    }
    bool bResult = false;
    m_pNodeRoot = remove(m_pNodeRoot, val, bResult);
    return bResult;
}
// public off
// private on
template <typename _Tp>
void AVLTree<_Tp>::preorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
{
    if (tree != nullptr) {
        list_out.push_back(tree->val);
        if (b_print) {
            std::cout << tree->val << " ";
        }
        preorder(tree->left);
        preorder(tree->right);
    }
}

template <typename _Tp>
void AVLTree<_Tp>::inorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
{
    if (tree != nullptr) {
        preorder(tree->left);
        list_out.push_back(tree->val);
        if (b_print) {
            std::cout << tree->val << " ";
        }
        preorder(tree->right);
    }
}

template <typename _Tp>
void AVLTree<_Tp>::postorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
{
    if (tree != nullptr) {
        preorder(tree->left);
        preorder(tree->right);
        list_out.push_back(tree->val);
        if (b_print) {
            std::cout << tree->val << " ";
        }
    }
}

template <typename _Tp>
void AVLTree<_Tp>::levelorder(AVLTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
{
    if (tree == nullptr) {
        return;
    }
    std::queue<AVLTreeNode<_Tp> *> queueNode;
    queueNode.push(tree);
    while (!queueNode.empty()) {
        AVLTreeNode<_Tp> *pNode = queueNode.front();
        queueNode.pop();
        list_out.push_back(pNode->val);
        if (b_print) {
            std::cout << pNode->val << " ";
        }
        if (pNode->left != nullptr) {
            queueNode.push(pNode->left);
        }
        if (pNode->right != nullptr) {
            queueNode.push(pNode->right);
        }
    }
}

template <typename _Tp>
void AVLTree<_Tp>::print(AVLTreeNode<_Tp> *node, size_t n_deepth, std::vector<bool> &vec_flag,
                         bool b_color)
{
    if (n_deepth > 0) {
        for (size_t i = 0; i < n_deepth - 1; i++) {
            printf(vec_flag[i] ? "│   " : "    ");
        }
        printf(vec_flag[n_deepth - 1] ? "├── " : "└── ");
    }
    if (node == nullptr) {
        printf("(null)\n");
        return;
    } else {
        std::cout << node->val << std::endl;
    }

    if (node->left == nullptr && node->right == nullptr) {
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

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::search(AVLTreeNode<_Tp> *node, _Tp val)
{
    if (node == nullptr || node->val == val)
        return node;

    if (val < node->val)
        return search(node->left, val);
    else
        return search(node->right, val);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::searchIterative(AVLTreeNode<_Tp> *node, _Tp val)
{
    while ((node != NULL) && (node->val != val)) {
        if (val < node->val)
            node = node->left;
        else
            node = node->right;
    }
    return node;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::min(AVLTreeNode<_Tp> *node)
{
    if (node == NULL)
        return NULL;

    while (node->left != NULL)
        node = node->left;
    return node;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::max(AVLTreeNode<_Tp> *node)
{
    if (node == NULL)
        return NULL;

    while (node->right != NULL)
        node = node->right;
    return node;
}

template <typename _Tp>
int AVLTree<_Tp>::height(AVLTreeNode<_Tp> *p_root)
{
    if (p_root == nullptr) {
        return 0;
    }
    return std::max(height(p_root->left), height(p_root->right)) + 1;
}

template <typename _Tp>
int AVLTree<_Tp>::balanceFactor(AVLTreeNode<_Tp> *p_root)
{
    return height(p_root->left) - height(p_root->right);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::rotationRR(AVLTreeNode<_Tp> *p_root)
{
    AVLTreeNode<_Tp> *pNodeTmp;
    pNodeTmp = p_root->right;
    p_root->right = pNodeTmp->left;
    pNodeTmp->left = p_root;
    return pNodeTmp;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::rotationLL(AVLTreeNode<_Tp> *p_root)
{
    AVLTreeNode<_Tp> *pNodeTmp;
    pNodeTmp = p_root->left;
    p_root->left = pNodeTmp->right;
    pNodeTmp->right = p_root;
    return pNodeTmp;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::rotationLR(AVLTreeNode<_Tp> *p_root)
{
    p_root->left = rotationRR(p_root->left);
    return rotationLL(p_root);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::rotationRL(AVLTreeNode<_Tp> *p_root)
{
    p_root->right = rotationLL(p_root->right);
    return rotationRR(p_root);
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::balance(AVLTreeNode<_Tp> *p_root)
{
    int heightDirr = balanceFactor(p_root);    // 计算平衡因子（左右子树高度差）
    if (heightDirr > 1) {                      // 左子树高于右子树
        if (balanceFactor(p_root->left) > 0) { // 左左外侧
            p_root = rotationLL(p_root);       // 右旋
        } else {                               // 左右内侧
            p_root = rotationLR(p_root);       // 先左旋后右旋
        }
    } else if (heightDirr < -1) {               // 右子树高于左子树
        if (balanceFactor(p_root->right) > 0) { // 右左内侧
            p_root = rotationRL(p_root);        // 先右旋后左旋
        } else {                                // 右右外侧
            p_root = rotationRR(p_root);        // 左旋
        }
    }
    return p_root;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::insert(AVLTreeNode<_Tp> *root, _Tp val, bool &b_insert)
{
    if (root == nullptr) {
        AVLTreeNode<_Tp> *node = new (std::nothrow) AVLTreeNode<_Tp>(val);
        if (node == nullptr) {
            b_insert = false;
        } else {
            b_insert = true;
        }
        return node;
    }

    if (root->val > val) {
        root->left = insert(root->left, val, b_insert);
        root = balance(root);
    } else if (root->val < val) {
        root->right = insert(root->right, val, b_insert);
        root = balance(root);
    } else {
        b_insert = true;
    }
    return root;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::remove(AVLTreeNode<_Tp> *root, _Tp val, bool &b_del)
{
    if (root == nullptr) {
        return nullptr;
    }
    if (root->val == val) {
        if (root->left != nullptr && root->right != nullptr) {
            if (balanceFactor(root) > 0) { // 左树高
                root->val = max(root->left)->val;
                root->left = remove(root->left, root->val, b_del);
            } else {
                root->val = min(root->right)->val;
                root->right = remove(root->right, root->val, b_del);
            }
        } else {
            AVLTreeNode<_Tp> *pNodeTmp = root;
            root = root->left == nullptr ? root->right : root->left;
            b_del = true;
            delete pNodeTmp;
        }
    } else if (root->val > val) {
        root->left = remove(root->left, val, b_del);
        root = balance(root);
    } else {
        root->right = remove(root->right, val, b_del);
        root = balance(root);
    }
    return root;
}

template <typename _Tp>
AVLTreeNode<_Tp> *AVLTree<_Tp>::copy(AVLTreeNode<_Tp> *root)
{
    if (root == nullptr) {
        return nullptr;
    }
    AVLTreeNode<_Tp> *rootNew = new (std::nothrow) AVLTreeNode<_Tp>(root->val);
    if (rootNew == nullptr) {
        return nullptr;
    }
    if (root->left != nullptr) {
        rootNew->left = copy(root->left);
    }
    if (root->right != nullptr) {
        rootNew->right = copy(root->right);
    }
    return rootNew;
}

template <typename _Tp>
void AVLTree<_Tp>::destroy(AVLTreeNode<_Tp> *tree)
{
    if (tree == nullptr)
        return;

    if (tree->left != nullptr)
        return destroy(tree->left);
    if (tree->right != nullptr)
        return destroy(tree->right);

    delete tree;
    tree = nullptr;
}
// private off

} // namespace avl_tree
} // namespace tree

#endif /* AVL_TREE_AVL_TREE */
