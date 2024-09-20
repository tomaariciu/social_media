#include <stdint.h>

/**
 * Initializes the user list
*/
void init_users(void);

/**
 * Find the user_id of a user by it's name
 *
 * @param name - The name of the user
 * @return the id of the user, of -1 if name is not found
*/
uint16_t get_user_id(char *name);

/**
 * Find the name of a user by it's id
 *
 * @param id - The id of a user
 * @return the name of a user, of NULL if not found
*/
char *get_user_name(uint16_t id);

/**
 * Compares two user_ids
 * @return < 0 if the first user_id is smaller, > 0 if it is larger,
 * = 0 if they are equal
*/
int user_cmp(void *data1, void *data2);

/**
 * Frees the user list
*/
void free_users(void);
