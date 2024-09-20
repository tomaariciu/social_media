#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdio.h>

#define DIE(assertion, call_description)								\
do {																	\
	if (assertion) {													\
		fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);				\
		perror(call_description);										\
		exit(errno);													\
	}																	\
} while (0)

#endif // UTILS_H
