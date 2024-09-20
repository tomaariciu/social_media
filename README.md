# Part 1 - Friend network
* Friendships are represented as a graph, that supports adding and removing friends.
* Implemented multiple functions, such as friend suggestions for a given user, common friends between two users and the distance between two users.

# Part 2 - Posts and reposts
* Users have the ability to create a post or repost an existing post. They also can remove anything they created.
* Every post is characterised by its title, the user that created the post, a list of users who liked it and the tree of its reposts.
* Users have the ability to like a post or to remove their like.
* Implemented a ratio function, that detects if there is a repost with more likes than the original post
* Implemented a common repost function, that identifies the last post/repost that two reposts have in common, using binary lifting :)
* All posts or reposts created by a user are kept in their profile.

# Part 3 - Social Media
* Each user has his/her own feed, that has the most recent posts/reposts created by them or their friends.
* Added a friends repost function, that prints the list of all the friends that reposted a given post.
* Implemented a common group function, that finds the largest group of friends that contains a given user. For this, I used the Bron–Kerbosch algorithm for finding the largest clique.