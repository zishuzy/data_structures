#ifndef C_DLINKED_LIST
#define C_DLINKED_LIST

struct slinked_list_node {
    struct slinked_list_node *next;
    void *data;
};

struct slinked_list_head {
    int size;

    struct slinked_list_node *first;
    struct slinked_list_node *last;
};

/**
 * @brief Create a singly-linked list
 *
 * @return struct slinked_list_head*
 */
struct slinked_list_head *slist_create();

/**
 * @brief Free the singly-linked list
 *
 * @param head Head of singly list
 * @param cb  callback function
 * @param ctx
 */
void slist_free(struct slinked_list_head *head,
                void (*cb)(struct slinked_list_node *node, void *ctx), void *ctx);

/**
 * @brief Create a node for a singly-linked list
 *
 * @param data node->data
 * @return struct slinked_list_node*
 */
struct slinked_list_node *slist_node_create(void *data);

/**
 * @brief Free the node of singly-linked list
 *
 * @param node
 */
void slist_node_free(struct slinked_list_node *node);

/**
 * @brief Insert the node at the start of the singly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int slist_insert_node_start(struct slinked_list_head *head, struct slinked_list_node *node);

/**
 * @brief Insert the node at the end of the singly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int slist_insert_node_end(struct slinked_list_head *head, struct slinked_list_node *node);

/**
 * @brief Insert the node before the pos_node.
 *
 * @param head
 * @param pos_node
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int slist_insert_node_before(struct slinked_list_head *head, struct slinked_list_node *pos_node,
                             struct slinked_list_node *node);

/**
 * @brief Insert the node after the pos_node.
 *
 * @param head
 * @param pos_node
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int slist_insert_node_after(struct slinked_list_head *head, struct slinked_list_node *pos_node,
                            struct slinked_list_node *node);

/**
 * @brief Find the previous node of the node in the singly-linked list.
 *
 * @param head
 * @param node
 * @return struct slinked_list_node*
 */
struct slinked_list_node *slist_find_node_prev(struct slinked_list_head *head,
                                               struct slinked_list_node *node);

/**
 * @brief Move the node to the start of the singly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int slist_move_node_start(struct slinked_list_head *head, struct slinked_list_node *node);

/**
 * @brief Move the node to the end of the singly-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int slist_move_node_end(struct slinked_list_head *head, struct slinked_list_node *node);

/**
 * @brief Remove the node from the singly-linked list.
 *
 * @param head
 * @param node
 * @return struct slinked_list_node*
 */
struct slinked_list_node *slist_remove_node(struct slinked_list_head *head,
                                            struct slinked_list_node *node);

/**
 * @brief Forward traverse each node of the singly linked list.
 *
 * @param head
 * @param cb
 * @param ctx
 */
void slist_forward_traverse(struct slinked_list_head *head,
                            int (*cb)(struct slinked_list_node *node, void *ctx), void *ctx);

#endif /* C_DLINKED_LIST */
