#ifndef TEST_H
# define TEST_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(predicate) \
if (predicate == 1) { \
    printf("Passed!\n"); \
} \
else { \
    printf("Failed!\n"); \
    exit(-1); \
} \


#endif