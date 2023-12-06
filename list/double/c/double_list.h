#ifndef C_DOUBLE_LIST
#define C_DOUBLE_LIST

struct dlist_node {
    struct dlist_node *prev;
    struct dlist_node *next;
    void *data;
};

struct dlist_head {
    int size;

    struct dlist_node *first;
    struct dlist_node *last;
};

/**
 * @brief Create a double-linked list
 *
 * @return struct dlist_head*
 */
struct dlist_head *create_dlist();

/**
 * @brief Free the double-linked list
 *
 * @param head Head of double list
 * @param cb  callback function
 * @param ctx
 */
void free_dlist(struct dlist_head *head, void (*cb)(struct dlist_node *node, void *ctx), void *ctx);

/**
 * @brief Create a node for a double-linked list
 *
 * @param data node->data
 * @return struct dlist_node*
 */
struct dlist_node *create_dlist_node(void *data);

/**
 * @brief Free the node of double-linked list
 *
 * @param node
 */
void free_dlist_node(struct dlist_node *node);

/**
 * @brief Insert the node at the start of the double-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int insert_node_start(struct dlist_head *head, struct dlist_node *node);

/**
 * @brief Insert the node at the end of the double-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int insert_node_end(struct dlist_head *head, struct dlist_node *node);

/**
 * @brief Insert the node before the pos_node.
 *
 * @param head
 * @param pos_node
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int insert_node_before(struct dlist_head *head, struct dlist_node *pos_node,
                       struct dlist_node *node);

/**
 * @brief Insert the node after the pos_node.
 *
 * @param head
 * @param pos_node
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int insert_node_after(struct dlist_head *head, struct dlist_node *pos_node,
                      struct dlist_node *node);

/**
 * @brief Move the node to the start of the double-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int move_node_start(struct dlist_head *head, struct dlist_node *node);

/**
 * @brief Move the node to the end of the double-linked list.
 *
 * @param head
 * @param node
 * @return int  On success, 0 is retuned. On error, -1 is returned.
 */
int move_node_end(struct dlist_head *head, struct dlist_node *node);

/**
 * @brief Remove the node from the double-linked list.
 *
 * @param head
 * @param node
 * @return struct dlist_node*
 */
struct dlist_node *remove_node(struct dlist_head *head, struct dlist_node *node);

/**
 * @brief Forward traverse each node of the doubly linked list.
 *
 * @param head
 * @param cb
 * @param ctx
 */
void walk_dlist_forward(struct dlist_head *head, int (*cb)(struct dlist_node *node, void *ctx),
                        void *ctx);

/**
 * @brief Reverse traverse each node of the doubly linked list
 *
 * @param head
 * @param cb
 * @param ctx
 */
void walk_dlist_reverse(struct dlist_head *head, int (*cb)(struct dlist_node *node, void *ctx),
                        void *ctx);

#endif /* C_DOUBLE_LIST */
