#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct ll_node_t ll_node_t;
typedef struct linked_list_t linked_list_t;

struct ll_node_t {
	void *data;
	ll_node_t *nxt;
};

struct linked_list_t {
	ll_node_t *head;
	ll_node_t *tail;
	unsigned int data_size;
	unsigned int size;
	void (*destructor)(void *data);
};

/**
 * Creates an empty linked list
 * @param data_size - The size of the data that will be stored in each node
 * @param destructor - The function that frees the memory that
 * data is pointing to, NULL if there is nothing to free
*/
linked_list_t *init_list(unsigned int data_size, void (*destructor)(void *));

/**
 * Inserts a new node at the end of a list
 * @param list
 * @param data
*/
void list_insert_to_tail(linked_list_t *list, void *data);

/**
 * Inserts a new node at the beginning of a list
 * @param list
 * @param data
*/
void list_insert_to_head(linked_list_t *list, void *data);

/**
 * Inserts a new node before the first element larger than it
 * Nothing happens if the data is found before that
 * Works predictably only on sorted lists
 * @param list - The list containing the nodes
 * @param data - The data being inserted
 * @param cmp_function - Compares the data to the list data
*/
void list_insert_sorted(linked_list_t *list, void *data,
						int (*cmp_function)(void *, void *));

/**
 * @param list
 * @param data
 * @param cmp_function - Compares the data to the list data to check
 * if they are the same
 * @return - The node containing the data or NULL if it's not found
*/
ll_node_t *list_find_node(linked_list_t *list, void *data,
						  int (*cmp_function)(void *, void *));

/**
 * Erases the first element containing the data
 * Nothing happens if the data is not found
 * @param list
 * @param data
*/
void list_erase_node(linked_list_t *list, ll_node_t *node);

/**
 *  Frees the memory occupied by a linked list
*/
void free_list(linked_list_t *list);

#endif // LINKED_LIST_H
