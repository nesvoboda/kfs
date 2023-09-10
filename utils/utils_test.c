#include "test.h"
#include "utils.h"

void test_atoi_hex() {
    ASSERT_EQ(5, ft_atoi_hex("5"));

    ASSERT_EQ(10, ft_atoi_hex("A"));
    ASSERT_EQ(14593298, ft_atoi_hex("DEAD12"));
}

void utils_tests() {
    test_atoi_hex();
}
