#include <stdio.h>
#include <stdlib.h>
#include "map.c"


int main() {
    
    map_node_t *node;
    rb_path_entry_t *path;
    
    printf("1. %p\n", path->node_cmp);
    rb_path_entry_set_node(path, node);
    printf("2. %p\n", path->node_cmp);

    return 0;
}
