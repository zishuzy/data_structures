#ifndef BS_TREE_BS_TREE
#define BS_TREE_BS_TREE

#include <iostream>
#include <list>
#include <queue>
#include <vector>

namespace tree
{
namespace bs_tree
{
template <typename _Tp>
struct BSTreeNode {
    _Tp val;
    BSTreeNode *left;
    BSTreeNode *right;

    BSTreeNode(_Tp v) : BSTreeNode(v, nullptr, nullptr) {}
    BSTreeNode(_Tp v, BSTreeNode *l, BSTreeNode *r) : val(v), left(l), right(r) {}
};

template <typename _Tp>
class BSTree
{
public:
    BSTree();
    ~BSTree();
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
    BSTreeNode<_Tp> *Search(_Tp val);
    // 查找一个key对应的Node(迭代)，如果没有返回NULL
    BSTreeNode<_Tp> *SearchIterative(_Tp val);
    // 查找红黑数最小节点
    BSTreeNode<_Tp> *Min();
    // 查找红黑数最大节点
    BSTreeNode<_Tp> *Max();
    // 插入一个节点
    bool Insert(_Tp val);
    // 删除一个节点
    bool Remove(_Tp val);

private:
    void preorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void inorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void postorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void levelorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void print(BSTreeNode<_Tp> *node, std::size_t n_deepth, std::vector<bool> &vec_flag,
               bool b_color);

    BSTreeNode<_Tp> *search(BSTreeNode<_Tp> *node, _Tp val);
    BSTreeNode<_Tp> *searchIterative(BSTreeNode<_Tp> *node, _Tp val);
    BSTreeNode<_Tp> *min(BSTreeNode<_Tp> *node);
    BSTreeNode<_Tp> *max(BSTreeNode<_Tp> *node);

    BSTreeNode<_Tp> *insert(BSTreeNode<_Tp> *root, _Tp val, bool &b_insert);
    BSTreeNode<_Tp> *remove(BSTreeNode<_Tp> *root, _Tp val, bool &b_del);
    void destroy(BSTreeNode<_Tp> *tree);

private:
    BSTreeNode<_Tp> *m_pNodeRoot; // 根节点
};

// public on
template <typename _Tp>
BSTree<_Tp>::BSTree() : m_pNodeRoot(nullptr)
{
}

template <typename _Tp>
BSTree<_Tp>::~BSTree()
{
    destroy(m_pNodeRoot);
    m_pNodeRoot = nullptr;
}

template <typename _Tp>
std::list<_Tp> BSTree<_Tp>::Preorder(bool b_print)
{
    std::list<_Tp> listOut;
    preorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> BSTree<_Tp>::Inorder(bool b_print)
{
    std::list<_Tp> listOut;
    inorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> BSTree<_Tp>::Postorder(bool b_print)
{
    std::list<_Tp> listOut;
    postorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> BSTree<_Tp>::Levelorder(bool b_print)
{
    std::list<_Tp> listOut;
    levelorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}

template <typename _Tp>
void BSTree<_Tp>::Print(bool b_color)
{
    std::vector<bool> vecFlag;
    print(m_pNodeRoot, 0, vecFlag, b_color);
}

template <typename _Tp>
bool BSTree<_Tp>::IsExist(_Tp val)
{
    if (m_pNodeRoot == NULL) {
        return 0;
    }
    return SearchIterative(val) == nullptr ? false : true;
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::Search(_Tp val)
{
    return search(m_pNodeRoot, val);
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::SearchIterative(_Tp val)
{
    return searchIterative(m_pNodeRoot, val);
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::Min()
{
    return min(m_pNodeRoot);
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::Max()
{
    return max(m_pNodeRoot);
}

template <typename _Tp>
bool BSTree<_Tp>::Insert(_Tp val)
{
    bool bResult = false;
    m_pNodeRoot = insert(m_pNodeRoot, val, bResult);
    return bResult;
}

template <typename _Tp>
bool BSTree<_Tp>::Remove(_Tp val)
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
void BSTree<_Tp>::preorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
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
void BSTree<_Tp>::inorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
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
void BSTree<_Tp>::postorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
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
void BSTree<_Tp>::levelorder(BSTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
{
    if (tree == nullptr) {
        return;
    }
    std::queue<BSTreeNode<_Tp> *> queueNode;
    queueNode.push(tree);
    while (!queueNode.empty()) {
        BSTreeNode<_Tp> *pNode = queueNode.front();
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
void BSTree<_Tp>::print(BSTreeNode<_Tp> *node, size_t n_deepth, std::vector<bool> &vec_flag,
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
BSTreeNode<_Tp> *BSTree<_Tp>::search(BSTreeNode<_Tp> *node, _Tp val)
{
    if (node == nullptr || node->val == val)
        return node;

    if (val < node->val)
        return search(node->left, val);
    else
        return search(node->right, val);
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::searchIterative(BSTreeNode<_Tp> *node, _Tp val)
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
BSTreeNode<_Tp> *BSTree<_Tp>::min(BSTreeNode<_Tp> *node)
{
    if (node == NULL)
        return NULL;

    while (node->left != NULL)
        node = node->left;
    return node;
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::max(BSTreeNode<_Tp> *node)
{
    if (node == NULL)
        return NULL;

    while (node->right != NULL)
        node = node->right;
    return node;
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::insert(BSTreeNode<_Tp> *root, _Tp val, bool &b_insert)
{
    if (root == nullptr) {
        BSTreeNode<_Tp> *node = new (std::nothrow) BSTreeNode<_Tp>(val);
        if (node == nullptr) {
            b_insert = false;
        } else {
            b_insert = true;
        }
        return node;
    }

    if (root->val > val) {
        root->left = insert(root->left, val, b_insert);
    } else if (root->val < val) {
        root->right = insert(root->right, val, b_insert);
    } else {
        b_insert = true;
    }
    return root;
}

template <typename _Tp>
BSTreeNode<_Tp> *BSTree<_Tp>::remove(BSTreeNode<_Tp> *root, _Tp val, bool &b_del)
{
    if (root == nullptr) {
        return nullptr;
    }

    if (root->val > val) {
        root->left = remove(root->left, val, b_del);
    } else if (root->val < val) {
        root->right = remove(root->right, val, b_del);
    } else if (root->left != nullptr && root->right != nullptr) {
        BSTreeNode<_Tp> *replace = nullptr;
        replace = min(root->right);
        root->val = replace->val;
        root->right = remove(root->right, replace->val, b_del);
    } else {
        BSTreeNode<_Tp> *tmp = root;
        if (root->left == nullptr) {
            root = root->right;
        } else if (root->right == nullptr) {
            root = root->left;
        }
        b_del = true;
        delete tmp;
    }
    return root;
}

template <typename _Tp>
void BSTree<_Tp>::destroy(BSTreeNode<_Tp> *tree)
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

} // namespace bs_tree
} // namespace tree

#endif /* BS_TREE_BS_TREE */
