
#include "test.h"
#include "oarray.h"

void test_ordered_array_insert() {
    ordered_array_t oa;
    oarray_create(&oa, 50, &lessthan);

    oarray_insert(&oa, 7);
    oarray_insert(&oa, 5);

    ASSERT_EQ(5, oarray_retrieve(&oa, 0));
    ASSERT_EQ(7, oarray_retrieve(&oa, 1));

    oarray_insert(&oa, 6);

    ASSERT_EQ(5, oarray_retrieve(&oa, 0));
    ASSERT_EQ(6, oarray_retrieve(&oa, 1));
    ASSERT_EQ(7, oarray_retrieve(&oa, 2));
    ASSERT_EQ(3, oa.len);

}

void test_ordered_array_remove() {
    ordered_array_t oa;
    oarray_create(&oa, 50, &lessthan);

    oarray_insert(&oa, 7);
    oarray_insert(&oa, 5);
    oarray_insert(&oa, 6);

    oarray_delete(&oa, 2);

    ASSERT_EQ(5, oarray_retrieve(&oa, 0));
    ASSERT_EQ(6, oarray_retrieve(&oa, 1));
    ASSERT_EQ(2, oa.len);

    oarray_insert(&oa, 42);

    oarray_delete(&oa, 1);

    ASSERT_EQ(5, oarray_retrieve(&oa, 0));
    ASSERT_EQ(42, oarray_retrieve(&oa, 1));
    ASSERT_EQ(2, oa.len);
}

void oarray_tests() {
    test_ordered_array_insert();
    test_ordered_array_remove();
}