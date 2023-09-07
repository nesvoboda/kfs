#include "bitset.h"

t_bitset bitset_create(int max_size)
{
	t_bitset ret;
	int ints_to_go = max_size / 32 + 1;
	ret.data = (u32int*)(sizeof(u32int) * ints_to_go);
	for (int i = 0; i < ints_to_go; i++) {
		ret.data[i] = 0;
	}
	ret.size = max_size;
	return ret;
}

int bitset_test(t_bitset bitset, int index)
{
	int full_integers = index / 32;
	int offset_into_integer = index % 32;
	int integer = bitset.data[full_integers];
	return (integer & (1 << offset_into_integer)) != 0;
}

void bitset_set(t_bitset bitset, int index)
{
	int full_integers = index / 32;
	int offset_into_integer = index % 32;
	bitset.data[full_integers] |= (1 << offset_into_integer);
}

void bitset_clear(t_bitset bitset, int index)
{
	int full_integers = index / 32;
	int offset_into_integer = index % 32;
	bitset.data[full_integers] &= ~(1 << offset_into_integer);
}

int bitset_first_set(t_bitset bitset)
{
	int full_integers = bitset.size / 32;

	for (int i = 0; i < full_integers; i++) {
		if (bitset.data[i] != 0) {
			for (int j = 0; j < 32; j++) {
				if (bitset.data[i] & (1 << j)) {
					return j;
				}
			}
		}
	}
	// All empty
	return -1;
}

int bitset_first_unset(t_bitset bitset)
{
	int full_integers = bitset.size / 32 + 1;

	for (int i = 0; i < full_integers; i++) {
		if (bitset.data[i] != 1) {
			for (int j = 0; j < 32; j++) {
				if (!(bitset.data[i] & (1 << j))) {
					return j;
				}
			}
		}
	}
	// All set
	return -1;
}
