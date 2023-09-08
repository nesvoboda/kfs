#ifndef TEST_H
# define TEST_H

#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(predicate) \
if (predicate == 1) { \
    printf("Passed!\n"); \
} \
else { \
    printf("Failed!\n"); \
    exit(-1); \
} \


#define ASSERT_EQ(expected, actual) \
if (expected == actual) { \
    printf("Passed!\n"); \
} \
else { \
    printf("Failed! Expected: %d, actual: %d, at line %d, file %s\n", expected, actual, __LINE__, __FILE__); \
    exit(-1); \
} \


#endif
