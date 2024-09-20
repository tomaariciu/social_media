#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linked_list.h"

linked_list_t *init_list(unsigned int data_size, void (*destructor)(void *)) {
	linked_list_t *list = malloc(sizeof(linked_list_t));
	list->head = NULL;
	list->tail = NULL;
	list->data_size = data_size;
	list->size = 0;
	list->destructor = destructor;
	return list;
}

void list_insert_to_tail(linked_list_t *list, void *data) {
	ll_node_t *new_node = malloc(sizeof(ll_node_t));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, data, list->data_size);
	new_node->nxt = NULL;
	list->size++;
	if (!list->tail) {
		list->head = new_node;
		list->tail = new_node;
		return;
	}
	list->tail->nxt = new_node;
	list->tail = new_node;
}

void list_insert_to_head(linked_list_t *list, void *data) {
	ll_node_t *new_node = malloc(sizeof(ll_node_t));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, data, list->data_size);
	new_node->nxt = NULL;
	list->size++;
	if (!list->head) {
		list->head = new_node;
		list->tail = new_node;
		return;
	}
	new_node->nxt = list->head;
	list->head = new_node;
}

void list_insert_sorted(linked_list_t *list, void *data,
						int (*cmp_function)(void*, void*)) {
	if (list_find_node(list, data, cmp_function))
		return;
	ll_node_t *new_node = malloc(sizeof(ll_node_t));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, data, list->data_size);
	list->size++;
	if (!list->head || cmp_function(data, list->head->data) < 0) {
		new_node->nxt = list->head;
		list->head = new_node;
		if (!list->tail)
			list->tail = new_node;
		return;
	}
	ll_node_t *curr_node = list->head, *nxt_node = list->head->nxt;
	while (nxt_node && cmp_function(data, nxt_node->data) > 0) {
		curr_node = nxt_node;
		nxt_node = nxt_node->nxt;
	}
	if (curr_node == list->tail)
		list->tail = new_node;
	curr_node->nxt = new_node;
	new_node->nxt = nxt_node;
}

ll_node_t *list_find_node(linked_list_t *list, void *data,
						  int (*cmp_function)(void *, void *)) {
	ll_node_t *node = list->head;
	while (node && cmp_function(data, node->data) != 0)
		node = node->nxt;
	return node;
}

void list_erase_node(linked_list_t *list, ll_node_t *node) {
	if (!node)
		return;
	list->size--;
	if (list->head == node) {
		list->head = list->head->nxt;
		if (list->tail == node)
			list->tail = NULL;
		if (list->destructor)
			list->destructor(node->data);
		free(node->data);
		free(node);
		return;
	}
	ll_node_t *prv_node = list->head;
	while (prv_node->nxt != node)
		prv_node = prv_node->nxt;
	if (node == list->tail)
		list->tail = prv_node;
	prv_node->nxt = node->nxt;
	if (list->destructor)
		list->destructor(node->data);
	free(node->data);
	free(node);
}

void free_list(linked_list_t *list) {
	while (list->size > 0) {
		ll_node_t *ll_node = list->head;
		list->head = list->head->nxt;
		if (list->destructor)
			list->destructor(ll_node->data);
		free(ll_node->data);
		free(ll_node);
		list->size--;
	}
	free(list);
}
