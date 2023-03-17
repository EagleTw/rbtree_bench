#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "map.c"
#include "map.h"

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

enum { NNODES = 1000 * 1000 };

int main()
{
    printf("===== Self benchmark: map =====\n");

    map_t tree = map_init(int, int, map_cmp_int);

    int key[NNODES];
    int val[NNODES];

    /*
     *  Generate data for insertion
     */
    for (int i = 0; i < NNODES; i++) {
        key[i] = i;
        val[i] = i;
    }

    /* This is not a reconmended way to randomize stuff, just a simple test */
    /* I know MT19937 might be better */
    srand((unsigned) time(NULL));
    for (int i = 0; i < 1e7; i++) {
        int pos_a = rand() % NNODES;
        int pos_b = rand() % NNODES;
        swap(&key[pos_a], &key[pos_b]);
        swap(&val[pos_a], &val[pos_b]);
    }

    /*
     * Insertion clcoking
     */
    clock_t start = clock();
    for (int i = 0; i < NNODES; i++) {
        map_insert(tree, key + i, val + i);
    }
    printf("  tree_insert time: %f, seconds\n",
           (double) (clock() - start) / CLOCKS_PER_SEC);

    /*
     * Seaching clcoking
     */
    start = clock();
    map_iter_t my_it;
    for (int i = 0; i < NNODES; i++) {
        map_find(tree, &my_it, key + i);
    }
    printf("  tree_find time: %f, seconds\n",
           (double) (clock() - start) / CLOCKS_PER_SEC);

    /*
     * Deletion clcoking
     */
    start = clock();
    map_delete(tree);
    printf("  tree_delete time: %f, seconds\n",
           (double) (clock() - start) / CLOCKS_PER_SEC);

    return 0;
}
