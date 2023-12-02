#include <vector>
#include "b_tree.hpp"
#include "common/log/log.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) ((sizeof(a)) / (sizeof(a[0])))
#endif

int main(int argc, char *argv[])
{
    (void)argc;
    LOG_INFO("start: [%s]", argv[0]);
    // int arrData[] = {10, 0, 30, 60, 90, 70, 20, 50, 80};
    int arrData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 10};
    std::vector<int *> vecData;

    tree::b_tree::BTree<int> btree;
    for (size_t i = 0; i < ARRAY_SIZE(arrData); ++i) {
        if (arrData[i] == 0) {
            vecData.push_back(nullptr);
        } else {
            vecData.push_back(&arrData[i]);
        }
    }
    btree.InitTree(vecData);
    btree.Print(true);

    // for (size_t i = 0; i < vecData.size(); i++) {
    //     std::string strRm;
    //     tree.Remove(vecData[i], strRm);
    //     LOG_INFO("delete node[%d] data[%s]", vecData[i], strRm.c_str());
    //     tree.Print(true);
    // }
    return 0;
}