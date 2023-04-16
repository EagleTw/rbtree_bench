#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

int main(int argc, char *argv[])
{
    int val[5];
    map_t tree = map_init(int, int, map_cmp_int);

    for (int i = 0; i < 5; i++) {
        val[i] = i;
        map_insert(tree, &i, val + i);
        printf("insert\t(key: %d, value: %d),\taddr (%p, %p)\n", i, val[i], &i,
               val + i);
        map_iter_t it;
        map_find(tree, &it, &i);
        if (!map_at_end(tree, &it)) {
            printf("find\t(key: %d, value: %d),\taddr (%p, %p)\n", i,
                   *(int *) it.node->val, &i, val + i);
        } else {
            printf("find xxx\t(key: %d, value: %d),\taddr (%p, %p)\n", i, 0, &i,
                   val + i);
        }
    }

    for (int j = 0; j < 5; j++) {
        map_iter_t it;
        map_find(tree, &it, &j);
        if (!map_at_end(tree, &it)) {
            printf("find\t(key: %d, value: %d),\taddr (%p, %p)\n", j,
                   *(int *) it.node->val, &j, val + j);
        } else {
            printf("find xxx\t(key: %d, value: %d),\taddr (%p, %p)\n", j, 0, &j,
                   val + j);
        }
    }


    return 0;
}
