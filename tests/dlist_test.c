#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dlist.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    {
        srand(time(NULL));
        dlist_t list = dlist_create();

        int rand_ns[20];
        for (int i = 0; i < 20; ++i)
        {
            rand_ns[i] = rand();
            dlist_insert(&list, &rand_ns[i], i);
        }

        for (int i = 0; i < 20; ++i)
        {
            int value = dlist_at_cast(&list, i, int);

            if (value != rand_ns[i])
                printf("Test Failed at index %d.\n", i);
        }

        dlist_free(&list);
    }

    return 0;
}
