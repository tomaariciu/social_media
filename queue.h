#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_t queue_t;
struct queue_t {
	unsigned int max_size;
	unsigned int size;
	unsigned int data_size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
};

/**
 * Creates an empty queue
 * @param max_size - The maximum number of elements that can be stored
 * in the queue at the same time
 * @param data_size - The size of an element
*/
queue_t *init_queue(unsigned int max_size, unsigned int data_size);

/**
 * Adds an element to the end of a queue
 * @param queue
 * @param data
*/
void queue_push(queue_t *queue, void *data);

/**
 * @param queue
 * @return - Pointer to the first element in the queue
*/
void *queue_front(queue_t *queue);

/**
 * Removes the first element of a queue
 * @param queue
*/
void queue_pop(queue_t *queue);

/**
 * Frees the memory occupied by a queue
 * @param queue
*/
void free_queue(queue_t *queue);

#endif // QUEUE_H
