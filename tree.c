#include <string.h>
#include <stdlib.h>

#include "tree.h"

tree_t *init_tree(unsigned int data_size, void (*destructor)(void *)) {
	tree_t *tree = malloc(sizeof(tree_t));
	tree->data_size = data_size;
	tree->root = NULL;
	tree->destructor = destructor;
	return tree;
}

tree_node_t *init_node(void *data, unsigned int data_size,
					   tree_node_t *parent) {
	tree_node_t *node = malloc(sizeof(tree_node_t));
	node->data = malloc(data_size);
	memcpy(node->data, data, data_size);
	node->children = init_list(sizeof(tree_node_t *), NULL);
	node->parent = parent;
	if (!parent) {
		node->depth = 0;
		node->ancestors = NULL;
	} else {
		node->depth = parent->depth + 1;
		int log = 0;
		while ((1 << log) <= node->depth)
			log++;
		node->ancestors = malloc(log * sizeof(tree_node_t *));
		node->ancestors[0] = node->parent;
		for (int i = 1; i < log; i++) {
			tree_node_t *prv_ancestor = node->ancestors[i - 1];
			tree_node_t *curr_ancestor = prv_ancestor->ancestors[i - 1];
			node->ancestors[i] = curr_ancestor;
		}
	}
	return node;
}

void add_root(tree_t *tree, void *data) {
	DIE(tree->root, "This tree already has a root!\n");
	tree->root = init_node(data, tree->data_size, NULL);
}

void add_node(tree_t *tree, tree_node_t *node, void *data, void *parent_data,
			  int (*cmp_function)(void *, void *)) {
	if (!cmp_function(parent_data, node->data)) {
		tree_node_t *child = init_node(data, tree->data_size, node);
		list_insert_to_tail(node->children, &child);
		return;
	}
	ll_node_t *ll_node = node->children->head;
	for (size_t i = 0; i < node->children->size; i++) {
		tree_node_t *child = *(tree_node_t **)ll_node->data;
		add_node(tree, child, data, parent_data, cmp_function);
		ll_node = ll_node->nxt;
	}
}

int check_node(void *data1, void *data2) {
	tree_node_t *node1 = *(tree_node_t **)data1;
	tree_node_t *node2 = *(tree_node_t **)data2;
	return (node1 != node2);
}

tree_node_t *tree_find_node(tree_t *tree, tree_node_t *node, void *data,
							int (*cmp_function)(void *, void *)) {
	if (!cmp_function(data, node->data))
		return node;
	ll_node_t *ll_node = node->children->head;
	for (size_t i = 0; i < node->children->size; i++) {
		tree_node_t *child = *(tree_node_t **)ll_node->data;
		tree_node_t *found_node = tree_find_node(tree, child, data,
												 cmp_function);
		if (found_node)
			return found_node;
		ll_node = ll_node->nxt;
	}
	return NULL;
}

tree_node_t *compute_lca(tree_node_t *node1, tree_node_t *node2) {
	if (node1->depth > node2->depth) {
		tree_node_t *aux = node1;
		node1 = node2;
		node2 = aux;
	}
	int diff_depth = node2->depth - node1->depth;
	int log = 0;
	while ((1 << log) <= diff_depth)
		log++;
	for (int i = log; i >= 0; i--)
		if (diff_depth & (1 << i))
			node2 = node2->ancestors[i];
	if (node1 == node2)
		return node1;
	log = 0;
	while ((1 << log) <= node1->depth)
		log++;
	for (int i = log; i >= 0; i--) {
		if ((1 << i) <= node1->depth &&
			node1->ancestors[i] != node2->ancestors[i]) {
			node1 = node1->ancestors[i];
			node2 = node2->ancestors[i];
		}
	}
	return node1->parent;
}

void dfs(tree_t *tree, tree_node_t *node, void (*function)(void *)) {
	function(node->data);
	ll_node_t *ll_node = node->children->head;
	for (size_t i = 0; i < node->children->size; i++) {
		tree_node_t *child = *(tree_node_t **)ll_node->data;
		dfs(tree, child, function);
		ll_node = ll_node->nxt;
	}
}

tree_node_t *find_max(tree_t *tree, tree_node_t *node,
					  int (*cmp_function)(void *, void *)) {
	tree_node_t *max = node;
	ll_node_t *ll_node = node->children->head;
	for (size_t i = 0; i < node->children->size; i++) {
		tree_node_t *child = *(tree_node_t **)ll_node->data;
		tree_node_t *candidate = find_max(tree, child, cmp_function);
		if (cmp_function(candidate->data, max->data) > 0)
			max = candidate;
		ll_node = ll_node->nxt;
	}
	return max;
}

void free_node(tree_node_t *node, void (*destructor)(void *)) {
	if (destructor)
		destructor(node->data);
	free_list(node->children);
	free(node->ancestors);
	free(node->data);
	free(node);
}

void delete_subtree(tree_t *tree, tree_node_t *node) {
	ll_node_t *ll_node = node->children->head;
	for (size_t i = 0; i < node->children->size; i++) {
		tree_node_t *child = *(tree_node_t **)ll_node->data;
		delete_subtree(tree, child);
		ll_node = ll_node->nxt;
	}
	free_node(node, tree->destructor);
}

void free_tree(tree_t *tree) {
	delete_subtree(tree, tree->root);
	free(tree);
}
