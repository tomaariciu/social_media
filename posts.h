#ifndef POSTS_H
#define POSTS_H

#include "linked_list.h"
#include "tree.h"
#include "users.h"

#define MAX_PEOPLE 550

typedef struct post_t post_t;
typedef struct profile_t profile_t;

struct post_t {
	uint16_t user_id;
	uint32_t post_id;
	char *title;
	tree_t *tree;
	linked_list_t *likes;
};

struct profile_t {
	linked_list_t *posts;
};

/**
 * Initializing all the data structures needed to keep the posts
 * In this case, a linked list
*/
void init_posts(void);

/**
 * Initializing all the data structures needed to keep the profiles
 * In this case, an array with a linked list for each user
*/
void init_profiles(void);

/**
 * Function that returns the list containing the original posts
 * Needed for other tasks
*/
linked_list_t *get_all_posts(void);

/**
 * Function that returns the profile of a given user
 * Needed for other tasks
*/
profile_t *get_profile(uint16_t user_id);

/**
 * Function that returns the post given the post id
 * Needed for other tasks
*/
post_t *get_post(uint32_t pos_id);

/**
 * Function that handles the calling of every command from task 2
*/
void handle_input_posts(char *input);

/**
 * Function that frees all the memory used for the posts
 * It frees the list containing the posts and all of the post objects
*/
void free_posts(void);

/**
 * Function that frees all the memory used for the profiles
 * Frees the linked lists and the array containing them
*/
void free_profiles(void);

#endif // POSTS_H
