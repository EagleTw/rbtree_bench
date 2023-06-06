#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"

void swap(size_t *x, size_t *y)
{
    size_t tmp = *x;
    *x = *y;
    *y = tmp;
}

static inline int map_cmp_sizet(const void *arg0, const void *arg1)
{
    size_t *a = (size_t *) arg0;
    size_t *b = (size_t *) arg1;
    return (*a < *b) ? _CMP_LESS : (*a > *b) ? _CMP_GREATER : _CMP_EQUAL;
}

static void perf_rb(const char *benchmark_id,
                    const size_t scale,
                    const size_t reps)
{
    if (reps == 0) {
        return;
    }

    map_t tree = map_init(long, long, map_cmp_sizet);

    size_t *key = malloc(scale * sizeof(size_t));
    size_t *val = malloc(scale * sizeof(size_t));

    ///* Generate data */
    for (size_t i = 0; i < scale; i++) {
        key[i] = i;
        val[i] = i;
    }

    for (size_t i = 0; i < scale; i++) {
        int pos_a = rand() % scale;
        int pos_b = rand() % scale;
        swap(&key[pos_a], &key[pos_b]);
        swap(&val[pos_a], &val[pos_b]);
    }

    struct timespec before;
    struct timespec after;
    clock_gettime(CLOCK_MONOTONIC, &before);
    /* Insert */
    for (size_t i = 0; i < scale; i++) {
        map_insert(tree, key + i, val + i);
    }
    clock_gettime(CLOCK_MONOTONIC, &after);
    double result = (after.tv_sec - before.tv_sec) * 1000000000UL +
                    (after.tv_nsec - before.tv_nsec);
    printf("%f, \"%s\", \"%s\", %zu, %zu\n", result, benchmark_id, "insert",
           scale, reps);

    clock_gettime(CLOCK_MONOTONIC, &before);
    ///* Find */
    for (size_t i = 0; i < scale; i++) {
        map_iter_t my_it;
        map_find(tree, &my_it, key);
    }
    clock_gettime(CLOCK_MONOTONIC, &after);
    result = (after.tv_sec - before.tv_sec) * 1000000000UL +
             (after.tv_nsec - before.tv_nsec);
    printf("%f, \"%s\", \"%s\", %zu, %zu\n", result, benchmark_id, "find",
           scale, reps);

    clock_gettime(CLOCK_MONOTONIC, &before);
    ///* Remove */
    for (size_t i = 0; i < scale; i++) {
        map_iter_t my_it;
        map_find(tree, &my_it, key + i);
        if (!map_at_end(tree, &my_it)) {
            map_erase(tree, &my_it);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &after);
    result = (after.tv_sec - before.tv_sec) * 1000000000UL +
             (after.tv_nsec - before.tv_nsec);
    printf("%f, \"%s\", \"%s\", %zu, %zu\n", result, benchmark_id, "erase",
           scale, reps);

    map_delete(tree);
    free(key);
    free(val);

    perf_rb(benchmark_id, scale, reps - 1);
}

int main(int argc, char *argv[])
{
    char* benchmark_id = "no_bm_id";
    size_t scale[] = {/*1, 1e1, 1e2,*/ 1e3, 1e4, 1e5, 1e6/*, 1e7, 1e8*/};
    size_t n_scales = 4;
    size_t reps = 20;

    for (size_t i = 0; i < n_scales; i++) {
        perf_rb(benchmark_id, scale[i], reps);
    }

    return 0;
}
