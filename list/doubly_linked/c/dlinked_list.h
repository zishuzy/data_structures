/**
 * @file dlinked_list.h
 * @author zishu (zishuzy@gmail.com)
 * @brief Doubly-linked list using C.
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef C_DLINKED_LIST
#define C_DLINKED_LIST

struct dlinked_list_node {
    struct dlinked_list_node *prev;
    struct dlinked_list_node *next;
    void *data;
};

struct dlinked_list_head {
    int size;

    struct dlinked_list_node *first;
    struct dlinked_list_node *last;
};

/**
 * @brief Create a doubly-linked list
 *
 * @return struct dlinked_list_head*
 */
struct dlinked_list_head *dlist_create();

/**
 * @brief Free the doubly-linked list
 *
 * @param head Head of doubly list
 * @param cb  callback function
 * @param ctx
 */
void dlist_free(struct dlinked_list_head *head,
                void (*cb)(struct dlinked_list_node *node, void *ctx), void *ctx);

/**
 * @brief Create a node for a doubly-linked list
 *
 * @param data node->data
 * @return struct dlinked_list_node*
 */
struct dlinked_list_node *dlist_node_create(void *data);

/**
 * @brief Free the node of doubly-linked list
 *
 * @param node
 */
void dlist_node_free(struct dlinked_list_node *node);

/**
 * @brief Insert the node at the start of the doubly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int dlist_insert_node_start(struct dlinked_list_head *head, struct dlinked_list_node *node);

/**
 * @brief Insert the node at the end of the doubly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int dlist_insert_node_end(struct dlinked_list_head *head, struct dlinked_list_node *node);

/**
 * @brief Insert the node before the pos_node.
 *
 * @param head
 * @param pos_node
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int dlist_insert_node_before(struct dlinked_list_head *head, struct dlinked_list_node *pos_node,
                             struct dlinked_list_node *node);

/**
 * @brief Insert the node after the pos_node.
 *
 * @param head
 * @param pos_node
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int dlist_insert_node_after(struct dlinked_list_head *head, struct dlinked_list_node *pos_node,
                            struct dlinked_list_node *node);

/**
 * @brief Move the node to the start of the doubly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int dlist_move_node_start(struct dlinked_list_head *head, struct dlinked_list_node *node);

/**
 * @brief Move the node to the end of the doubly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int dlist_move_node_end(struct dlinked_list_head *head, struct dlinked_list_node *node);

/**
 * @brief Remove the node from the doubly-linked list.
 *
 * @param head
 * @param node
 * @return struct dlinked_list_node*
 */
struct dlinked_list_node *dlist_remove_node(struct dlinked_list_head *head,
                                            struct dlinked_list_node *node);

/**
 * @brief Forward traverse each node of the doubly linked list.
 *
 * @param head
 * @param cb
 * @param ctx
 */
void dlist_forward_traverse(struct dlinked_list_head *head,
                            int (*cb)(struct dlinked_list_node *node, void *ctx), void *ctx);

/**
 * @brief Reverse traverse each node of the doubly linked list
 *
 * @param head
 * @param cb
 * @param ctx
 */
void dlist_reverse_traverse(struct dlinked_list_head *head,
                            int (*cb)(struct dlinked_list_node *node, void *ctx), void *ctx);

#endif /* C_DLINKED_LIST */
