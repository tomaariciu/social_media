#include <stdint.h>
#include <stdio.h>

#include "graph.h"
#include "queue.h"

graph_t *init_graph(unsigned int graph_size) {
	graph_t *graph = malloc(sizeof(graph_t));
	graph->size = graph_size;
	graph->neighbors = malloc(graph_size * sizeof(linked_list_t *));
	for (size_t i = 0; i < graph_size; i++)
		graph->neighbors[i] = init_list(sizeof(uint16_t), NULL);
	return graph;
}

linked_list_t *get_neighbors(graph_t *graph, uint16_t node) {
	return graph->neighbors[node];
}

void add_edge(graph_t *graph, uint16_t node1, uint16_t node2) {
	list_insert_sorted(graph->neighbors[node1], &node2, node_cmp);
	list_insert_sorted(graph->neighbors[node2], &node1, node_cmp);
}

void remove_edge(graph_t *graph, uint16_t node1, uint16_t node2) {
	ll_node_t *ll_node;
	ll_node = list_find_node(graph->neighbors[node1], &node2, node_cmp);
	list_erase_node(graph->neighbors[node1], ll_node);
	ll_node = list_find_node(graph->neighbors[node2], &node1, node_cmp);
	list_erase_node(graph->neighbors[node2], ll_node);
}

int *bfs(graph_t *graph, uint16_t source, int max_dist) {
	int *dist = malloc(graph->size * sizeof(int));
	for (uint16_t node = 0; node < graph->size; node++)
		dist[node] = -1;
	queue_t *queue = init_queue(graph->size, sizeof(uint16_t));
	queue_push(queue, &source);
	dist[source] = 0;
	while (queue->size > 0) {
		uint16_t curr_node = *(uint16_t *)queue_front(queue);
		queue_pop(queue);
		ll_node_t *ll_node = graph->neighbors[curr_node]->head;
		for (size_t i = 0; i < graph->neighbors[curr_node]->size; i++) {
			uint16_t nxt_node = *(uint16_t *)ll_node->data;
			if (dist[nxt_node] == -1) {
				dist[nxt_node] = dist[curr_node] + 1;
				if (max_dist == -1 || dist[nxt_node] <= max_dist)
					queue_push(queue, &nxt_node);
			}
			ll_node = ll_node->nxt;
		}
	}
	free_queue(queue);
	return dist;
}

/**
 * Intersects two lists of nodes
 * By iterating through one of them and trying to find the element in the other
*/
static linked_list_t *intersect_lists(linked_list_t *list1,
									  linked_list_t *list2) {
	linked_list_t *intersection = init_list(list1->data_size,
										    list1->destructor);
	ll_node_t *ll_node = list1->head;
	for (size_t i = 0; i < list1->size; i++) {
		uint16_t node = *(uint16_t *)ll_node->data;
		if (list_find_node(list2, &node, node_cmp))
			list_insert_to_tail(intersection, &node);
		ll_node = ll_node->nxt;
	}
	return intersection;
}

/**
 * Using the Bron-Kerbosch algorithm to find the maximum clique
*/
linked_list_t *bron_kerbosch(graph_t *graph, linked_list_t *clique,
							 linked_list_t *possible, linked_list_t *used) {
	linked_list_t *max_clique = init_list(sizeof(uint16_t), NULL);
	if (possible->size == 0 && used->size == 0) {
		ll_node_t *ll_node = clique->head;
		while (ll_node) {
			uint16_t node = *(uint16_t *)ll_node->data;
			list_insert_to_tail(max_clique, &node);
			ll_node = ll_node->nxt;
		}
		return max_clique;
	}
	linked_list_t *new_clique, *new_possible, *new_used, *neighbors, *candidate;
	ll_node_t *ll_node = possible->head;
	while (ll_node) {
		uint16_t curr_node = *(uint16_t *)ll_node->data;
		neighbors = graph->neighbors[curr_node];

		new_clique = init_list(sizeof(uint16_t), NULL);
		ll_node_t *ll_node2 = clique->head;
		while (ll_node2) {
			uint16_t node = *(uint16_t *)ll_node2->data;
			list_insert_to_tail(new_clique, &node);
			ll_node2 = ll_node2->nxt;
		}
		list_insert_sorted(new_clique, &curr_node, node_cmp);
		new_possible = intersect_lists(possible, neighbors);
		new_used = intersect_lists(used, neighbors);

		candidate = bron_kerbosch(graph, new_clique, new_possible, new_used);
		if (candidate->size > max_clique->size) {
			free_list(max_clique);
			max_clique = candidate;
		} else {
			free_list(candidate);
		}
		free_list(new_clique);
		free_list(new_possible);
		free_list(new_used);
		ll_node2 = ll_node->nxt;
		list_erase_node(possible, ll_node);
		list_insert_sorted(used, &curr_node, node_cmp);
		ll_node = ll_node2;
	}
	return max_clique;
}

linked_list_t *max_clique(graph_t *graph, uint16_t source_node) {
	linked_list_t *init_clique = init_list(sizeof(uint16_t), NULL);
	linked_list_t *possible_nodes = init_list(sizeof(uint16_t), NULL);
	linked_list_t *used_nodes = init_list(sizeof(uint16_t), NULL);
	list_insert_sorted(init_clique, &source_node, node_cmp);
	ll_node_t *ll_node = graph->neighbors[source_node]->head;
	for (size_t i = 0; i < graph->neighbors[source_node]->size; i++) {
		uint16_t nxt_node = *(uint16_t *)ll_node->data;
		list_insert_to_tail(possible_nodes, &nxt_node);
		ll_node = ll_node->nxt;
	}
	linked_list_t *clique = bron_kerbosch(graph, init_clique, possible_nodes,
										  used_nodes);
	free_list(init_clique);
	free_list(possible_nodes);
	free_list(used_nodes);
	return clique;
}

void free_graph(graph_t *graph) {
	for (size_t i = 0; i < graph->size; i++)
		free_list(graph->neighbors[i]);
	free(graph->neighbors);
	free(graph);
}

int node_cmp(void *data1, void *data2) {
	uint16_t node1 = *(uint16_t *)data1;
	uint16_t node2 = *(uint16_t *)data2;
	return (int)(node1 - node2);
}
