#include "bitset.h"

void bitset_create() {
    // TODO malloc storage for data
} 

int bitset_test(t_bitset bitset, int index) {
    int full_integers = index / 32;
    int offset_into_integer = index % 32;
    int integer = bitset.data[full_integers];
    return (integer & (1 << offset_into_integer)) != 0;
}

void bitset_set(t_bitset bitset, int index) {
    int full_integers = index / 32;
    int offset_into_integer = index % 32;
    bitset.data[full_integers] |= (1 << offset_into_integer);
}
