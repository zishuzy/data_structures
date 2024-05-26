#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "slinked_list.h"

struct slinked_list_head *create_by_list(const std::vector<long> &vec_data)
{
    struct slinked_list_head *head = slist_create();

    if (!head) {
        return head;
    }

    for (auto data : vec_data) {
        struct slinked_list_node *node;
        node = slist_node_create((void *)data);
        slist_insert_node_in_end(head, node);
    }
    return head;
}

bool slist_eq_data(struct slinked_list_head *head, const std::vector<long> &vec_data)
{
    bool res = false;

    do {
        if (head->size != vec_data.size()) {
            break;
        }
        if (!vec_data.empty()) {
            if (head->first->data != (void *)vec_data.front() ||
                head->last->data != (void *)vec_data.back()) {
                break;
            }
        }
        struct slinked_list_node *node = head->first;
        uint64_t i = 0;
        for (; i < vec_data.size() && node; i++, node = node->next) {
            if ((long)(node->data) != vec_data[i]) {
                break;
            }
        }
        if (node || i != vec_data.size()) {
            break;
        }

        res = true;
    } while (false);

    return res;
}

TEST(slinked_list, create)
{
    struct slinked_list_head *head = slist_create();

    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->size, 0);
    EXPECT_EQ(head->first, nullptr);
    EXPECT_EQ(head->last, nullptr);
}

TEST(slinked_list, create_free_node)
{
    struct slinked_list_node *node;
    void *data;

    node = slist_node_create((void *)5555);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->next, nullptr);
    EXPECT_EQ(node->data, (void *)5555);

    data = slist_node_free(node);
    EXPECT_EQ(data, (void *)5555);
}

TEST(slinked_list, insert_node_in_start)
{
    struct slinked_list_head *head = slist_create();
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int ret;

    ASSERT_NE(head, nullptr);

    for (auto data : vec_data) {
        struct slinked_list_node *node;
        node = slist_node_create((void *)data);
        ASSERT_NE(node, nullptr);
        ret = slist_insert_node_in_start(head, node);
        EXPECT_EQ(ret, 0);
    }

    ASSERT_EQ(head->size, vec_data.size());
    EXPECT_EQ(head->first->data, (void *)vec_data.back());
    EXPECT_EQ(head->last->data, (void *)vec_data.front());

    struct slinked_list_node *node = head->first;
    int i = vec_data.size() - 1;
    for (; i >= 0 && node; i--) {
        EXPECT_EQ(node->data, (void *)vec_data[i]) << (long)node->data << " != " << vec_data[i];
        node = node->next;
    }
    EXPECT_EQ(i, -1);
    EXPECT_EQ(node, nullptr);
}

TEST(slinked_list, insert_node_in_end)
{
    struct slinked_list_head *head = slist_create();
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int ret;

    ASSERT_NE(head, nullptr);

    for (auto data : vec_data) {
        struct slinked_list_node *node;
        node = slist_node_create((void *)data);
        ASSERT_NE(node, nullptr);
        ret = slist_insert_node_in_end(head, node);
        EXPECT_EQ(ret, 0);
    }

    ASSERT_EQ(head->size, vec_data.size());
    EXPECT_EQ(head->first->data, (void *)vec_data.front());
    EXPECT_EQ(head->last->data, (void *)vec_data.back());

    struct slinked_list_node *node = head->first;
    uint64_t i = 0;
    for (; i < vec_data.size() && node; i++) {
        EXPECT_EQ(node->data, (void *)vec_data[i]) << (long)node->data << " != " << vec_data[i];
        node = node->next;
    }
    EXPECT_EQ(i, vec_data.size());
    EXPECT_EQ(node, nullptr);
}

TEST(slinked_list, insert_node_before)
{
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    struct slinked_list_node *node;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    node = slist_node_create((void *)1111);
    ASSERT_NE(node, nullptr);
    slist_insert_node_before(head, head->first, node);
    ASSERT_TRUE(slist_eq_data(head, {1111, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

    node = slist_node_create((void *)2222);
    ASSERT_NE(node, nullptr);
    slist_insert_node_before(head, head->last, node);
    ASSERT_TRUE(slist_eq_data(head, {1111, 0, 1, 2, 3, 4, 5, 6, 7, 8, 2222, 9}));

    node = slist_node_create((void *)3333);
    ASSERT_NE(node, nullptr);
    slist_insert_node_before(head, head->first->next->next, node);
    ASSERT_TRUE(slist_eq_data(head, {1111, 0, 3333, 1, 2, 3, 4, 5, 6, 7, 8, 2222, 9}));
}

TEST(slinked_list, insert_node_after)
{
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    struct slinked_list_node *node;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    node = slist_node_create((void *)1111);
    ASSERT_NE(node, nullptr);
    slist_insert_node_after(head, head->first, node);
    ASSERT_TRUE(slist_eq_data(head, {0, 1111, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

    node = slist_node_create((void *)2222);
    ASSERT_NE(node, nullptr);
    slist_insert_node_after(head, head->last, node);
    ASSERT_TRUE(slist_eq_data(head, {0, 1111, 1, 2, 3, 4, 5, 6, 7, 8, 9, 2222}));

    node = slist_node_create((void *)3333);
    ASSERT_NE(node, nullptr);
    slist_insert_node_after(head, head->first->next->next, node);
    ASSERT_TRUE(slist_eq_data(head, {0, 1111, 1, 3333, 2, 3, 4, 5, 6, 7, 8, 9, 2222}));
}

TEST(slinked_list, find_node)
{
    auto fn_eq = [](void *a, void *b) -> int { return a == b ? 0 : 1; };

    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    struct slinked_list_node *node;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    node = slist_find_node(head, (void *)0, fn_eq);
    EXPECT_EQ(node, head->first);
    EXPECT_EQ(node->data, (void *)0);

    node = slist_find_node(head, (void *)9, fn_eq);
    EXPECT_EQ(node, head->last);
    EXPECT_EQ(node->data, (void *)9);

    node = slist_find_node(head, (void *)5, fn_eq);
    EXPECT_EQ(node->data, (void *)5);
}

TEST(slinked_list, find_node_prev)
{
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    struct slinked_list_node *node;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    node = slist_find_node_prev(head, head->first);
    EXPECT_EQ(node, nullptr);

    node = slist_find_node_prev(head, head->last);
    EXPECT_EQ((long)node->data, (long)8);

    node = slist_find_node_prev(head, head->first->next->next);
    EXPECT_EQ((long)node->data, (long)1);
}

TEST(slinked_list, move_node_to_start)
{
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    int ret;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    ret = slist_move_node_to_start(head, head->first);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(slist_eq_data(head, vec_data));

    ret = slist_move_node_to_start(head, head->last);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(slist_eq_data(head, {9, 0, 1, 2, 3, 4, 5, 6, 7, 8}));

    ret = slist_move_node_to_start(head, head->first->next->next);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(slist_eq_data(head, {1, 9, 0, 2, 3, 4, 5, 6, 7, 8}));
}

TEST(slinked_list, move_node_to_end)
{
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    int ret;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    ret = slist_move_node_to_end(head, head->last);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(slist_eq_data(head, vec_data));

    ret = slist_move_node_to_end(head, head->first);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(slist_eq_data(head, {1, 2, 3, 4, 5, 6, 7, 8, 9, 0}));

    ret = slist_move_node_to_end(head, head->first->next->next);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(slist_eq_data(head, {1, 2, 4, 5, 6, 7, 8, 9, 0, 3}));
}

TEST(slinked_list, remove_node)
{
    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);
    struct slinked_list_node *node;

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    node = slist_remove_node(head, head->first);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data, (void *)0);
    EXPECT_TRUE(slist_eq_data(head, {1, 2, 3, 4, 5, 6, 7, 8, 9}));
    slist_node_free(node);

    node = slist_remove_node(head, head->last);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data, (void *)9);
    EXPECT_TRUE(slist_eq_data(head, {1, 2, 3, 4, 5, 6, 7, 8}));
    slist_node_free(node);

    node = slist_remove_node(head, head->first->next->next);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data, (void *)3);
    EXPECT_TRUE(slist_eq_data(head, {1, 2, 4, 5, 6, 7, 8}));
    slist_node_free(node);
}

TEST(slinked_list, forward_traverse)
{

    std::vector<long> vec_data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct slinked_list_head *head = create_by_list(vec_data);

    auto fn_traverse = [](struct slinked_list_node *node, void *ctx) -> int {
        static int i = 0;
        std::vector<long> *vec_data = (std::vector<long> *)ctx;
        EXPECT_EQ(node->data, (void *)vec_data->at(i));
        i++;
        return 0;
    };

    ASSERT_NE(head, nullptr);
    ASSERT_EQ(head->size, vec_data.size());
    ASSERT_TRUE(slist_eq_data(head, vec_data));

    slist_forward_traverse(head, fn_traverse, &vec_data);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
