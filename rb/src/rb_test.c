#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rb.h"

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

enum { NNODES = 30};

bool test_mix_insert() {
    bool failed = false;

    map_t tree = map_init(int, int, uint_key_cmp);

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
    for (int i = 0; i < NNODES; i++) {
        int pos_a = rand() % NNODES; int pos_b = rand() % NNODES;
        swap(&key[pos_a], &key[pos_b]);
        swap(&val[pos_a], &val[pos_b]);
    }

    for (int i = 0; i < NNODES; i++) {
        map_iter_t* my_it = NULL;
        map_insert(tree, key + i, val + i);
        map_find(tree, my_it, key + i);
        if (NULL == my_it) { failed = 1; }
    }

}

int main(int argc, char *argv[])
{
    printf("=== start testfile ====\n");
    return 0;
}
