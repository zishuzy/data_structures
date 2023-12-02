# data_structures

数据结构学习和测试

## 平衡二叉树

| 插入方式 | 描述                                            | 旋转方式     |
| -------- | ----------------------------------------------- | ------------ |
| LL       | 在 A 的左子树根节点的左子树上插入节点而破坏平衡 | 右旋转       |
| RR       | 在 A 的右子树根节点的右子树上插入节点而破坏平衡 | 左旋转       |
| LR       | 在 A 的左子树根节点的右子树上插入节点而破坏平衡 | 先左旋后右旋 |
| RL       | 在 A 的右子树根节点的左子树上插入节点而破坏平衡 | 先右旋后左旋 |

## 红黑树

性质：

* 性质1：每个节点要么是黑色，要么是红色。
* 性质2：根节点是黑色。
* 性质3：每个叶子节点（NIL）是黑色。
* 性质4：每个红色结点的两个子结点一定都是黑色。
* 性质5：任意一结点到每个叶子结点的路径都包含数量相同的黑结点。
