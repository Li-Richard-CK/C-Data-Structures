#include "dlist.h"

#include <__stddef_size_t.h>
#include <__stddef_null.h>
#include <stdarg.h>

struct dlist_node
{
    void *next, *prev;
    size_t index;
};

struct dlist dlist_create(...)
{
    struct dlist new_list;

    new_list.len = 0;
    new_list.head = NULL;
    new_list.tail = NULL;

        return new_list;
}
