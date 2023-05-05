#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <uuid/uuid.h>

#include "rb.h"

void swap(size_t *x, size_t *y)
{
    size_t tmp = *x;
    *x = *y;
    *y = tmp;
}

static void perf_rb(const char *benchmark_id,
                    const size_t scale,
                    const size_t reps)
{
    map_t tree = map_init(long, long, map_cmp_sizet);

    size_t *key = malloc(scale * sizeof(size_t));
    size_t *val = malloc(scale * sizeof(size_t));

    ///* Generate data */
    for (size_t i = 0; i < scale; i++) {
        key[i] = i;
        val[i] = i;
    }

    for (size_t i = 0; i < scale; i++) {
        size_t pos_a = arc4random() % scale;
        size_t pos_b = arc4random() % scale;
        swap(&key[pos_a], &key[pos_b]);
        swap(&val[pos_a], &val[pos_b]);
    }

    /* Insert */
    for (size_t i = 0; i < scale; i++) {
        map_insert(tree, key + i, val + i);
    }

    ///* Find */
    for (size_t i = 0; i < scale; i++) {
        map_iter_t my_it;
        map_find(tree, &my_it, key);
    }

    ///* Remove */
    for (size_t i = 0; i < scale; i++) {
        map_iter_t my_it;
        map_find(tree, &my_it, key + i);
        if (!map_at_end(tree, &my_it)) {
            map_erase(tree, &my_it);
        }
    }

    map_delete(tree);
    free(key);
    free(val);
}

int main(int argc, char *argv[])
{
    /* generate benchmark id */
    uuid_t uuid;
    uuid_generate_random(uuid);
    char benchmark_id[37];
    uuid_unparse_lower(uuid, benchmark_id);

    size_t scale[] = {1e3, 1e4, 1e5, 1e6, 1e7};
    size_t n_scales = 5;
    size_t reps = 20;

    for (size_t i = 0; i < n_scales; i++) {
        perf_rb(benchmark_id, scale[i], reps);
    }

    return 0;
}
