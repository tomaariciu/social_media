#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "posts.h"
#include "friends.h"

/**
 * Checking if a post was made by a specific user
 * Used to check which friends reposted a given post
*/
static int check_post_user(void *data1, void *data2) {
	uint16_t user_id = *(uint16_t *)data1;
	post_t *post = *(post_t **)data2;
	return (post->user_id - user_id);
}

/**
 * Getting the most recent feed_size posts made by a user or its friends
 * Just iterating through the linked list containing the posts
 * And filtering the needed ones
*/
static void get_feed(char *user, char *feed_size_string) {
	uint16_t user_id = get_user_id(user);
	uint32_t feed_size = atoi(feed_size_string);
	linked_list_t *all_posts = get_all_posts();
	linked_list_t *friends = get_friends(user_id);
	ll_node_t *ll_node = all_posts->head;
	while (ll_node && feed_size) {
		post_t *post = *(post_t **)ll_node->data;
		if (post->user_id == user_id ||
			list_find_node(friends, &post->user_id, user_cmp)) {
			printf("%s: %s\n", get_user_name(post->user_id), post->title);
			feed_size--;
		}
		ll_node = ll_node->nxt;
	}
}

/**
 * Viewing all the posts/reposts made by a user
 * Kept a list with them for each user for a simpler implementation
*/
static void view_profile(char *user) {
	uint16_t user_id = get_user_id(user);
	profile_t *user_profile = get_profile(user_id);
	ll_node_t *ll_node = user_profile->posts->head;
	while (ll_node) {
		post_t *post = *(post_t **)ll_node->data;
		if (post->tree)
			printf("Posted: %s\n", post->title);
		else
			printf("Reposted: %s\n", post->title);
		ll_node = ll_node->nxt;
	}
}

/**
 * Printing all the friends of a user that posted a given post
 * Iterating through the list of friends first
 * And checking if they reposted anything
 * Doing this to ensure that they are printed in order of their IDs
*/
static void friends_repost(char *user, char *post_string) {
	uint16_t user_id = get_user_id(user);
	linked_list_t *friends = get_friends(user_id);
	uint32_t post_id = atoi(post_string);
	post_t *post = get_post(post_id);
	ll_node_t *node = friends->head;
	while (node) {
		uint16_t friend_id = *(uint16_t *)node->data;
		tree_node_t *repost = tree_find_node(post->tree, post->tree->root,
											 &friend_id, check_post_user);
		if (repost)
			printf("%s\n", get_user_name(friend_id));
		node = node->nxt;
	}
}

/**
 * Finding the maximum group of friends that are friends with one another
 * The group must contain a given user
 * This is the maximum clique in the friend graph that contains the user
*/
static void find_max_group(char *user) {
	uint16_t user_id = get_user_id(user);
	linked_list_t *group = find_max_friend_group(user_id);
	printf("The closest friend group of %s is:\n", user);
	ll_node_t *node = group->head;
	while (node) {
		uint16_t friend_id = *(uint16_t *)node->data;
		printf("%s\n", get_user_name(friend_id));
		node = node->nxt;
	}
	free_list(group);
}

void handle_input_feed(char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *user = strtok(NULL, "\n ");
		char *feed_size_string = strtok(NULL, "\n ");
		get_feed(user, feed_size_string);
	} else if (!strcmp(cmd, "view-profile")) {
		char *user = strtok(NULL, "\n ");
		view_profile(user);
	} else if (!strcmp(cmd, "friends-repost")) {
		char *user = strtok(NULL, "\n ");
		char *post_string = strtok(NULL, "\n ");
		friends_repost(user, post_string);
	} else if (!strcmp(cmd, "common-group")) {
		char *user = strtok(NULL, "\n ");
		find_max_group(user);
	}

	free(commands);
}
