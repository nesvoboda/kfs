
#include "oarray.h"
#include "test.h"

void test_ordered_array_insert()
{
	oarray_t oa;
	oarray_create(&oa, 50, &lessthan);

	oarray_insert(&oa, (void *)7);
	oarray_insert(&oa, (void *)5);

	ASSERT_EQ((void *)5, oarray_retrieve(&oa, 0));
	ASSERT_EQ((void *)7, oarray_retrieve(&oa, 1));

	oarray_insert(&oa, (void *)6);

	ASSERT_EQ((void *)5, oarray_retrieve(&oa, 0));
	ASSERT_EQ((void *)6, oarray_retrieve(&oa, 1));
	ASSERT_EQ((void *)7, oarray_retrieve(&oa, 2));
	ASSERT_EQ(3, oa.len);
}

void test_ordered_array_remove()
{
	oarray_t oa;
	oarray_create(&oa, 50, &lessthan);

	oarray_insert(&oa, (void *)7);
	oarray_insert(&oa, (void *)5);
	oarray_insert(&oa, (void *)6);

	oarray_delete(&oa, 2);

	ASSERT_EQ((void *)5, oarray_retrieve(&oa, 0));
	ASSERT_EQ((void *)6, oarray_retrieve(&oa, 1));
	ASSERT_EQ(2, oa.len);

	oarray_insert(&oa, (void *)42);

	oarray_delete(&oa, 1);

	ASSERT_EQ((void *)5, oarray_retrieve(&oa, 0));
	ASSERT_EQ((void *)42, oarray_retrieve(&oa, 1));
	ASSERT_EQ(2, oa.len);
}

void oarray_tests()
{
	test_ordered_array_insert();
	test_ordered_array_remove();
}
