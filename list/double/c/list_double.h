#ifndef C_LIST_DOUBLE
#define C_LIST_DOUBLE

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

struct dlist_head *create_dlist();

struct dlist_node *add_node_last(struct dlist_head *head, void *data);

struct dlist_node *add_node_first(struct dlist_head *head, void *data);

struct dlist_node *add_node_prev(struct dlist_head *head, struct dlist_node *next, void *data);

struct dlist_node *add_node_next(struct dlist_head *head, struct dlist_node *prev, void *data);

void delete_node(struct dlist_head *head, struct dlist_node *node);

void walk_dlist_forward(struct dlist_head *head, int (*cb)(struct dlist_node *node, void *ctx),
                        void *ctx);

void walk_dlist_reverse(struct dlist_head *head, int (*cb)(struct dlist_node *node, void *ctx),
                        void *ctx);

#endif /* C_LIST_DOUBLE */
