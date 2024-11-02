#include <stdio.h>

#include "dlist.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    dlist_t list = dlist_create();

    int x = 123;
    dlist_insert(&list, &x, 0);

    dlist_free(&list);

    return 0;
}
