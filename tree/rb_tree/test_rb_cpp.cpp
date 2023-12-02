#include <vector>
#include "common/log/log.h"
#include "rb_tree_cpp.hpp"

int main(int argc, char *argv[])
{
    (void)argc;
    LOG_INFO("start: [%s]", argv[0]);
    std::vector<int> vecData = {10, 40, 30, 60, 90, 70, 20, 50, 80};

    tree::CRBTree<int, std::string> tree;

    for (size_t i = 0; i < vecData.size(); ++i) {
        tree.Insert(vecData[i], std::to_string(vecData[i]));
    }
    tree.Print(true);

    for (size_t i = 0; i < vecData.size(); i++) {
        std::string strRm;
        tree.Remove(vecData[i], strRm);
        LOG_INFO("delete node[%d] data[%s]", vecData[i], strRm.c_str());
        tree.Print(true);
    }
    return 0;
}