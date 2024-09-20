#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

#include "linked_list.h"
#include "users.h"

/**
 * Function that initializes all the data structures needed
 * In this case, the friend graph: the nodes are the users
 * And the edges represent the friendships between them
*/
void init_friends(void);

/**
 * Function that returns the friend list of a user
 * Needed for other tasks
*/
linked_list_t *get_friends(uint16_t user);

/**
 * Returns a list containing the group of friends of maximium size
 * The group must contain only users that are friends with one another
 * And the given user
*/
linked_list_t *find_max_friend_group(uint16_t user);

/**
 * Function that handles the calling of every command from task 1
*/
void handle_input_friends(char *input);

/**
 * Function that frees all the memory used
 * It frees the adjacency lists and the graph itself
*/
void free_friends(void);

#endif // FRIENDS_H
