/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void) {
	#ifdef TASK_1
	init_friends();
	#endif

	#ifdef TASK_2
	init_posts();
	init_profiles();
	#endif

	#ifdef TASK_3

	#endif
}

void end_tasks(void) {
	#ifdef TASK_1
	free_friends();
	#endif

	#ifdef TASK_2
	free_posts();
	free_profiles();
	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input);
		#endif

		#ifdef TASK_2
		handle_input_posts(input);
		#endif

		#ifdef TASK_3
		handle_input_feed(input);
		#endif
	}

	free_users();
	end_tasks();
	free(input);

	return 0;
}
