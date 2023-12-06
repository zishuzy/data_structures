#include "double_list.h"

#include "common/log/log.h"

#include <stdlib.h>

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

void free_dlist(struct dlist_head *head, void (*cb)(struct dlist_node *node, void *ctx), void *ctx)
{
    if (!head) {
        return;
    }

    struct dlist_node *node, *node_head;
    for (node = head->first; node != NULL; node = node_head) {
        if (cb) {
            cb(node, ctx);
        }
        node_head = node->next;
        free(node);
    }
    free(head);
}

struct dlist_node *create_dlist_node(void *data)
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

void free_dlist_node(struct dlist_node *node)
{
    if (!node) {
        return;
    }
    free(node);
}

int insert_node_start(struct dlist_head *head, struct dlist_node *node)
{
    if (!head || !node) {
        return -1;
    }

    node->prev = NULL;
    if (head->first) {
        node->next = head->first;
        head->first->prev = node;
        head->first = node;
    } else {
        node->next = NULL;
        head->last = node;
        head->first = node;
    }

    head->size++;

    return 0;
}

int insert_node_end(struct dlist_head *head, struct dlist_node *node)
{
    if (!head || !node) {
        return -1;
    }

    node->next = NULL;
    if (head->last) {
        node->prev = head->last;
        head->last->next = node;
        head->last = node;
    } else {
        node->prev = NULL;
        head->last = node;
        head->first = node;
    }
    head->size++;

    return 0;
}

int insert_node_before(struct dlist_head *head, struct dlist_node *pos_node,
                       struct dlist_node *node)
{
    if (!head || !pos_node || !node) {
        return -1;
    }

    if (pos_node->prev) {
        pos_node->prev->next = node;
        node->prev = pos_node->prev;
        pos_node->prev = node;
        node->next = pos_node;
    } else {
        if (head->first == pos_node) {
            head->first = node;
            pos_node->prev = node;
            node->next = pos_node;
            node->prev = NULL;
        } else {
            LOG_ERROR("BUG: head->first != pos_node");
            return -1;
        }
    }
    head->size++;
    return 0;
}

int insert_node_after(struct dlist_head *head, struct dlist_node *pos_node, struct dlist_node *node)
{
    if (!head || !pos_node || !node) {
        return -1;
    }

    if (pos_node->next) {
        pos_node->next->prev = node;
        node->next = pos_node->next;
        pos_node->next = node;
        node->prev = pos_node;
    } else {
        if (head->last == pos_node) {
            head->last = node;
            pos_node->next = node;
            node->prev = pos_node;
            node->next = NULL;
        } else {
            LOG_ERROR("BUG: head->last != prev");
            return -1;
        }
    }
    head->size++;
    return 0;
}

int move_node_start(struct dlist_head *head, struct dlist_node *node)
{
    if (!head || !head->first || !node) {
        return -1;
    }

    if (head->first == node) {
        return 0;
    }
    if (head->last == node) {
        head->last = node->prev;
    }

    if (node->prev) {
        node->prev->next = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    }

    node->next = head->first;
    head->first->prev = node;
    head->first = node;
    node->prev = NULL;

    return 0;
}

int move_node_end(struct dlist_head *head, struct dlist_node *node)
{
    if (!head || !head->last || !node) {
        return -1;
    }

    if (head->last == node) {
        return 0;
    }
    if (head->first == node) {
        head->first = node->next;
    }

    if (node->prev) {
        node->prev->next = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    }

    node->prev = head->last;
    head->last->next = node;
    head->last = node;
    node->next = NULL;

    return 0;
}

struct dlist_node *remove_node(struct dlist_head *head, struct dlist_node *node)
{
    if (!head || !node) {
        return NULL;
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
    head->size--;

    return node;
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
