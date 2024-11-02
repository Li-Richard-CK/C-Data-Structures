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

struct dlist dlist_create(...);

#endif
