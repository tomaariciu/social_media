#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "utils.h"

queue_t *init_queue(unsigned int max_size, unsigned int data_size) {
	queue_t *queue = malloc(sizeof(queue_t));
	queue->max_size = max_size;
	queue->size = 0;
	queue->data_size = data_size;
	queue->read_idx = 0;
	queue->write_idx = 0;
	queue->buff = malloc(max_size * sizeof(void *));
	return queue;
}

void queue_push(queue_t *queue, void *data) {
	queue->buff[queue->write_idx] = malloc(queue->data_size);
	memcpy(queue->buff[queue->write_idx], data, queue->data_size);
	queue->write_idx++;
	if (queue->write_idx == queue->max_size)
		queue->write_idx = 0;
	queue->size++;
}

void *queue_front(queue_t *queue) {
	DIE(!queue->size, "Trying to get the first element of an empty queue!");
	void *data = queue->buff[queue->read_idx];
	return data;
}

void queue_pop(queue_t *queue) {
	DIE(!queue->size, "Trying to pop the first element of an empty queue!");
	free(queue->buff[queue->read_idx]);
	queue->read_idx++;
	if (queue->read_idx == queue->max_size)
		queue->read_idx = 0;
	queue->size--;
}

void free_queue(queue_t *queue) {
	while (queue->size > 0)
		queue_pop(queue);
	free(queue->buff);
	free(queue);
}

