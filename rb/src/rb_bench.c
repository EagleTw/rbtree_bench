#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <uuid/uuid.h>

#include "rb.h"

void swap(int *x, int *y)
{
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

static void perf_rb(const char *benchmark_id,
                    const size_t scale,
                    const size_t reps)
{
}

int main(int argc, char *argv[])
{
    /* generate benchmark id */
    uuid_t uuid;
    uuid_generate_random(uuid);
    char benchmark_id[37];
    uuid_unparse_lower(uuid, benchmark_id);

    /* get a timestamp */
    time_t now = time(0);

    size_t scale[] = {1e3, 1e4, 1e5, 1e6, 1e7};
    size_t n_scales = 5;
    size_t reps = 20;

    for (size_t i = 0; i < n_scales; i++) {
        perf_rb(benchmark_id, scale[i], reps);
    }

    return 0;
}
