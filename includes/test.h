#ifndef TEST_H
# define TEST_H

#include <stdio.h>
#include <stdlib.h>

void file_header(const char *filename);
void func_header(const char *function);
void all_headers(const char *filename, const char *function);

#define TEST_ASSERT(predicate) \
if (predicate == 1) { \
    all_headers(__FILE__, __FUNCTION__);\
    printf("\033[32;3m ✓\033[0m"); \
} \
else { \
    printf("[%s] Failed!\n", __FUNCTION__); \
    exit(-1); \
} \



#define ASSERT_EQ(expected, actual) \
if (expected == actual) { \
    all_headers(__FILE__, __FUNCTION__);\
    printf("\033[32;3m ✓\033[0m"); \
} \
else { \
    printf("\n\n[%s] Failed! Expected: %lld, actual: %lld, at line %d, file %s\n", __FUNCTION__, (long long)expected, (long long)actual, __LINE__, __FILE__); \
    exit(-1); \
} \

#define ASSERT_NEQ(expected, actual) \
if (expected == actual) { \
    printf("\n\n[%s] Failed! Expected %lld and %lld to not be equal, at line %d, file %s\n", __FUNCTION__, (long long)expected, (long long)actual, __LINE__, __FILE__); \
    exit(-1); \
} \
else { \
    all_headers(__FILE__, __FUNCTION__);\
    printf("\033[32;3m ✓\033[0m"); \
} \



#endif
