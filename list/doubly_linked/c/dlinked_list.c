#include "dlinked_list.h"

#include "common/log/log.h"

#include <stdlib.h>

struct dlinked_list_head *dlist_create()
{
    struct dlinked_list_head *head = malloc(sizeof(struct dlinked_list_head));
    if (!head) {
        return head;
    }

    head->size = 0;
    head->first = NULL;
    head->last = NULL;

    return head;
}

void dlist_free(struct dlinked_list_head *head,
                void (*cb)(struct dlinked_list_node *node, void *ctx), void *ctx)
{
    if (!head) {
        return;
    }

    struct dlinked_list_node *node, *node_head;
    for (node = head->first; node != NULL; node = node_head) {
        if (cb) {
            cb(node, ctx);
        }
        node_head = node->next;
        free(node);
    }
    free(head);
}

struct dlinked_list_node *dlist_node_create(void *data)
{
    struct dlinked_list_node *node = malloc(sizeof(struct dlinked_list_node));
    if (!node) {
        return node;
    }

    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void dlist_node_free(struct dlinked_list_node *node)
{
    if (!node) {
        return;
    }
    free(node);
}

int dlist_insert_node_start(struct dlinked_list_head *head, struct dlinked_list_node *node)
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

int dlist_insert_node_end(struct dlinked_list_head *head, struct dlinked_list_node *node)
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

int dlist_insert_node_before(struct dlinked_list_head *head, struct dlinked_list_node *pos_node,
                             struct dlinked_list_node *node)
{
    if (!head || !pos_node || !node) {
        return -1;
    }

    if (head->first == pos_node) {
        head->first = node;
        node->prev = NULL;
        node->next = pos_node;
        pos_node->prev = node;
    } else {
        // If the pos_node isnot the first node, then pos_node->prev isnot NULL.
        pos_node->prev->next = node;
        node->prev = pos_node->prev;
        pos_node->prev = node;
        node->next = pos_node;
    }
    head->size++;
    return 0;
}

int dlist_insert_node_after(struct dlinked_list_head *head, struct dlinked_list_node *pos_node,
                            struct dlinked_list_node *node)
{
    if (!head || !pos_node || !node) {
        return -1;
    }

    if (head->last == pos_node) {
        head->last = node;
        node->next = NULL;
        node->prev = pos_node;
        pos_node->next = node;
    } else {
        // If the pos_node isnot the last node, then pos_node->next isnot NULL.
        pos_node->next->prev = node;
        node->next = pos_node->next;
        pos_node->next = node;
        node->prev = pos_node;
    }
    head->size++;
    return 0;
}

int dlist_move_node_start(struct dlinked_list_head *head, struct dlinked_list_node *node)
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

int dlist_move_node_end(struct dlinked_list_head *head, struct dlinked_list_node *node)
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

struct dlinked_list_node *dlist_remove_node(struct dlinked_list_head *head,
                                            struct dlinked_list_node *node)
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

void dlist_forward_traverse(struct dlinked_list_head *head,
                            int (*cb)(struct dlinked_list_node *node, void *ctx), void *ctx)
{
    if (!head) {
        return;
    }
    struct dlinked_list_node *node = head->first;

    while (node) {
        if (cb(node, ctx) == 1) {
            break;
        }
        node = node->next;
    }
}

void dlist_reverse_traverse(struct dlinked_list_head *head,
                            int (*cb)(struct dlinked_list_node *node, void *ctx), void *ctx)
{
    if (!head) {
        return;
    }
    struct dlinked_list_node *node = head->last;

    while (node) {
        if (cb(node, ctx) == 1) {
            break;
        }
        node = node->prev;
    }
}
