#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <random>

#include "rb.h"

using std::cout;
using std::endl;

struct node_;
typedef struct node_ node_t;

struct node_ {
    rb_node(node_t) link;
    int key;
};

static int node_cmp(const node_t *a, const node_t *b)
{
    int ret = (a->key > b->key) - (a->key < b->key);
    if (ret == 0 && a != b)
        assert(0 && "Duplicates are not allowed in the tree");
    return (ret);
}

typedef rb_tree(node_t) tree_t;
rb_gen(static, tree_, tree_t, node_t, link, node_cmp);

typedef int value_t;

enum {
    LEFT = 0,
    RIGHT = 1,
    NEITHER = 3,
};

typedef int direction;

struct node_s;

class node
{
public:
    node() : m_node(NULL) {}
    explicit node(node_s *n) : m_node(n) {}

    node_s *operator->() const { return as_node(); }
    explicit operator bool() const { return m_node != NULL; }
    int longer() const { return uintptr_t(m_node) & 3; }
    void set_longer(int longer)
    {
        assert(m_node);
        m_node = (node_s *) ((intptr_t(m_node) & ssize_t(-4)) | longer);
    }
    bool balanced() const { return (uintptr_t(m_node) & 2) != 0; }

    bool operator==(const node &other) const
    {
        return as_node() == other.as_node();
    }
    bool operator!=(const node &other) const
    {
        return as_node() != other.as_node();
    }

private:
    node_s *as_node() const
    {
        return (node_s *) (intptr_t(m_node) & ssize_t(-4));
    }

    node_s *m_node;
};

struct node_s {
    value_t value;
    node next[2];
};

int dir_check_depth(node tree)
{
    if (!tree)
        return 0;

    int err = 0;
    int rv;
    int b = dir_check_depth(tree->next[LEFT]);
    int f = dir_check_depth(tree->next[RIGHT]);
    if (b == f) {
        if (!tree.balanced())
            err = 1;
        rv = b + 1;
    } else if (b == f - 1) {
        if (tree.longer() != RIGHT)
            err = 1;
        rv = f + 1;
    } else if (b - 1 == f) {
        if (tree.longer() != LEFT)
            err = 1;
        rv = b + 1;
    } else {
        err = 1;
        rv = 0;
    }
    if (err)
        printf("err at %d: b=%d f=%d longer=%d\n", tree->value, b, f,
               tree.longer());
    return rv;
}

std::pair<unsigned int, unsigned long long> doGetGepth(node_t *tree,
                                                       node_t *null,
                                                       unsigned int level)
{
    if (tree == null)
        return std::make_pair(0U, 0ULL);
    auto left(doGetGepth(rbtn_left_get(node_t, link, tree), null, level + 1)),
        right(doGetGepth(rbtn_right_get(node_t, link, tree), null, level + 1));
    return {1U + left.first + right.first, left.second + right.second + level};
}

std::pair<unsigned int, unsigned long long> doGetGepth(node tree,
                                                       node null,
                                                       unsigned int level)
{
    if (tree == null)
        return std::make_pair(0U, 0ULL);
    auto left(doGetGepth(tree->next[0], null, level + 1)),
        right(doGetGepth(tree->next[1], null, level + 1));
    return {1U + left.first + right.first, left.second + right.second + level};
}

template <typename T>
double getDepth(T tree, T null = static_cast<T>(0))
{
    auto depth(doGetGepth(tree, null, 0U));
    return depth.first ? double(depth.second) / depth.first : 0;
}

enum { NNODES = 1000 * 1000 };

int main(int argc, char *argv[])
{
    node_t *nodes = new node_t[NNODES];

    std::default_random_engine dre;
    for (int i = 0; i < NNODES; ++i) {
        std::uniform_int_distribution<int> di(0, i);
        const int j = di(dre);
        if (i != j)
            nodes[i].key = nodes[j].key;
        nodes[j].key = i;
    }

    tree_t tree;
    tree_new(&tree);

    clock_t start = clock();

    for (int i = 0; i < NNODES; ++i)
        tree_insert(&tree, nodes + i);

    cout << "  tree_insert time: "
         << (double) (clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

    cout << "    RB depth: " << getDepth(tree.root) << endl;

    start = clock();

    for (int i = 0; i < NNODES; ++i) {
        if (!tree_search(&tree, nodes + i))
            cout << "Strange failure to tree_search " << nodes[i].key << '\n';
    }

    cout << "  tree_search time: "
         << (double) (clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

    start = clock();

    for (int i = 0; i < NNODES; ++i) {
        if (!tree_search(&tree, nodes + nodes[i].key))
            cout << "Strange failure to tree_search " << nodes[i].key << '\n';
    }

    cout << "  alternative tree_search time: "
         << (double) (clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

    start = clock();

    for (int i = 0; i < NNODES; ++i) {
        tree_remove(&tree, nodes + i);
    }

    cout << "  tree_remove time: "
         << (double) (clock() - start) / CLOCKS_PER_SEC << " seconds" << endl;

    delete[] nodes;
    tree_destroy(&tree, NULL, NULL);

    cout << endl;

    return 0;
}
