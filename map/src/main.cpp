#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


#include "map.h"

enum {NNODES = 1000 * 1000};

int main() {
  printf("--- In map\n\n ---");

  map_t tree = map_init(int, int, map_cmp_uint);


  clock_t start = clock();
  // Insert test
  printf("  tree_insert time: %f, seconds\n", (double) (clock() - start) / CLOCKS_PER_SEC);

  // Search test
  //
  // Delete test

  return 0;
}
