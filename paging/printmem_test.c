#include "test.h"
#include "printmem.h"

void printmem_tests() {
    // Not enough fields
    printmem_command_t res1 = parse_printmem_input("printmem something");
    ASSERT_EQ(1, res1.is_error);

    // wrong format of first operand
    printmem_command_t res2 = parse_printmem_input("printmem something wrong");
    ASSERT_EQ(1, res2.is_error);

    // correct command
    printmem_command_t res3 = parse_printmem_input("printmem 0x2A 43");
    ASSERT_EQ(0, res3.is_error);
    ASSERT_EQ(42, res3.address);
    ASSERT_EQ(43, res3.len);

}
