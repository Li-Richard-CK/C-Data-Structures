#include "dlist.h"

#include <__stddef_size_t.h>
#include <__stddef_null.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <mm_malloc.h>

#define DEFAULT_ALLOC malloc
#define DEFAULT_FREE free

struct dlist_node
{
    void *data;

    struct dlist_node *next, *prev;
    size_t index;
};

static struct dlist_node *_create_node(void *(*alloc_f)(size_t), const void *data)
{
    struct dlist_node *new_node = alloc_f(sizeof(struct dlist_node));
    if (!new_node)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    new_node->data = alloc_f(sizeof(void *));
    if (!new_node->data)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    memcpy(new_node->data, data, sizeof(void *));

    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->index = 0;

    return new_node;
}

/*
dlist_create(void)

creates a new doubly linked list

return:
newly created doubly linked list

time complexity: O(1)
space complexity: O(1)
*/
struct dlist dlist_create(void)
{
    struct dlist new_list;

    new_list.len = 0;
    new_list.head = NULL;
    new_list.tail = NULL;

    new_list.cmp_f = NULL;

    new_list.mm.alloc_f = DEFAULT_ALLOC;
    new_list.mm.free_f = DEFAULT_FREE;

    return new_list;
}

/*
dlist_insert(struct dlist *list, const void *data, size_t index)

inserts 'data' into 'index' of the doubly linked list
this function will assume that if the index is greater than the length of the linked list
to be the length of the list resulting in inserting at the end of the list
it will also automatically decide the way(from head / tail) to traverse the linked list for
optimization
it will also automatically update the node's index after insertion

time complexity:
worse-case: O(n)
average: O(n / 2)
best-case: O(1)
space complexity: O(1)
*/
void dlist_insert(struct dlist *list, const void *data, size_t index)
{
    if (index > list->len)
        index = list->len;

    struct dlist_node *new_node = _create_node(list->mm.alloc_f, data);
    if (!new_node)
    {
        fprintf(stderr, "Failed to create new dlist node.\n");
        return;
    }
    new_node->index = index;

    if (list->head == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
        goto ret;
    }

    if (index == list->len)
    {
        list->tail->next = new_node;
        list->tail = new_node;
        goto ret;
    }

    struct dlist_node *cur = list->head;
    /* traverse from tail */
    if (index >= list->len / 2)
    {
        cur = list->tail;
        while (cur->index != index && cur->prev != NULL)
            cur = cur->prev;
    }
    /* traverse from head */
    else
    {
        cur = list->head;
        while (cur->index != index && cur->next != NULL)
            cur = cur->next;
    }

    new_node->next = cur->next;
    new_node->prev = cur;
    cur->next = new_node;

    while (cur->next != NULL)
    {
        ++cur->next->index;
        cur = cur->next;
    }

    if (new_node->next == NULL)
        list->tail = new_node;

ret:
    ++list->len;
}

/*
dlist_free(struct dlist *list)

free memory allocated by the doubly linked list
traverse the list while freeing the memory

time complexity: O(n)
space complexity: O(1)
*/
void dlist_free(struct dlist *list)
{
    struct dlist_node *cur, *next;
    cur = list->head;
    next = NULL;

    while (cur != NULL)
    {
        next = cur->next;

        list->mm.free_f(cur->data);
        list->mm.free_f(cur);

        cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
}
