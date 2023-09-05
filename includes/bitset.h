#ifndef BITSET_H
# define BITSET_H

#include "types.h"

// Bitset data structure
typedef struct s_bitset {
    u32int *data;
} t_bitset;

void bitset_create();
int bitset_test(t_bitset bitset, int index);
void bitset_set(t_bitset bitset, int index);

#endif