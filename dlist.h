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

/*
dlist_is_empty(struct dlist *list)

returns if 'list' is empty

time complexity: O(1)
space complexity: O(1)
*/
int dlist_is_empty(struct dlist *list);

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
void dlist_insert(struct dlist *list, const void *data, size_t index);

/*
dlist_append(list, data)

append 'data' into the tail of the doubly linked list

time complexity: O(1)
space complexity: O(1)
*/
#define dlist_append(list, data) dlist_insert((list), (data), ((list)->len))

/*
dlist_at(struct dlist *list, size_t index)

retrieves data from 'index' of the doubly linked list
this function will assume that if the index is greater than the length of the linked list
to be the length of the list resulting in inserting at the end of the list
it will also automatically decide the way(from head / tail) to traverse the linked list for
optimization

time complexity:
worse-case: O(n)
average: O(n / 2)
best-case: O(1)
space complexity: O(1)
*/
void *dlist_at(struct dlist *list, size_t index);

/*
dlist_at_node(struct dlist *list, size_t index)

retrieves node of 'index'
this function will assume that if the index is greater than the length of the linked list
to be the length of the list resulting in inserting at the end of the list
it will also automatically decide the way(from head / tail) to traverse the linked list for
optimization

time complexity:
worse-case: O(n)
average: O(n / 2)
best-case: O(1)
space complexity: O(1)
*/
struct dlist_node *dlist_at_node(struct dlist *list, size_t index);

/*
dlist_at_cast(list, index, _t)

retrieves data from 'index' of the doubly linked list and cast it to '_t' type
this function will assume that if the index is greater than the length of the linked list
to be the length of the list resulting in inserting at the end of the list
it will also automatically decide the way(from head / tail) to traverse the linked list for
optimization

time complexity:
worse-case: O(n)
average: O(n / 2)
best-case: O(1)
space complexity: O(1)
*/
#define dlist_at_cast(list, index, _t) *((_t *)dlist_at((list), (index)))

/*
dlist_erase(struct dlist *list, size_t start, ...)

erase the nodes of index 'start' and end 1 index before 'end'
it will delete 'end' - 'start' amount of elements
if 'start' and 'end' is the same then the function will automatically call '++end'
it will also automatically decide the way(from head / tail) to traverse the linked list to
find the starting node for optimization

time complexity:
worse-case: O(n + M)
average: O(n / 2 + M)
best-case: O(1)
space complexity: O(1)
*/
void dlist_erase(struct dlist *list, size_t start, size_t end);

/*
dlist_swap(struct dlist *list, size_t index0, size_t index1)

swap the node of 'index0' and 'index1'

time complexity:
worse-case: O(n)
average: O(n)
best-case: O(1)
space complexity: O(1)
*/
void dlist_swap(struct dlist *list, size_t index0, size_t index1);

/*
dlist_free(struct dlist *list)

free memory allocated by the doubly linked list
traverse the list while freeing the memory

time complexity: O(n)
space complexity: O(1)
*/
void dlist_free(struct dlist *list);

#endif
