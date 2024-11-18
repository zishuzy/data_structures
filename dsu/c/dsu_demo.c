/**
 * @file dsu_demo.c
 * @author zishu (zishuzy@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-11-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>

// 并查集结构体
typedef struct dsu {
    int *parent; // 父节点数组
    int *rank;   // 秩数组，用于按秩合并
    int size;    // 集合大小
} dsu_t;

// 初始化并查集
dsu_t *dsu_create(int size)
{
    dsu_t *uf = (dsu_t *)malloc(sizeof(dsu_t));
    uf->size = size;
    uf->parent = (int *)malloc(size * sizeof(int));
    uf->rank = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; ++i) {
        uf->parent[i] = i; // 每个节点的初始父节点是自己
        uf->rank[i] = 0;   // 初始秩为 0
    }
    return uf;
}

// 查找操作（路径压缩）
int dsu_find(dsu_t *uf, int x)
{
    if (uf->parent[x] != x) {
        uf->parent[x] = dsu_find(uf, uf->parent[x]); // 路径压缩
    }
    return uf->parent[x];
}

// 合并操作（按秩合并）
void dsu_union(dsu_t *uf, int x, int y)
{
    int root_x = dsu_find(uf, x);
    int root_y = dsu_find(uf, y);

    if (root_x != root_y) {
        if (uf->rank[root_x] > uf->rank[root_y]) {
            uf->parent[root_y] = root_x;
        } else if (uf->rank[root_x] < uf->rank[root_y]) {
            uf->parent[root_x] = root_y;
        } else {
            uf->parent[root_y] = root_x;
            uf->rank[root_x]++;
        }
    }
}

// 判断两个元素是否属于同一集合
int dsu_connected(dsu_t *uf, int x, int y)
{
    return dsu_find(uf, x) == dsu_find(uf, y);
}

// 释放内存
void dsu_destroy(dsu_t *uf)
{
    free(uf->parent);
    free(uf->rank);
    free(uf);
}

// 主函数测试
int main()
{
    int n = 10; // 并查集中元素个数
    dsu_t *uf = dsu_create(n);

    // 合并操作
    dsu_union(uf, 0, 1);
    dsu_union(uf, 1, 2);
    dsu_union(uf, 3, 4);
    dsu_union(uf, 4, 5);

    // 查询连通性
    printf("0 和 2 是否连通: %s\n", dsu_connected(uf, 0, 2) ? "是" : "否");
    printf("2 和 3 是否连通: %s\n", dsu_connected(uf, 2, 3) ? "是" : "否");
    printf("3 和 5 是否连通: %s\n", dsu_connected(uf, 3, 5) ? "是" : "否");

    // 合并更多集合
    dsu_union(uf, 2, 3);
    printf("2 和 3 是否连通: %s\n", dsu_connected(uf, 2, 3) ? "是" : "否");

    // 释放资源
    dsu_destroy(uf);

    return 0;
}
