#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "common/log/log.h"
#include "rb_tree.h"
// #include "rb_tree_c.h"

#ifndef ARRAY_SIZE
    #define ARRAY_SIZE(a) ((sizeof(a)) / (sizeof(a[0])))
#endif

void print_key_val(void *key, void *val)
{
    printf("key: %ld, val: %ld\n", (long)key, (long)val);
}

void test1(void)
{
    // LOG_INFO("start: [%s]", __FUNCTION__);
    long key[] = {10, 40, 30, 60, 90, 70, 20, 50, 80, 33, 56, 12, 53, 31, 55};
    long val[] = {10, 40, 30, 60, 90, 70, 20, 50, 80, 33, 56, 12, 53, 31, 55};
    int i = 0;
    int key_len = ARRAY_SIZE(key);
    struct rbtree_root *rb_root = NULL;
    struct rbtree_arg arg = {
        .is_thread_safe = false,
    };

    rb_root = rbtree_init(arg);

    for (i = 0; i < key_len; ++i) {
        rbtree_insert(rb_root, (void *)key[i], (void *)val[i], false, false);
    }
    rbtree_inorder(rb_root, print_key_val);
    print_rbtree(rb_root);
    rbtree_destroy(rb_root);

    // for (i = 0; i < key_len; i++) {
    //     rbtree_delete(rb_root, (void *)key[i]);
    //     LOG_INFO("delete key[%ld]", key[i]);
    //     print_rbtree(rb_root);
    // }
}

void test2_print_key_val(void *key, void *val)
{
    printf("key: %s, val: %s\n", (char *)key, (char *)val);
}

int test2_cmp_key(void *key0, void *key1)
{
    return strcmp(key0, key1);
}

void *test2_copy_key(void *key)
{
    void *k = calloc(1, 4096);
    int key_len = strlen(key);

    memcpy(k, key, key_len);

    return k;
}

void test2_free_key(void *key)
{
    free(key);
}

void *test2_copy_value(void *value)
{
    void *v = calloc(1, 4096);
    int value_len = strlen(value);

    memcpy(v, value, value_len);

    return v;
}

void test2_free_value(void *value)
{
    free(value);
}

void test2(void)
{
    // LOG_INFO("start: [%s]", __FUNCTION__);
    char *key[] = {"fff", "1111", "3", "f432", "11fds1", "fdsfafdsa", "43grfdg", "igfdg"};
    char *val[] = {"fff", "1111", "3", "f432", "11fds1", "fdsfafdsa", "43grfdg", "igfdg"};
    int i = 0;
    int key_len = ARRAY_SIZE(key);
    struct rbtree_root *rb_root = NULL;
    struct rbtree_arg arg = {
        .is_thread_safe = false,
        .cmp_key = test2_cmp_key,
        .copy_key = test2_copy_key,
        .free_key = test2_free_key,
        .copy_value = test2_copy_value,
        .free_value = test2_free_value,
    };

    rb_root = rbtree_init(arg);

    for (i = 0; i < key_len; ++i) {
        rbtree_insert(rb_root, (void *)key[i], (void *)val[i], true, true);
    }
    rbtree_inorder(rb_root, test2_print_key_val);
    print_rbtree(rb_root);
    rbtree_destroy(rb_root);

    // for (i = 0; i < key_len; i++) {
    //     rbtree_delete(rb_root, (void *)key[i]);
    //     LOG_INFO("delete key[%ld]", key[i]);
    //     print_rbtree(rb_root);
    // }
}

int main(int argc, char *argv[])
{
    (void)argc;
    LOG_INFO("start: [%s]", argv[0]);
    // test1();
    test2();
    return 0;
}