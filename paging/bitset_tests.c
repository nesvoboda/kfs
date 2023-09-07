#include "bitset.h"
#include "test.h"

#include <malloc.h>

void test_bitset_create_test()
{
	t_bitset test = bitset_create(65);

	ASSERT_EQ(65, test.size);
	ASSERT(11 < malloc_usable_size(test.data));
	// 3 ints, all zeroes
	for (int i = 0; i < 3; i++) {
		ASSERT_EQ(0, test.data[i]);
	}
}

void test_bitset_test()
{
	u32int test_data[2] = { 0, 2 }; // 0000000000000000000000000000000000000000000000000000000000000010
	t_bitset bitset;
	bitset.data = test_data;
	ASSERT_EQ(bitset_test(bitset, 33), 1);
}

void test_bitset_set()
{
	u32int test_data[2] = { 0, 0 };
	t_bitset bitset;

	ASSERT_EQ(bitset_test(bitset, 4), 0);
	bitset_set(bitset, 4);
	ASSERT_EQ(bitset_test(bitset, 4), 1);
}

void test_bitset_clear()
{
	u32int test_data[2] = { 0, 0 };
	t_bitset bitset;
	bitset.data = test_data;

	bitset_set(bitset, 4);
	ASSERT_EQ(bitset_test(bitset, 4), 1);
	bitset_clear(bitset, 4);
	ASSERT_EQ(bitset_test(bitset, 4), 0);
}

void test_bitset_first_set()
{
	u32int test_data[2] = { 0, 0 };
	t_bitset bitset;
	bitset.data = test_data;
	bitset.size = 64;

	bitset_set(bitset, 4);
	bitset_set(bitset, 5);

	ASSERT_EQ(bitset_first_set(bitset), 4);
	bitset_clear(bitset, 4);
	ASSERT_EQ(5, bitset_first_set(bitset));
}

void test_bitset_first_unset()
{
	u32int test_data[2] = { 0, 0 };
	t_bitset bitset;
	bitset.data = test_data;
	bitset.size = 64;

	bitset_set(bitset, 0);
	bitset_set(bitset, 1);

	ASSERT_EQ(bitset_first_unset(bitset), 2);
	bitset_clear(bitset, 0);
	ASSERT_EQ(0, bitset_first_unset(bitset));
}

void bitset_tests()
{
	test_bitset_test();
	test_bitset_set();
	test_bitset_clear();
	test_bitset_first_set();
	test_bitset_create_test();
	test_bitset_first_unset();
}
