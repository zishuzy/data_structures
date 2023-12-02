#ifndef B_TREE_B_TREE
#define B_TREE_B_TREE

#include <iostream>
#include <list>
#include <queue>
#include <vector>

namespace tree
{
namespace b_tree
{
template <typename _Tp>
struct BTreeNode {
    _Tp val;
    BTreeNode *left;
    BTreeNode *right;

    BTreeNode(_Tp v) : BTreeNode(v, nullptr, nullptr) {}
    BTreeNode(_Tp v, BTreeNode *l, BTreeNode *r) : val(v), left(l), right(r) {}
};

template <typename _Tp>
class BTree
{
public:
    BTree();
    ~BTree();
    void InitTree(const std::vector<_Tp *> vec_data);
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

private:
    void preorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void inorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void postorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void levelorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print);
    void print(BTreeNode<_Tp> *node, std::size_t n_deepth, std::vector<bool> &vec_flag,
               bool b_color);

    void destroy(BTreeNode<_Tp> *tree);

private:
    BTreeNode<_Tp> *m_pNodeRoot; // 根节点
};

// public on
template <typename _Tp>
BTree<_Tp>::BTree() : m_pNodeRoot(nullptr)
{
}

template <typename _Tp>
BTree<_Tp>::~BTree()
{
    destroy(m_pNodeRoot);
    m_pNodeRoot = nullptr;
}

template <typename _Tp>
void BTree<_Tp>::InitTree(const std::vector<_Tp *> vec_data)
{
    if (m_pNodeRoot != nullptr) {
        destroy(m_pNodeRoot);
        m_pNodeRoot = nullptr;
    }
    if (vec_data.empty() || vec_data[0] == nullptr) {
        return;
    }

    m_pNodeRoot = new (std::nothrow) BTreeNode<_Tp>(*vec_data[0]);
    std::queue<std::pair<BTreeNode<_Tp> *, int>> queueNode;
    queueNode.push(std::make_pair(m_pNodeRoot, 0));
    while (!queueNode.empty()) {
        std::pair<BTreeNode<_Tp> *, int> &pairData = queueNode.front();
        size_t idxLeft = pairData.second * 2 + 1;
        size_t idxRight = pairData.second * 2 + 2;
        if (idxLeft < vec_data.size() && vec_data[idxLeft] != nullptr) {
            pairData.first->left = new (std::nothrow) BTreeNode<_Tp>(*vec_data[idxLeft]);
            queueNode.push(std::make_pair(pairData.first->left, idxLeft));
        }
        if (idxRight < vec_data.size() && vec_data[idxRight] != nullptr) {
            pairData.first->right = new (std::nothrow) BTreeNode<_Tp>(*vec_data[idxRight]);
            queueNode.push(std::make_pair(pairData.first->right, idxRight));
        }
        queueNode.pop();
    }
}

template <typename _Tp>
std::list<_Tp> BTree<_Tp>::Preorder(bool b_print)
{
    std::list<_Tp> listOut;
    preorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> BTree<_Tp>::Inorder(bool b_print)
{
    std::list<_Tp> listOut;
    inorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> BTree<_Tp>::Postorder(bool b_print)
{
    std::list<_Tp> listOut;
    postorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}
template <typename _Tp>
std::list<_Tp> BTree<_Tp>::Levelorder(bool b_print)
{
    std::list<_Tp> listOut;
    levelorder(m_pNodeRoot, listOut, b_print);
    if (b_print) {
        std::cout << std::endl;
    }
}

template <typename _Tp>
void BTree<_Tp>::Print(bool b_color)
{
    std::vector<bool> vecFlag;
    print(m_pNodeRoot, 0, vecFlag, b_color);
}
// public off
// private on
template <typename _Tp>
void BTree<_Tp>::preorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
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
void BTree<_Tp>::inorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
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
void BTree<_Tp>::postorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
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
void BTree<_Tp>::levelorder(BTreeNode<_Tp> *tree, std::list<_Tp> &list_out, bool b_print)
{
    if (tree == nullptr) {
        return;
    }
    std::queue<BTreeNode<_Tp> *> queueNode;
    queueNode.push(tree);
    while (!queueNode.empty()) {
        BTreeNode<_Tp> *pNode = queueNode.front();
        queueNode.pop();
        list_out.push_back(pNode->val);
        if (b_print) {
            std::cout << pNode->val << " ";
        }
        if (pNode->pLeftNode != nullptr) {
            queueNode.push(pNode->pLeftNode);
        }
        if (pNode->pRightNode != nullptr) {
            queueNode.push(pNode->pRightNode);
        }
    }
}

template <typename _Tp>
void BTree<_Tp>::print(BTreeNode<_Tp> *node, size_t n_deepth, std::vector<bool> &vec_flag,
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
void BTree<_Tp>::destroy(BTreeNode<_Tp> *tree)
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

} // namespace b_tree
} // namespace tree

#endif /* B_TREE_B_TREE */
