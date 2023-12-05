#include "list_double.h"

#include "common/log/log.h"

#include <stdlib.h>

static inline struct dlist_node *create_node(void *data)
{
    struct dlist_node *node = malloc(sizeof(struct dlist_node));
    if (!node) {
        return node;
    }

    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

struct dlist_head *create_dlist()
{
    struct dlist_head *head = malloc(sizeof(struct dlist_head));
    if (!head) {
        return head;
    }

    head->size = 0;
    head->first = NULL;
    head->last = NULL;

    return head;
}

struct dlist_node *add_node_last(struct dlist_head *head, void *data)
{
    if (!head) {
        return NULL;
    }
    struct dlist_node *node = create_node(data);
    if (!node) {
        return node;
    }

    if (head->last) {
        node->prev = head->last;
        head->last->next = node;
        head->last = node;
    } else {
        head->last = node;
        head->first = node;
    }

    return node;
}

struct dlist_node *add_node_first(struct dlist_head *head, void *data)
{
    if (!head) {
        return NULL;
    }
    struct dlist_node *node = create_node(data);
    if (!node) {
        return node;
    }

    if (head->first) {
        node->next = head->first;
        head->first->prev = node;
        head->first = node;
    } else {
        head->last = node;
        head->first = node;
    }

    return node;
}

struct dlist_node *add_node_prev(struct dlist_head *head, struct dlist_node *next, void *data)
{
    if (!head || !next) {
        return NULL;
    }

    struct dlist_node *node = create_node(data);
    if (!node) {
        return node;
    }

    if (next->prev) {
        next->prev->next = node;
        node->prev = next->prev;
        next->prev = node;
        node->next = next;
    } else {
        if (head->first != next) {
            LOG_ERROR("BUG: head->first != next");
            free(node);
            return NULL;
        } else {
            head->first = node;
            next->prev = node;
            node->next = next;
        }
    }
    return node;
}

struct dlist_node *add_node_next(struct dlist_head *head, struct dlist_node *prev, void *data)
{
    if (!head || !prev) {
        return NULL;
    }

    struct dlist_node *node = create_node(data);
    if (!node) {
        return node;
    }

    if (prev->next) {
        prev->next->prev = node;
        node->next = prev->next;
        prev->next = node;
        node->prev = prev;
    } else {
        if (head->last != prev) {
            LOG_ERROR("BUG: head->last != prev");
            free(node);
            return NULL;
        } else {
            head->last = node;
            prev->next = node;
            node->prev = prev;
        }
    }
    return node;
}

void delete_node(struct dlist_head *head, struct dlist_node *node)
{
    if (!head || !node) {
        return;
    }

    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }

    if (head->first == node) {
        head->first = node->next;
    }
    if (head->last == node) {
        head->last = node->prev;
    }
}

void walk_dlist_forward(struct dlist_head *head, int (*cb)(struct dlist_node *node, void *ctx),
                        void *ctx)
{
    if (!head) {
        return;
    }
    struct dlist_node *node = head->first;

    while (node) {
        if (cb(node, ctx) == 1) {
            break;
        }
        node = node->next;
    }
}

void walk_dlist_reverse(struct dlist_head *head, int (*cb)(struct dlist_node *node, void *ctx),
                        void *ctx)
{
    if (!head) {
        return;
    }
    struct dlist_node *node = head->last;

    while (node) {
        if (cb(node, ctx) == 1) {
            break;
        }
        node = node->prev;
    }
}
