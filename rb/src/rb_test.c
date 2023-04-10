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

enum { NNODES = 30 };

bool test_mix_insert()
{
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
        int pos_a = rand() % NNODES;
        int pos_b = rand() % NNODES;
        swap(&key[pos_a], &key[pos_b]);
        swap(&val[pos_a], &val[pos_b]);
    }

    /* add first 1/2 items */
    for (int i = 0; i < NNODES / 2; i++) {
        map_iter_t my_it;
        map_insert(tree, key + i, val + i);
        map_find(tree, &my_it, key + i);
        if (NULL == my_it.node) {
            failed = true;
        }
        printf("| index: %d\t| key: %d\t| value: %d\t| failed?: %d\t|\n", i,
               key[i], *(int *) (my_it.node->val), failed);
    }

    printf("\n==============================\n");

    /* remove first 1/4 items */
    for (int i = 0; i < NNODES / 4; i++) {
        map_iter_t my_it;
        my_it.node->key = key + i;
        my_it.node->val = val + i;
        //FIXME: Assertion fail
        map_erase(tree, &my_it);
        map_find(tree, &my_it, key + i);
        if (NULL != my_it.node) {
            failed = true;
        }
        //        printf("| index: %d\t| key: %d\t| value: %d\t| failed?:
        //        %d\t|\n", i, key[i], *(int*)(my_it.node->val), failed);
    }

    /* TODO: add the rest */
    /* TODO: remove not exsites items */

    return !failed;
}

int main(int argc, char *argv[])
{
    printf("=== start testfile ====\n");

    bool passed = test_mix_insert();
    printf("test pass? %d\n", passed);

    return 0;
}
