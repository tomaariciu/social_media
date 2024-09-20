#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "posts.h"

static linked_list_t *all_posts;
static profile_t **profiles;
static uint32_t posts_number;

/**
 * Checking if a given post has a given id
 * Used to find the post in the list
*/
static int check_post(void *data1, void *data2) {
	uint32_t id = *(uint32_t *)data1;
	post_t *post = *(post_t **)data2;
	return (post->post_id - id);
}

/**
 * Printing the title of a given post
 * Or its id if it's a repost
*/
static void print_post(void *data) {
	post_t *post = *(post_t **)data;
	char *user = get_user_name(post->user_id);
	if (post->tree)
		printf("%s - Post by %s\n", post->title, user);
	else
		printf("Repost #%d by %s\n", post->post_id, user);
}

/**
 * Comparing two posts by their number of likes
 * Returns > 0  if post1 has more likes, < 0 if post2 has more likes
 * or 0 if they have the same number of likes
*/
static int cmp_likes(void *data1, void *data2) {
	post_t *post1 = *(post_t **)data1;
	post_t *post2 = *(post_t **)data2;
	return post1->likes->size - post2->likes->size;
}

/**
 * Frees all the memory ocuppied by a repost
*/
static void free_repost(void *data) {
	post_t *repost = *(post_t **)data;
	if (repost->tree)
		return;
	free_list(repost->likes);
	free(repost);
}

/**
 * Frees all the memory ocuppied by an original post
*/
static void free_single_post(void *data) {
	post_t *post = *(post_t **)data;
	free_tree(post->tree);
	free(post->title);
	free_list(post->likes);
	free(post);
}

void init_posts(void) {
	all_posts = init_list(sizeof(post_t *), free_single_post);
	posts_number = 0;
}

void init_profiles(void) {
	profiles = malloc(MAX_PEOPLE * sizeof(profile_t *));
	for (size_t i = 0; i < MAX_PEOPLE; i++) {
		profiles[i] = malloc(sizeof(profile_t));
		profiles[i]->posts = init_list(sizeof(post_t *), NULL);
	}
}

linked_list_t *get_all_posts(void) {
	return all_posts;
}

profile_t *get_profile(uint16_t user_id) {
	return profiles[user_id];
}

post_t *get_post(uint32_t post_id) {
	ll_node_t *node = list_find_node(all_posts, &post_id, check_post);
	post_t *post = *(post_t **)node->data;
	return post;
}

/**
 * Creates a post given its title and the user thats making it
 * Allocates its corresponding repost tree
 * And adding it to all the lists
*/
static void create_post(char *user, char *title) {
	uint16_t user_id = get_user_id(user);
	posts_number++;
	post_t *post = malloc(sizeof(post_t));
	post->user_id = user_id;
	post->post_id = posts_number;
	post->title = malloc((strlen(title) + 1) * sizeof(char));
	strcpy(post->title, title);
	post->tree = init_tree(sizeof(post_t *), free_repost);
	post->likes = init_list(sizeof(uint16_t), NULL);
	add_root(post->tree, &post);
	list_insert_to_head(all_posts, &post);
	list_insert_to_tail(profiles[user_id]->posts, &post);
	printf("Created %s for %s\n", title, user);
}

/**
 * Creates a repost given the user thats making it, the original post
 * And (optionally) the repost father
 * Inserts it in the corresponding tree and adds it to the profile list
*/
static void create_repost(char *user, char *post_string, char *repost_string) {
	uint16_t user_id = get_user_id(user);
	uint32_t root_id = atoi(post_string);
	uint32_t parent_id = root_id;
	if (repost_string)
		parent_id = atoi(repost_string);
	post_t *root = get_post(root_id);
	posts_number++;
	post_t *repost = malloc(sizeof(post_t));
	repost->user_id = user_id;
	repost->post_id = posts_number;
	repost->title = root->title;
	repost->tree = NULL;
	repost->likes = init_list(sizeof(uint16_t), NULL);
	add_node(root->tree, root->tree->root, &repost, &parent_id, check_post);
	list_insert_to_tail(profiles[user_id]->posts, &repost);
	printf("Created repost #%d for %s\n", posts_number, user);
}

/**
 * Gets the first common repost of two given reposts and their original post
 * Finds the corresponding tree nodes and computes their LCA
*/
static void get_common_repost(char *post_string, char *repost1_string,
							  char *repost2_string) {
	uint32_t root_id = atoi(post_string);
	uint32_t post1_id = atoi(repost1_string);
	uint32_t post2_id = atoi(repost2_string);
	post_t *root = get_post(root_id);
	tree_node_t *tree_node1 = tree_find_node(root->tree, root->tree->root,
											 &post1_id, check_post);
	tree_node_t *tree_node2 = tree_find_node(root->tree, root->tree->root,
											 &post2_id, check_post);
	tree_node_t *lca = compute_lca(tree_node1, tree_node2);
	post_t *post_lca = *(post_t **)lca->data;
	printf("The first common repost of %d and %d is %d\n",
		   post1_id, post2_id, post_lca->post_id);
}

/**
 * Adds a user's like to a post
 * If the user already liked that post, then it will be considered a dislike
*/
static void like_post(char *user, char *post_string, char *repost_string) {
	uint16_t user_id = get_user_id(user);
	uint32_t root_id = atoi(post_string);
	uint32_t post_id = root_id;
	if (repost_string)
		post_id = atoi(repost_string);
	post_t *root = get_post(root_id);
	tree_node_t *tree_node = tree_find_node(root->tree, root->tree->root,
											&post_id, check_post);
	post_t *post = *(post_t **)tree_node->data;
	ll_node_t *like_node = list_find_node(post->likes, &user_id, user_cmp);
	if (!like_node) {
		list_insert_sorted(post->likes, &user_id, user_cmp);
		printf("User %s liked ", user);
	} else {
		list_erase_node(post->likes, like_node);
		printf("User %s unliked ", user);
	}
	if (post_id == root_id)
		printf("post %s\n", post->title);
	else
		printf("repost %s\n", root->title);
}

/**
 * Finds if a post was ratio'd by one of its reposts
 * A ratio is a repost that has more likes than the original post
*/
static void find_ratio(char *post_string) {
	uint32_t root_id = atoi(post_string);
	post_t *root = get_post(root_id);
	tree_node_t *max = find_max(root->tree, root->tree->root, cmp_likes);
	post_t *most_liked_post = *(post_t **)max->data;
	if (most_liked_post == root)
		printf("The original post is the highest rated\n");
	else
		printf("Post %d got ratio'd by repost %d\n", root_id,
			   most_liked_post->post_id);
}

/**
 * Deletes a post and all of the reposts originating from it
*/
static void delete_post(char *post_string, char *repost_string) {
	uint32_t root_id = atoi(post_string);
	uint32_t post_id = root_id;
	if (repost_string) {
		post_id = atoi(repost_string);
		post_t *root = get_post(root_id);
		tree_node_t *tree_node = tree_find_node(root->tree, root->tree->root,
											&post_id, check_post);
		post_t *post = *(post_t **)tree_node->data;
		printf("Deleted repost #%d of post %s\n", post->post_id, root->title);
		ll_node_t *ll_node = list_find_node(tree_node->parent->children,
											&tree_node, check_node);
		list_erase_node(tree_node->parent->children, ll_node);
		delete_subtree(root->tree, tree_node);
	} else {
		ll_node_t *node = list_find_node(all_posts, &root_id, check_post);
		post_t *root = *(post_t **)node->data;
		printf("Deleted %s\n", root->title);
		list_erase_node(all_posts, node);
	}
}

/**
 * Prints a post's/repost's number of likes
*/
static void get_likes(char *post_string, char *repost_string) {
	uint32_t root_id = atoi(post_string);
	uint32_t post_id = root_id;
	if (repost_string)
		post_id = atoi(repost_string);
	post_t *root = get_post(root_id);
	tree_node_t *tree_node = tree_find_node(root->tree, root->tree->root,
											&post_id, check_post);
	post_t *post = *(post_t **)tree_node->data;
	if (post->tree)
		printf("Post %s has %d likes\n", post->title, post->likes->size);
	else
		printf("Repost #%d has %d likes\n", post->post_id, post->likes->size);
}

/**
 * Prints of all the reposts originating from the given repost (or post if
 * repost string is NULL)
*/
static void get_reposts(char *post_string, char *repost_string) {
	uint32_t root_id = atoi(post_string);
	uint32_t post_id = root_id;
	if (repost_string)
		post_id = atoi(repost_string);
	post_t *root = get_post(root_id);
	tree_node_t *subtree_root = tree_find_node(root->tree, root->tree->root,
											   &post_id, check_post);
	dfs(root->tree, subtree_root, print_post);
}

void handle_input_posts(char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *user = strtok(NULL, " ");
		char *title = strtok(NULL, "\n");
		create_post(user, title);
	} else if (!strcmp(cmd, "repost")) {
		char *user = strtok(NULL, " ");
		char *post_string = strtok(NULL, "\n ");
		char *repost_string = strtok(NULL, "\n");
		create_repost(user, post_string, repost_string);
	} else if (!strcmp(cmd, "common-repost")) {
		char *post_string = strtok(NULL, " ");
		char *repost1_string = strtok(NULL, " ");
		char *repost2_string = strtok(NULL, "\n");
		get_common_repost(post_string, repost1_string, repost2_string);
	} else if (!strcmp(cmd, "like")) {
		char *user = strtok(NULL, " ");
		char *post_string = strtok(NULL, "\n ");
		char *repost_string = strtok(NULL, "\n");
		like_post(user, post_string, repost_string);
	} else if (!strcmp(cmd, "ratio")) {
		char *post_string = strtok(NULL, "\n ");
		find_ratio(post_string);
	} else if (!strcmp(cmd, "delete")) {
		char *post_string = strtok(NULL, "\n ");
		char *repost_string = strtok(NULL, "\n ");
		delete_post(post_string, repost_string);
	} else if (!strcmp(cmd, "get-likes")) {
		char *post_string = strtok(NULL, "\n ");
		char *repost_string = strtok(NULL, "\n ");
		get_likes(post_string, repost_string);
	} else if (!strcmp(cmd, "get-reposts")) {
		char *post_id_string = strtok(NULL, "\n ");
		char *repost_id_string = strtok(NULL, "\n");
		get_reposts(post_id_string, repost_id_string);
	}

	free(commands);
}

void free_posts(void) {
	free_list(all_posts);
}

void free_profiles(void) {
	for (int i = 0; i < MAX_PEOPLE; i++) {
		free_list(profiles[i]->posts);
		free(profiles[i]);
	}
	free(profiles);
}
