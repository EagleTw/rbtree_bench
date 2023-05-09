#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* C macro implementation of left-leaning 2-3 red-black trees.  Parent
 * pointers are not used, and color bits are stored in the least significant
 * bit of right-child pointers, thus making node linkage as compact as is
 * possible for red-black trees.
 */

/* Each node in the RB tree consumes at least 1 byte of space (for the linkage
 * if nothing else, so there are a maximum of sizeof(void *) << 3 rb tree nodes
 * in any process (and thus, at most sizeof(void *) << 3 nodes in any rb tree).
 * The choice of algorithm bounds the depth of a tree to twice the binary log of
 * the number of elements in the tree; the following bound follows.
 */
#define RB_MAX_DEPTH (sizeof(void *) << 4)

/* Node structure */
#define rb_node(x_type)           \
    struct {                      \
        x_type *left, *right_red; \
    }

typedef struct map_node map_node_t;
struct map_node {
    void *key;
    void *val;
    rb_node(map_node_t) link;
};

typedef struct rb_tree {
    map_node_t *root;
    int (*cmp)(void *, void *);
    size_t key_size;
    size_t val_size;
} map_internal_t;

typedef map_internal_t *map_t;

typedef struct {
    map_node_t *prev, *node;
    size_t count;
} map_iter_t;

#define map_iter_value(it, type) (*(type *) (it)->node->val)

enum { _CMP_LESS = -1, _CMP_EQUAL = 0, _CMP_GREATER = 1 };

/* Integer comparison */
static inline int map_cmp_int(const map_node_t *arg0, const map_node_t *arg1)
{
    int *a = (int *) arg0->key;
    int *b = (int *) arg1->key;
    return (*a < *b) ? _CMP_LESS : (*a > *b) ? _CMP_GREATER : _CMP_EQUAL;
}

/* Unsigned integer comparison */
static inline int map_cmp_uint(const map_node_t *arg0, const map_node_t *arg1)
{
    unsigned int *a = (unsigned int *) arg0->key;
    unsigned int *b = (unsigned int *) arg1->key;
    return (*a < *b) ? _CMP_LESS : (*a > *b) ? _CMP_GREATER : _CMP_EQUAL;
}

static inline int map_cmp_sizet(const map_node_t *arg0, const map_node_t *arg1)
{
    size_t *a = (size_t *) arg0->key;
    size_t *b = (size_t *) arg1->key;
    return (*a < *b) ? _CMP_LESS : (*a > *b) ? _CMP_GREATER : _CMP_EQUAL;
}

static inline void cb(map_node_t *node, void *UNUSED)
{
    free(node);
}

/* Constructor */
map_t map_new(size_t, size_t, int (*)(const map_node_t *, const map_node_t *));

/* Add function */
bool map_insert(map_t, void *, void *);

/* Get functions */
void map_find(map_t, map_iter_t *, void *);
bool map_empty(map_t);

/* Iteration */
bool map_at_end(map_t, map_iter_t *);

/* Remove functions */
void map_erase(map_t, map_iter_t *);
void map_clear(map_t);

/* Destructor */
void map_delete(map_t);

#define map_init(key_type, element_type, __func) \
    map_new(sizeof(key_type), sizeof(element_type), __func)
