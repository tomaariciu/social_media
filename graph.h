#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

#include "linked_list.h"

typedef struct graph_t graph_t;

struct graph_t {
	unsigned int size;
	linked_list_t **neighbors;
};

/**
 * Creates a graph of a given size
 * @param graph_size - The size of the graph
 * @return a pointer to the newly created graph
*/
graph_t *init_graph(unsigned int graph_size);

/**
 * Returns the neighbor list of a given node
*/
linked_list_t *get_neighbors(graph_t *graph, uint16_t node);

/**
 * Adds an edge between two nodes
*/
void add_edge(graph_t *graph, uint16_t node1, uint16_t node2);

/**
 * Removes the edge between two nodes
*/
void remove_edge(graph_t *graph, uint16_t node1, uint16_t node2);

/**
 * Does a BFS traversal of a graph starting with a source node
 * @param graph
 * @param source
 * @param max_dist - The maximum distance that can be reached from the source,
 * -1 for no limit
 * @return - The distance array storing the distance from the source node
 * or -1 if it can't be reached
*/
int *bfs(graph_t *graph, uint16_t source, int max_dist);

/**
 * @param graph
 * @param source_node
 * @return - The maximum clique containing the source node
*/
linked_list_t *max_clique(graph_t *graph, uint16_t source_node);

/**
 * Frees the memory occupied by a graph
 * @param graph
*/
void free_graph(graph_t *graph);

/**
 * Compares two nodes - used to keep the neighbors lists sorted
 * @return < 0 if the first node is smaller, > 0 if it is larger,
 * = 0 if they are equal
*/
int node_cmp(void *data1, void *data2);

#endif // GRAPH_H
