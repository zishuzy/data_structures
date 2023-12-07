/**
 * @file slinked_list.c
 * @author zishu (zishuzy@gmail.com)
 * @brief Singly-linked list using C.
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "slinked_list.h"

#include "common/log/log.h"

#include <stdlib.h>

struct slinked_list_head *slist_create()
{
    struct slinked_list_head *head = malloc(sizeof(struct slinked_list_head));
    if (!head) {
        return head;
    }

    head->size = 0;
    head->first = NULL;
    head->last = NULL;

    return head;
}

void slist_free(struct slinked_list_head *head,
                void (*cb)(struct slinked_list_node *node, void *ctx), void *ctx)
{
    if (!head) {
        return;
    }

    struct slinked_list_node *node, *node_head;
    for (node = head->first; node != NULL; node = node_head) {
        if (cb) {
            cb(node, ctx);
        }
        node_head = node->next;
        free(node);
    }
    free(head);
}

struct slinked_list_node *slist_node_create(void *data)
{
    struct slinked_list_node *node = malloc(sizeof(struct slinked_list_node));
    if (!node) {
        return node;
    }

    node->data = data;
    node->next = NULL;
    return node;
}

void slist_node_free(struct slinked_list_node *node)
{
    if (!node) {
        return;
    }
    free(node);
}

int slist_insert_node_start(struct slinked_list_head *head, struct slinked_list_node *node)
{
    if (!head || !node) {
        return -1;
    }

    if (head->first) {
        node->next = head->first;
        head->first = node;
    } else {
        node->next = NULL;
        head->last = node;
        head->first = node;
    }

    head->size++;

    return 0;
}

int slist_insert_node_end(struct slinked_list_head *head, struct slinked_list_node *node)
{
    if (!head || !node) {
        return -1;
    }

    node->next = NULL;
    if (head->last) {
        head->last->next = node;
        head->last = node;
    } else {
        head->last = node;
        head->first = node;
    }
    head->size++;

    return 0;
}

int slist_insert_node_before(struct slinked_list_head *head, struct slinked_list_node *pos_node,
                             struct slinked_list_node *node)
{
    struct slinked_list_node *prev;
    if (!head || !pos_node || !node) {
        return -1;
    }

    if (head->first == pos_node) {
        node->next = pos_node;
        head->first = node;
    } else {
        prev = slist_find_node_prev(head, pos_node);
        if (!prev) {
            LOG_ERROR("Can't find the node in the singly-linked list!")
            return -1;
        }
        node->next = prev->next;
        prev->next = node;
    }

    head->size++;
    return 0;
}

int slist_insert_node_after(struct slinked_list_head *head, struct slinked_list_node *pos_node,
                            struct slinked_list_node *node)
{
    if (!head || !pos_node || !node) {
        return -1;
    }

    if (head->last == pos_node) {
        pos_node->next = node;
        head->last = node;
    } else {
        node->next = pos_node->next;
        pos_node->next = node;
    }

    head->size++;
    return 0;
}

struct slinked_list_node *slist_find_node_prev(struct slinked_list_head *head,
                                               struct slinked_list_node *node)
{
    int found = 0;
    struct slinked_list_node *found_node = NULL;
    if (!head || !node) {
        return NULL;
    }

    for (found_node = head->first; found_node != NULL; found_node = found_node->next) {
        if (found_node->next == node) {
            found = 1;
            break;
        }
    }

    return found ? found_node : NULL;
}

int slist_move_node_start(struct slinked_list_head *head, struct slinked_list_node *node)
{
    struct slinked_list_node *prev;

    if (!head || !head->first || !node) {
        return -1;
    }

    if (head->first == node) {
        return 0;
    }

    prev = slist_find_node_prev(head, node);
    if (!prev) {
        LOG_ERROR("Can't find the node in the singly-linked list!")
        return -1;
    }

    if (head->last == node) {
        head->last = prev;
    }

    prev->next = node->next;

    node->next = head->first;
    head->first = node;

    return 0;
}

int slist_move_node_end(struct slinked_list_head *head, struct slinked_list_node *node)
{
    struct slinked_list_node *prev;

    if (!head || !head->last || !node) {
        return -1;
    }

    if (head->last == node) {
        return 0;
    }

    if (head->first == node) {
        head->first = node->next;
    } else {
        prev = slist_find_node_prev(head, node);
        if (!prev) {
            LOG_ERROR("Can't find the node in the singly-linked list!")
            return -1;
        }
        prev->next = node->next;
    }

    head->last->next = node;
    head->last = node;
    node->next = NULL;

    return 0;
}

struct slinked_list_node *slist_remove_node(struct slinked_list_head *head,
                                            struct slinked_list_node *node)
{
    struct slinked_list_node *prev;
    if (!head || !node) {
        return NULL;
    }

    if (head->first == node) {
        head->first = node->next;
    } else {
        prev = slist_find_node_prev(head, node);
        if (!prev) {
            LOG_ERROR("Can't find the node in the singly-linked list!")
            return NULL;
        }
        prev->next = node->next;
        if (head->last == node) {
            head->last = prev;
        }
    }
    head->size--;

    return node;
}

void slist_forward_traverse(struct slinked_list_head *head,
                            int (*cb)(struct slinked_list_node *node, void *ctx), void *ctx)
{
    if (!head) {
        return;
    }
    struct slinked_list_node *node = head->first;

    while (node) {
        if (cb(node, ctx) == 1) {
            break;
        }
        node = node->next;
    }
}
