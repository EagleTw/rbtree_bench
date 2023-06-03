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

enum { NNODES = 10000 };

bool test_mix_insert()
{
    bool failed = false;

    map_t tree = map_init(int, int, map_cmp_uint);

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
        printf(
            "Insert | index: %d\t| it.node->data: %d\t| value: %d\t| failed?: "
            "%d\t|\n",
            i, key[i], *(int *) (my_it.node->data), failed);
    }

    printf("\n==============================\n\n");

    /* remove first 1/4 items */
    for (int i = 0; i < NNODES / 4; i++) {
        map_iter_t my_it;
        map_find(tree, &my_it, key + i);
        if (!map_at_end(tree, &my_it)) {
            map_erase(tree, &my_it);
            map_find(tree, &my_it, key + i);
            if (NULL != my_it.node) {
                failed = true;
            }
        }
        printf(
            "Erase  | index: %d\t| key: %d\t| it.node: %p\t| "
            "failed?: %d\t|\n",
            i, key[i], my_it.node, failed);
    }

    printf("\n==============================\n\n");

    /* add the rest */
    for (int i = NNODES / 2 + 1; i < NNODES; i++) {
        map_iter_t my_it;
        map_insert(tree, key + i, val + i);
        map_find(tree, &my_it, key + i);
        if (NULL == my_it.node) {
            failed = true;
        }
        printf(
            "Insert | index: %d\t| it.node->data: %d\t| value: %d\t| failed?: "
            "%d\t|\n",
            i, key[i], *(int *) (my_it.node->data), failed);
    }

    printf("\n==============================\n\n");

    /* remove 2nd quarter of items */
    for (int i = NNODES / 4 + 1; i < NNODES / 2; i++) {
        map_iter_t my_it;
        map_find(tree, &my_it, key + i);
        if (!map_at_end(tree, &my_it)) {
            map_erase(tree, &my_it);
            map_find(tree, &my_it, key + i);
            if (NULL != my_it.node) {
                failed = true;
            }
        }
        printf(
            "Erase  | index: %d\t| key: %d\t| it.node: %p\t| "
            "failed?: %d\t|\n",
            i, key[i], my_it.node, failed);
    }

    // printf("\n[ori]   tree: %p\t| tree->root: %p\t| Map empty? %d\n", tree,
    //        tree->root, map_empty(tree));
    map_clear(tree);
    // printf("[clear] tree: %p\t| tree->root: %p\t| Map empty? %d\n", tree,
    //        tree->root, map_empty(tree));
    map_delete(tree);
    // printf("[delete]tree: %p\t| tree->root: %p\t| Map empty? %d\n", tree,
    //        tree->root, map_empty(tree));

    return !failed;
}

int main(int argc, char *argv[])
{
    printf("=== start testfile ====\n");

    bool passed = test_mix_insert();
    printf("\n\ntest pass? %d\n", passed);

    return 0;
}
