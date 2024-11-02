#ifndef _DLIST_H
#define _DLIST_H

#include <__stddef_size_t.h>

typedef struct dlist_node dlist_node_t;

typedef struct dlist
{
    struct dlist_node *head, *tail;
    size_t len;

    int (*cmp_f)(const void *, const void *);
    struct
    {
        void *(*alloc_f)(size_t);
        void (*free_f)(void *);
    } mm;
} dlist_t;

/*
dlist_create(void)

creates a new doubly linked list

return:
newly created doubly linked list

time complexity: O(1)
space complexity: O(1)
*/
struct dlist dlist_create(void);

void dlist_insert(struct dlist *list, const void *data, size_t index);

void dlist_free(struct dlist *list);

#endif
