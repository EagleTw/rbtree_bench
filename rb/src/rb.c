#include "rb.h"
#include <memory.h>

/* Left accessors */
static inline map_node_t *rbtn_left_get(map_node_t *node)
{
    return node->link.left;
}

static inline void rbtn_left_set(map_node_t *node, map_node_t *left)
{
    node->link.left = left;
}

/* Right accessors */
static inline map_node_t *rbtn_right_get(map_node_t *node)
{
    return (map_node_t *) (((size_t) node->link.right_red) & ~3);
}

static inline void rbtn_right_set(map_node_t *node, map_node_t *right)
{
    node->link.right_red =
        (map_node_t *) (((size_t) right) |
                        (((size_t) node->link.right_red) & 1));
}

/* Color accessors */
static inline bool rbtn_red_get(map_node_t *node)
{
    return (bool) (((size_t) node->link.right_red) & 1);
}

static inline void rbtn_color_set(map_node_t *node, bool bRed)
{
    node->link.right_red =
        (map_node_t *) (((size_t) node->link.right_red & ~3) | (size_t) bRed);
}

static inline void rbtn_red_set(map_node_t *node)
{
    node->link.right_red = (map_node_t *) (((size_t) node->link.right_red) | 1);
}

static inline void rbtn_black_set(map_node_t *node)
{
    node->link.right_red =
        (map_node_t *) (((size_t) node->link.right_red) & ~3);
}

/* Node initializer */
static inline void rbt_node_init(map_node_t *node)
{
    assert((((size_t) node) & (0x1)) == 0);
    rbtn_left_set(node, NULL);
    rbtn_right_set(node, NULL);
    rbtn_red_set(node);
}

/* Internal utility macros */
#define rbtn_rotate_left(x_type, x_field, x_node, r_node)  \
    do {                                                   \
        (r_node) = rbtn_right_get((x_node));               \
        rbtn_right_set((x_node), rbtn_left_get((r_node))); \
        rbtn_left_set((r_node), (x_node));                 \
    } while (0)

#define rbtn_rotate_right(x_type, x_field, x_node, r_node) \
    do {                                                   \
        (r_node) = rbtn_left_get((x_node));                \
        rbtn_left_set((x_node), rbtn_right_get((r_node))); \
        rbtn_right_set((r_node), (x_node));                \
    } while (0)

// ----------------------------------------------------------------------------
// look up:
//      x_attr: static
//      x_prefix: internal_map_
//      x_rbt_type: map_internal_t
//      x_type: map_node_t
//      x_field: link
//      x_cmp: rbtree->cmp

typedef struct {
    map_node_t *node;
    int cmp;
} internal_map_path_entry_t;

static void internal_map_new(map_internal_t *rbtree)
{
    rbtree->root = NULL;
}

static map_node_t *internal_map_search(map_internal_t *rbtree,
                                       const map_node_t *key)
{
    int cmp;
    map_node_t *ret = rbtree->root;
    while (ret && (cmp = (rbtree->cmp)(key, ret)) != 0) {
        if (cmp < 0) {
            ret = rbtn_left_get(ret);
        } else {
            ret = rbtn_right_get(ret);
        }
    }
    return ret;
}

static void internal_map_insert(map_internal_t *rbtree, map_node_t *node)
{
    internal_map_path_entry_t path[RB_MAX_DEPTH];
    internal_map_path_entry_t *pathp;
    rbt_node_init(node);
    /* Wind. */
    path->node = rbtree->root;
    for (pathp = path; pathp->node; pathp++) {
        int cmp = pathp->cmp = (rbtree->cmp)(node, pathp->node);
        if (cmp == 0) {
            /* assert(cmp != 0); */
            break; /* If the key matches something, don't insert */
        }
        if (cmp < 0) {
            pathp[1].node = rbtn_left_get(pathp->node);
        } else {
            pathp[1].node = rbtn_right_get(pathp->node);
        }
    }
    pathp->node = node;
    /* A loop invariant we maintain is that all nodes with            */
    /* out-of-date summaries live in path[0], path[1], ..., *pathp.   */
    /* To maintain this, we have to summarize node, since we          */
    /* decrement pathp before the first iteration.                    */
    assert(!rbtn_left_get(node));
    assert(!rbtn_right_get(node));
    /* Unwind. */
    for (pathp--; (uintptr_t) pathp >= (uintptr_t) path; pathp--) {
        map_node_t *cnode = pathp->node;
        if (pathp->cmp < 0) {
            map_node_t *left = pathp[1].node;
            rbtn_left_set(cnode, left);
            if (!rbtn_red_get(left))
                return;
            map_node_t *leftleft = rbtn_left_get(left);
            if (leftleft && rbtn_red_get(leftleft)) {
                /* Fix up 4-node. */
                map_node_t *tnode;
                rbtn_black_set(leftleft);
                rbtn_rotate_right(map_node_t, link, cnode, tnode);
                cnode = tnode;
            }
        } else {
            map_node_t *right = pathp[1].node;
            rbtn_right_set(cnode, right);
            if (!rbtn_red_get(right))
                return;
            map_node_t *left = rbtn_left_get(cnode);
            if (left && rbtn_red_get(left)) {
                /* Split 4-node. */
                rbtn_black_set(left);
                rbtn_black_set(right);
                rbtn_red_set(cnode);
            } else {
                /* Lean left. */
                map_node_t *tnode;
                bool tred = rbtn_red_get(cnode);
                rbtn_rotate_left(map_node_t, link, cnode, tnode);
                rbtn_color_set(tnode, tred);
                rbtn_red_set(cnode);
                cnode = tnode;
            }
        }
        pathp->node = cnode;
    }
    /* Set root, and make it black. */
    rbtree->root = path->node;
    rbtn_black_set(rbtree->root);
}

static void internal_map_remove(map_internal_t *rbtree, map_node_t *node)
{
    internal_map_path_entry_t path[RB_MAX_DEPTH];
    internal_map_path_entry_t *pathp = NULL;
    internal_map_path_entry_t *nodep = NULL;
    /* Wind. */
    path->node = rbtree->root;
    for (pathp = path; pathp->node; pathp++) {
        int cmp = pathp->cmp = (rbtree->cmp)(node, pathp->node);
        if (cmp < 0) {
            pathp[1].node = rbtn_left_get(pathp->node);
        } else {
            pathp[1].node = rbtn_right_get(pathp->node);
            if (cmp == 0) {
                /* Find node's successor, in preparation for swap. */
                pathp->cmp = 1;
                nodep = pathp;
                for (pathp++; pathp->node; pathp++) {
                    pathp->cmp = -1;
                    pathp[1].node = rbtn_left_get(pathp->node);
                }
                break;
            }
        }
    }
    assert(nodep->node == node);
    pathp--;
    if (pathp->node != node) {
        /* Swap node with its successor. */
        bool tred = rbtn_red_get(pathp->node);
        rbtn_color_set(pathp->node, rbtn_red_get(node));
        rbtn_left_set(pathp->node, rbtn_left_get(node));
        /* If node's successor is its right child, the following code */
        /* will do the wrong thing for the right child pointer.       */
        /* However, it doesn't matter, because the pointer will be    */
        /* properly set when the successor is pruned.                 */
        rbtn_right_set(pathp->node, rbtn_right_get(node));
        rbtn_color_set(node, tred);
        /* The pruned leaf node's child pointers are never accessed   */
        /* again, so don't bother setting them to nil.                */
        nodep->node = pathp->node;
        pathp->node = node;
        if (nodep == path) {
            rbtree->root = nodep->node;
        } else {
            if (nodep[-1].cmp < 0) {
                rbtn_left_set(nodep[-1].node, nodep->node);
            } else {
                rbtn_right_set(nodep[-1].node, nodep->node);
            }
        }
    } else {
        map_node_t *left = rbtn_left_get(node);
        if (left) {
            /* node has no successor, but it has a left child.        */
            /* Splice node out, without losing the left child.        */
            assert(!rbtn_red_get(node));
            assert(rbtn_red_get(left));
            rbtn_black_set(left);
            if (pathp == path) {
                rbtree->root = left;
                /* Nothing to summarize -- the subtree rooted at the  */
                /* node's left child hasn't changed, and it's now the */
                /* root.                                              */
            } else {
                if (pathp[-1].cmp < 0) {
                    rbtn_left_set(pathp[-1].node, left);
                } else {
                    rbtn_right_set(pathp[-1].node, left);
                }
            }
            return;
        } else if (pathp == path) {
            /* The tree only contained one node. */
            rbtree->root = NULL;
            return;
        }
    }
    /* We've now established the invariant that the node has no right */
    /* child (well, morally; we didn't bother nulling it out if we    */
    /* swapped it with its successor), and that the only nodes with   */
    /* out-of-date summaries live in path[0], path[1], ..., pathp[-1].*/
    if (rbtn_red_get(pathp->node)) {
        /* Prune red node, which requires no fixup. */
        assert(pathp[-1].cmp < 0);
        rbtn_left_set(pathp[-1].node, NULL);
        return;
    }
    /* The node to be pruned is black, so unwind until balance is     */
    /* restored.                                                      */
    pathp->node = NULL;
    for (pathp--; (uintptr_t) pathp >= (uintptr_t) path; pathp--) {
        assert(pathp->cmp != 0);
        if (pathp->cmp < 0) {
            rbtn_left_set(pathp->node, pathp[1].node);
            if (rbtn_red_get(pathp->node)) {
                map_node_t *right = rbtn_right_get(pathp->node);
                map_node_t *rightleft = rbtn_left_get(right);
                map_node_t *tnode;
                if (rightleft && rbtn_red_get(rightleft)) {
                    /* In the following diagrams, ||, //, and \\      */
                    /* indicate the path to the removed node.         */
                    /*                                                */
                    /*      ||                                        */
                    /*    pathp(r)                                    */
                    /*  //        \                                   */
                    /* (b)        (b)                                 */
                    /*           /                                    */
                    /*          (r)                                   */
                    /*                                                */
                    rbtn_black_set(pathp->node);
                    rbtn_rotate_right(map_node_t, link, right, tnode);
                    rbtn_right_set(pathp->node, tnode);
                    rbtn_rotate_left(map_node_t, link, pathp->node, tnode);
                } else {
                    /*      ||                                        */
                    /*    pathp(r)                                    */
                    /*  //        \                                   */
                    /* (b)        (b)                                 */
                    /*           /                                    */
                    /*          (b)                                   */
                    /*                                                */
                    rbtn_rotate_left(map_node_t, link, pathp->node, tnode);
                }
                /* Balance restored, but rotation modified subtree    */
                /* root.                                              */
                assert((uintptr_t) pathp > (uintptr_t) path);
                if (pathp[-1].cmp < 0) {
                    rbtn_left_set(pathp[-1].node, tnode);
                } else {
                    rbtn_right_set(pathp[-1].node, tnode);
                }
                return;
            } else {
                map_node_t *right = rbtn_right_get(pathp->node);
                map_node_t *rightleft = rbtn_left_get(right);
                if (rightleft && rbtn_red_get(rightleft)) {
                    /*      ||                                        */
                    /*    pathp(b)                                    */
                    /*  //        \                                   */
                    /* (b)        (b)                                 */
                    /*           /                                    */
                    /*          (r)                                   */
                    map_node_t *tnode;
                    rbtn_black_set(rightleft);
                    rbtn_rotate_right(map_node_t, link, right, tnode);
                    rbtn_right_set(pathp->node, tnode);
                    rbtn_rotate_left(map_node_t, link, pathp->node, tnode);
                    /* Balance restored, but rotation modified        */
                    /* subtree root, which may actually be the tree   */
                    /* root.                                          */
                    if (pathp == path) {
                        /* Set root. */
                        rbtree->root = tnode;
                    } else {
                        if (pathp[-1].cmp < 0) {
                            rbtn_left_set(pathp[-1].node, tnode);
                        } else {
                            rbtn_right_set(pathp[-1].node, tnode);
                        }
                    }
                    return;
                } else {
                    /*      ||                                        */
                    /*    pathp(b)                                    */
                    /*  //        \                                   */
                    /* (b)        (b)                                 */
                    /*           /                                    */
                    /*          (b)                                   */
                    map_node_t *tnode;
                    rbtn_red_set(pathp->node);
                    rbtn_rotate_left(map_node_t, link, pathp->node, tnode);
                    pathp->node = tnode;
                }
            }
        } else {
            map_node_t *left;
            rbtn_right_set(pathp->node, pathp[1].node);
            left = rbtn_left_get(pathp->node);
            if (rbtn_red_get(left)) {
                map_node_t *tnode;
                map_node_t *leftright = rbtn_right_get(left);
                map_node_t *leftrightleft = rbtn_left_get(leftright);
                if (leftrightleft && rbtn_red_get(leftrightleft)) {
                    /*      ||                                        */
                    /*    pathp(b)                                    */
                    /*   /        \\                                  */
                    /* (r)        (b)                                 */
                    /*   \                                            */
                    /*   (b)                                          */
                    /*   /                                            */
                    /* (r)                                            */
                    map_node_t *unode;
                    rbtn_black_set(leftrightleft);
                    rbtn_rotate_right(map_node_t, link, pathp->node, unode);
                    rbtn_rotate_right(map_node_t, link, pathp->node, tnode);
                    rbtn_right_set(unode, tnode);
                    rbtn_rotate_left(map_node_t, link, unode, tnode);
                } else {
                    /*      ||                                        */
                    /*    pathp(b)                                    */
                    /*   /        \\                                  */
                    /* (r)        (b)                                 */
                    /*   \                                            */
                    /*   (b)                                          */
                    /*   /                                            */
                    /* (b)                                            */
                    assert(leftright);
                    rbtn_red_set(leftright);
                    rbtn_rotate_right(map_node_t, link, pathp->node, tnode);
                    rbtn_black_set(tnode);
                }
                /* Balance restored, but rotation modified subtree    */
                /* root, which may actually be the tree root.         */
                if (pathp == path) {
                    /* Set root. */
                    rbtree->root = tnode;
                } else {
                    if (pathp[-1].cmp < 0) {
                        rbtn_left_set(pathp[-1].node, tnode);
                    } else {
                        rbtn_right_set(pathp[-1].node, tnode);
                    }
                }
                return;
            } else if (rbtn_red_get(pathp->node)) {
                map_node_t *leftleft = rbtn_left_get(left);
                if (leftleft && rbtn_red_get(leftleft)) {
                    /*        ||                                      */
                    /*      pathp(r)                                  */
                    /*     /        \\                                */
                    /*   (b)        (b)                               */
                    /*   /                                            */
                    /* (r)                                            */
                    map_node_t *tnode;
                    rbtn_black_set(pathp->node);
                    rbtn_red_set(left);
                    rbtn_black_set(leftleft);
                    rbtn_rotate_right(map_node_t, link, pathp->node, tnode);
                    /* Balance restored, but rotation modified        */
                    /* subtree root.                                  */
                    assert((uintptr_t) pathp > (uintptr_t) path);
                    if (pathp[-1].cmp < 0) {
                        rbtn_left_set(pathp[-1].node, tnode);
                    } else {
                        rbtn_right_set(pathp[-1].node, tnode);
                    }
                    return;
                } else {
                    /*        ||                                      */
                    /*      pathp(r)                                  */
                    /*     /        \\                                */
                    /*   (b)        (b)                               */
                    /*   /                                            */
                    /* (b)                                            */
                    rbtn_red_set(left);
                    rbtn_black_set(pathp->node);
                    /* Balance restored. */
                    return;
                }
            } else {
                map_node_t *leftleft = rbtn_left_get(left);
                if (leftleft && rbtn_red_get(leftleft)) {
                    /*               ||                               */
                    /*             pathp(b)                           */
                    /*            /        \\                         */
                    /*          (b)        (b)                        */
                    /*          /                                     */
                    /*        (r)                                     */
                    map_node_t *tnode;
                    rbtn_black_set(leftleft);
                    rbtn_rotate_right(map_node_t, link, pathp->node, tnode);
                    /* Balance restored, but rotation modified        */
                    /* subtree root, which may actually be the tree   */
                    /* root.                                          */
                    if (pathp == path) {
                        /* Set root. */
                        rbtree->root = tnode;
                    } else {
                        if (pathp[-1].cmp < 0) {
                            rbtn_left_set(pathp[-1].node, tnode);
                        } else {
                            rbtn_right_set(pathp[-1].node, tnode);
                        }
                    }
                    return;
                } else {
                    /*               ||                               */
                    /*             pathp(b)                           */
                    /*            /        \\                         */
                    /*          (b)        (b)                        */
                    /*          /                                     */
                    /*        (b)                                     */
                    rbtn_red_set(left);
                }
            }
        }
    }
    /* Set root. */
    rbtree->root = path->node;
    assert(!rbtn_red_get(rbtree->root));
}

static void internal_map_destroy_recurse(map_internal_t *rbtree,
                                         map_node_t *node,
                                         void (*cb)(map_node_t *, void *),
                                         void *arg)
{
    if (!node)
        return;
    internal_map_destroy_recurse(rbtree, rbtn_left_get(node), cb, arg);
    rbtn_left_set((node), NULL);
    internal_map_destroy_recurse(rbtree, rbtn_right_get(node), cb, arg);
    rbtn_right_set((node), NULL);
    if (cb) {
        cb(node, arg);
    }
}
static void internal_map_destroy(map_internal_t *rbtree,
                                 void (*cb)(map_node_t *, void *),
                                 void *arg)
{
    internal_map_destroy_recurse(rbtree, rbtree->root, cb, arg);
    rbtree->root = NULL;
}

static map_node_t *map_create_node(void *key,
                                   void *value,
                                   size_t ksize,
                                   size_t vsize)
{
    map_node_t *node = malloc(sizeof(map_node_t));

    /* Allocate memory for the keys and values */
    node->key = malloc(ksize);
    node->val = malloc(vsize);

    /*
     * Copy over the key and values
     *
     * If the parameter passed in is NULL, make the element blank instead of
     * a segfault.
     */
    if (!key)
        memset(node->key, 0, ksize);
    else
        memcpy(node->key, key, ksize);

    if (!value)
        memset(node->val, 0, vsize);
    else
        memcpy(node->val, value, vsize);

    return node;
}

static void map_delete_node(map_t UNUSED, map_node_t *node)
{
    free(node->key);
    free(node->val);
    free(node);
}

/* Constructor */
map_t map_new(size_t s1,
              size_t s2,
              int (*cmp)(const map_node_t *, const map_node_t *))
{
    map_t tree = (map_internal_t *) malloc(sizeof(map_internal_t));
    tree->key_size = s1;
    tree->val_size = s2;
    tree->cmp = cmp;
    internal_map_new(tree);
    return tree;
}

/* Add function */
bool map_insert(map_t obj, void *key, void *val)
{
    map_node_t *node = map_create_node(key, val, obj->key_size, obj->val_size);
    internal_map_insert(obj, node);
    return true;
}

/* Get functions */
void map_find(map_t obj, map_iter_t *it, void *key)
{
    map_node_t *tmp_node = (map_node_t *) malloc(sizeof(map_node_t));
    tmp_node->key = key;
    it->node = internal_map_search(obj, tmp_node);
    free(tmp_node);
}

bool map_empty(map_t obj)
{
    return (NULL == obj->root);
}

/* Iteration */
bool map_at_end(map_t UNUSED, map_iter_t *it)
{
    return (NULL == it->node);
}

/* Remove functions */
void map_erase(map_t obj, map_iter_t *it)
{
    if (NULL == it->node)
        return;
    internal_map_remove(obj, it->node);
    map_delete_node(obj, it->node);
}

/* Empty map */
void map_clear(map_t obj)
{
    internal_map_destroy(obj, cb, NULL);
}

/* Destructor */
void map_delete(map_t obj)
{
    map_clear(obj);
    free(obj);
}
