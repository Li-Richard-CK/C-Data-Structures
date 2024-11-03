#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dlist.h"

int cmp_f(const void *a, const void *b)
{
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;

    return (arg1 > arg2) - (arg1 < arg2);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    srand(time(NULL));
    dlist_t list = dlist_create();
    list.cmp_f = cmp_f;

    int rand_ns[20];
    for (int i = 0; i < 20; ++i)
    {
        rand_ns[i] = rand();
        dlist_append(&list, &rand_ns[i]);
    }

    for (int i = 0; i < 20; ++i)
    {
        int value = dlist_at_cast(&list, i, int);
        if (value != rand_ns[i])
            printf("Test Failed at index %d.\n", i);
    }

    printf("\n");
    for (int i = 0; i < 20; ++i)
    {
        int value = dlist_at_cast(&list, i, int);
        printf("%d\n", value);
    }

    dlist_erase(&list, 5, 9);

    dlist_swap(&list, 0, list.len - 1);

    printf("\n");
    for (int i = 0; i < list.len; ++i)
    {
        int value = dlist_at_cast(&list, i, int);
        printf("%d\n", value);
    }

    dlist_free(&list);

    return 0;
}
