#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "graph.h"

static graph_t *friend_graph;

void init_friends(void) {
	friend_graph = init_graph(MAX_PEOPLE);
}

linked_list_t *get_friends(uint16_t user) {
	return get_neighbors(friend_graph, user);
}

linked_list_t *find_max_friend_group(uint16_t user) {
	linked_list_t *friend_group = max_clique(friend_graph, user);
	return friend_group;
}

/**
 * Adding a connection between two users
 * Transforming their names in ids and adding the edge in the graph
*/
static void add_connection(char *friend1, char *friend2) {
	uint16_t friend1_id = get_user_id(friend1);
	uint16_t friend2_id = get_user_id(friend2);
	add_edge(friend_graph, friend1_id, friend2_id);
	printf("Added connection %s - %s\n", friend1, friend2);
}

/**
 * Removing a connection between two users
 * Transforming their names in ids and removing the edge in the graph
*/
static void remove_connection(char *friend1, char *friend2) {
	uint16_t friend1_id = get_user_id(friend1);
	uint16_t friend2_id = get_user_id(friend2);
	remove_edge(friend_graph, friend1_id, friend2_id);
	printf("Removed connection %s - %s\n", friend1, friend2);
}

/**
 * Getting the distance between two users
 * By doing a breath first search from one of them
 * And printing the distance computed for the other one
*/
static void compute_distance(char *user1, char *user2) {
	uint16_t user1_id = get_user_id(user1);
	uint16_t user2_id = get_user_id(user2);
	int *dist = bfs(friend_graph, user1_id, -1);
	if (dist[user2_id] != -1)
		printf("The distance between %s - %s is %d\n", user1, user2,
			   dist[user2_id]);
	else
		printf("There is no way to get from %s to %s\n", user1, user2);
	free(dist);
}

/**
 * Getting friendship suggestions for a given user
 * These are friends of other friends that aren't the given user or its friends
 * This means they are at distance 2 from the given user, so we use a bfs
*/
static void get_suggestions(char *user) {
	uint16_t user_id = get_user_id(user);
	int *dist = bfs(friend_graph, user_id, 2);
	int cnt = 0;
	for (uint16_t node = 0; node < MAX_PEOPLE; node++)
		if (dist[node] == 2)
			cnt++;
	if (cnt == 0) {
		printf("There are no suggestions for %s\n", user);
	} else {
		printf("Suggestions for %s:\n", user);
		for (uint16_t node = 0; node < MAX_PEOPLE; node++) {
			if (dist[node] == 2) {
				char *suggestion_name = get_user_name(node);
				printf("%s\n", suggestion_name);
			}
		}
	}
	free(dist);
}

/**
 * Finding all of the common friends of two given users
 * This means they are at distance 1 from both
 * So we are using bfs twice
*/
static void common_friends(char *user1, char *user2) {
	uint16_t user1_id = get_user_id(user1);
	uint16_t user2_id = get_user_id(user2);
	int *dist1 = bfs(friend_graph, user1_id, 1);
	int *dist2 = bfs(friend_graph, user2_id, 1);
	int cnt = 0;
	for (uint16_t node = 0; node < MAX_PEOPLE; node++)
		if (dist1[node] == 1 && dist2[node] == 1)
			cnt++;
	if (cnt == 0) {
		printf("No common friends for %s and %s\n", user1, user2);
	} else {
		printf("The common friends between %s and %s are:\n", user1, user2);
		for (uint16_t node = 0; node < MAX_PEOPLE; node++) {
			if (dist1[node] == 1 && dist2[node] == 1) {
				char *common_friend = get_user_name(node);
				printf("%s\n", common_friend);
			}
		}
	}

	free(dist1);
	free(dist2);
}

/**
 * Printing how many friends a given user has
*/
static void friend_count(char *user) {
	uint16_t user_id = get_user_id(user);
	unsigned int cnt = friend_graph->neighbors[user_id]->size;
	printf("%s has %u friends\n", user, cnt);
}

/**
 * Finding the friend of a given user that has the most friends
*/
static void most_popular_friend(char *user) {
	uint16_t user_id = get_user_id(user);
	uint16_t most_popular = user_id;
	unsigned int max_friends = friend_graph->neighbors[user_id]->size;
	ll_node_t *ll_node = friend_graph->neighbors[user_id]->head;
	for (size_t i = 0; i < friend_graph->neighbors[user_id]->size; i++) {
		uint16_t friend = *(uint16_t *)ll_node->data;
		unsigned int cnt = friend_graph->neighbors[friend]->size;
		if (cnt > max_friends) {
			most_popular = friend;
			max_friends = cnt;
		}
		ll_node = ll_node->nxt;
	}
	if (most_popular == user_id) {
		printf("%s is the most popular\n", user);
	} else {
		char *friend_name = get_user_name(most_popular);
		printf("%s is the most popular friend of %s\n", friend_name, user);
	}
}

void handle_input_friends(char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *friend1 = strtok(NULL, " ");
		char *friend2 = strtok(NULL, "\n ");
		add_connection(friend1, friend2);
	} else if (!strcmp(cmd, "remove")) {
		char *friend1 = strtok(NULL, " ");
		char *friend2 = strtok(NULL, "\n ");
		remove_connection(friend1, friend2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *user = strtok(NULL, "\n ");
		get_suggestions(user);
	} else if (!strcmp(cmd, "distance")) {
		char *friend1 = strtok(NULL, " ");
		char *friend2 = strtok(NULL, "\n ");
		compute_distance(friend1, friend2);
	} else if (!strcmp(cmd, "common")) {
		char *friend1 = strtok(NULL, " ");
		char *friend2 = strtok(NULL, "\n ");
		common_friends(friend1, friend2);
	} else if (!strcmp(cmd, "friends")) {
		char *user = strtok(NULL, "\n ");
		friend_count(user);
	} else if (!strcmp(cmd, "popular")) {
		char *user = strtok(NULL, "\n ");
		most_popular_friend(user);
	}

	free(commands);
}

void free_friends(void) {
	free_graph(friend_graph);
}
