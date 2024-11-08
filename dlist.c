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

    return new_node;
}

static void _free_node(struct dlist_node *node, void (*free_f)(void *))
{
    if (!node)
    {
        fprintf(stderr, "'node' cannot be empty.\n");
        return;
    }
    if (!free_f)
    {
        fprintf(stderr, "'free_f' cannot be empty.\n");
        return;
    }

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;

    free_f(node->data);
    free_f(node);
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
dlist_is_empty(struct dlist *list)

returns if 'list' is empty

time complexity: O(1)
space complexity: O(1)
*/
int dlist_is_empty(struct dlist *list)
{
    return (list->head) ? 0 : 1;
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
    if (!list)
    {
        fprintf(stderr, "'list' cannot be empty.\n");
        return;
    }
    if (!list->mm.alloc_f)
    {
        fprintf(stderr, "'alloc_f' cannot be empty.\n");
        return;
    }

    struct dlist_node *new_node = _create_node(list->mm.alloc_f, data);
    if (!new_node)
    {
        fprintf(stderr, "Failed to create new dlist node.\n");
        return;
    }

    if (index > list->len)
        index = list->len;

    /* set the head of the list */
    if (!list->head)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    /* insert as head */
    else if (index == 0)
    {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    /* insert at tail */
    else if (index >= list->len)
    {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    /* anywhere in between */
    else
    {
        // dlist_node_t *cur = list->head;
        // while (cur && index > 0)
        // {
        //     cur = cur->next;
        //     index--;
        // }

        // new_node->prev = cur->prev;
        // new_node->next = cur;
        // cur->prev->next = new_node;
        // cur->prev = new_node;

        struct dlist_node *cur = NULL;
        size_t cur_index = 0;
        /* traverse from head */
        if (index <= list->len / 2)
        {
            cur = list->head;
            while (cur_index != index && cur->next != NULL)
            {
                cur = cur->next;
                ++cur_index;
            }
        }
        /* traverse from tail */
        else
        {
            cur_index = list->len - 1;
            cur = list->tail;
            while (cur_index != index && cur->prev != NULL)
            {
                cur = cur->prev;
                --cur_index;
            }
        }

        new_node->next = cur->next;
        new_node->prev = cur;
        cur->next = new_node;

        if (new_node->next)
            new_node->next->prev = new_node;

        /* update index of the nodes */
        while (cur->next != NULL)
            cur = cur->next;
    }

    ++list->len;
}

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
void *dlist_at(struct dlist *list, size_t index)
{
    return dlist_at_node(list, index)->data;
}

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
struct dlist_node *dlist_at_node(struct dlist *list, size_t index)
{
    if (!list)
    {
        fprintf(stderr, "'list' cannot be empty.\n");
        return NULL;
    }
    if (dlist_is_empty(list))
    {
        fprintf(stderr, "'list' is empty.\n");
        return NULL;
    }

    if (index >= list->len)
        index = list->len - 1;

    // dlist_node_t *cur = list->head;
    // while (cur && index > 0)
    // {
    //     cur = cur->next;
    //     index--;
    // }

    struct dlist_node *cur = NULL;
    size_t cur_index = 0;
    /* traverse from head */
    if (index <= list->len / 2)
    {
        cur = list->head;
        while (cur_index != index && cur->next != NULL)
        {
            cur = cur->next;
            ++cur_index;
        }
    }
    /* traverse from tail */
    else
    {
        cur_index = list->len - 1;
        cur = list->tail;
        while (cur_index != index && cur->prev != NULL)
        {
            cur = cur->prev;
            --cur_index;
        }
    }

    return cur;
}

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
void dlist_erase(struct dlist *list, size_t start, size_t end)
{
    if (!list)
    {
        fprintf(stderr, "'list' cannot be empty.\n");
        return;
    }
    if (dlist_is_empty(list))
    {
        fprintf(stderr, "'list' is empty.\n");
        return;
    }

    if (start > end)
    {
        fprintf(stderr, "'start' cannot be greater than 'end'.\n");
        return;
    }

    if (start == end)
        ++end;

    /* find the starting node */
    struct dlist_node *cur = NULL;
    size_t cur_index = 0;
    /* traverse from head */
    if (start <= list->len / 2)
    {
        cur = list->head;
        while (cur_index != start && cur->next != NULL)
        {
            cur = cur->next;
            ++cur_index;
        }
    }
    /* traverse from tail */
    else
    {
        cur_index = list->len - 1;
        cur = list->tail;
        while (cur_index != start && cur->prev != NULL)
        {
            cur = cur->prev;
            --cur_index;
        }
    }

    while (start < end)
    {
        struct dlist_node *next = cur->next;
        /* update head */
        if (cur_index == 0)
            list->head = cur->next;
        /* update tail */
        if (cur_index == list->len - 1)
            list->tail = cur->prev;

        _free_node(cur, list->mm.free_f);

        cur = next;
        ++start;
        --list->len;
    }
}

/*
dlist_swap(struct dlist *list, size_t index0, size_t index1)

swap the node of 'index0' and 'index1'

time complexity:
worse-case: O(n)
average: O(n)
best-case: O(1)
space complexity: O(1)
*/
void dlist_swap(struct dlist *list, size_t index0, size_t index1)
{
    if (!list)
    {
        fprintf(stderr, "'list' cannot be empty.\n");
        return;
    }
    if (dlist_is_empty(list))
    {
        fprintf(stderr, "'list' is empty.\n");
        return;
    }
    if (index0 >= list->len || index1 >= list->len)
    {
        fprintf(stderr, "index out of bounds.\n");
        return;
    }
    if (index0 == index1)
        return;

    struct dlist_node *node0 = dlist_at_node(list, index0);
    struct dlist_node *node1 = dlist_at_node(list, index1);

    struct dlist_node *tmp = node0->prev;
    node0->prev = node1->prev;
    node1->prev = tmp;

    tmp = node0->next;
    node0->next = node1->next;
    node1->next = tmp;

    if (node0->prev)
        node0->prev->next = node0;
    else
        list->head = node0;

    if (node0->next)
        node0->next->prev = node0;
    else
        list->tail = node0;

    if (node1->prev)
        node1->prev->next = node1;
    else
        list->head = node1;

    if (node1->next)
        node1->next->prev = node1;
    else
        list->tail = node1;
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
    if (!list)
    {
        fprintf(stderr, "'list' cannot be empty.\n");
        return;
    }
    if (!list->mm.alloc_f)
    {
        fprintf(stderr, "'alloc_f' cannot be empty.\n");
        return;
    }

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
