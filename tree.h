#ifndef TREE_H
#define TREE_H

#include "linked_list.h"
#include "utils.h"

typedef struct tree_node_t tree_node_t;
typedef struct tree_t tree_t;

struct tree_node_t {
	void *data;
	linked_list_t *children;
	tree_node_t *parent;
	int depth;
	tree_node_t **ancestors;
};

struct tree_t {
	tree_node_t *root;
	unsigned int data_size;
	void (*destructor)(void *data);
};

/**
 * Creates a tree
 * @param data_size - The size of the data that will be stored in each node
 * @param destructor - The function that frees the memory that
 * data is pointing to, NULL if there is nothing to free
*/
tree_t *init_tree(unsigned int data_size, void (*destructor)(void *));

/**
 * Inserts a new node as a child of the given parent
 * @param data - The data of the node
 * @param data_size
 * @param parent - The parent of the new node
*/
tree_node_t *init_node(void *data, unsigned int data_size, tree_node_t *parent);

/**
 * Adds a tree's root
 * @param tree
 * @param data
*/
void add_root(tree_t *tree, void *data);

/**
 * Adds a new node to the tree, given its parent's data
 * @param tree
 * @param node
 * @param data
 * @param parent_data
 * @param cmp_function
*/
void add_node(tree_t *tree, tree_node_t *node, void *data, void *parent_data,
			  int (*cmp_function)(void *, void *));

/**
 * Checking if two tree_nodes are the same
 * Needed for other tasks
*/
int check_node(void *data1, void *data2);

/**
 * Returns the tree_node containing the given data
 * @param tree
 * @param node
 * @param data
 * @param cmp_function
*/
tree_node_t *tree_find_node(tree_t *tree, tree_node_t *node, void *data,
							int (*cmp_function)(void *, void *));

/**
 * Computes the LCA of two given nodes using binary lifting
 * @param node1
 * @param node2
*/
tree_node_t *compute_lca(tree_node_t *node1, tree_node_t *node2);

/**
 * Does a dfs traversal of the given tree
 * And applies the given function
 * @param tree
 * @param node
 * @param function
*/
void dfs(tree_t *tree, tree_node_t *node, void (*function)(void *));

/**
 * Returns the node containing the maximum
 * It's determined by using the compare function given\
 * @param tree
 * @param node
 * @param cmp_function
*/
tree_node_t *find_max(tree_t *tree, tree_node_t *node,
					  int (*cmp_function)(void *, void *));

/**
 *  Frees the memory occupied by a given node and its subtree
 * @param tree
 * @param node
*/
void delete_subtree(tree_t *tree, tree_node_t *node);

/**
 *  Frees the memory occupied by a tree
*/
void free_tree(tree_t *tree);

#endif // TREE_H
