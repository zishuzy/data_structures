#include <vector>
#include "avl_tree.hpp"
#include "common/log/log.h"
#include "common/utils/string_utils.hpp"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) ((sizeof(a)) / (sizeof(a[0])))
#endif

int main(int argc, char *argv[])
{
    (void)argc;
    LOG_INFO("start: [%s]", argv[0]);
#if 0
    tree::avl_tree::AVLTree<int> avltree;
    for (int i = 5; i < 15; ++i) {
        bool bSuccess = avltree.Insert(i);
        LOG_INFO("insert node[%d] [%s]", i, string_utils::Bool2str(bSuccess).c_str());
        avltree.Print(true);
    }

    for (int i = 0; i < 10; i++) {
        bool bSuccess = avltree.Remove(i);
        LOG_INFO("delete node[%d] [%s]", i, string_utils::Bool2str(bSuccess).c_str());
        avltree.Print(true);
    }
#endif
    tree::avl_tree::AVLTree<int> avltree;
    for (int i = 5; i < 15; ++i) {
        bool bSuccess = avltree.Insert(i);
        LOG_INFO("insert node[%d] [%s]", i, string_utils::Bool2str(bSuccess).c_str());
    }
    avltree.Print(true);

    tree::avl_tree::AVLTree<int> avltree_2(avltree);
    avltree_2.Print(true);

    return 0;
}