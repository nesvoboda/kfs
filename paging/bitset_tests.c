#include "bitset.h"
#include "test.h"

void test_bitset_test() {
    u32int test_data[2] = {0,2}; //0000000000000000000000000000000000000000000000000000000000000010
    t_bitset bitset;
    bitset.data = test_data;
    ASSERT(bitset_test(bitset, 33) == 1);
}

void test_bitset_set() {
    u32int test_data[2] = {0,0};
    t_bitset bitset;

    ASSERT(bitset_test(bitset, 4) == 0);
    bitset_set(bitset, 4);
    ASSERT(bitset_test(bitset, 4) == 1);
}

int main() {
    test_bitset_test();
    test_bitset_set();
}