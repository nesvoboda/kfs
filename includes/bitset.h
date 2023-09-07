#ifndef BITSET_H
# define BITSET_H

#include "types.h"
#include "allocate.h"

// Bitset data structure
typedef struct s_bitset {
    u32int *data;
    u32int size;
} t_bitset;

t_bitset bitset_create();
int bitset_test(t_bitset bitset, int index);
void bitset_set(t_bitset bitset, int index);
void bitset_clear(t_bitset bitset, int index);
int bitset_first_set(t_bitset bitset);
int bitset_first_unset(t_bitset bitset);

#endif