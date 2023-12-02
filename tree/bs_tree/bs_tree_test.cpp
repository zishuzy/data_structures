#include <vector>
#include "bs_tree.hpp"
#include "common/log/log.h"
#include "common/utils/string_utils.hpp"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) ((sizeof(a)) / (sizeof(a[0])))
#endif

int main(int argc, char *argv[])
{
    (void)argc;
    LOG_INFO("start: [%s]", argv[0]);
    std::vector<int> vecData = {4, 9, 2, 1, 5, 8};

    tree::bs_tree::BSTree<int> btree;
    for (size_t i = 0; i < vecData.size(); ++i) {
        btree.Insert(vecData[i]);
    }
    btree.Print(true);

    for (int i = 0; i < 10; i++) {
        bool bSuccess = btree.Remove(i);
        LOG_INFO("delete node[%d] [%s]", i, string_utils::Bool2str(bSuccess).c_str());
        btree.Print(true);
    }
    return 0;
}