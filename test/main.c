#include "test.h"

void bitset_tests();
void page_tests();
void oarray_tests();
void heap_tests();
void utils_tests();
void printmem_tests();
void physical_heap_tests();

const char *last_file_name = NULL;
const char *last_test_name = NULL;

void file_header(const char *filename) {
    if (filename != last_file_name) {
        last_file_name = filename;
        printf("\n\n--- [%s] ---", filename);
    }
}

void func_header(const char *function) {
    if (function != last_test_name) {
        last_test_name = function;
        printf("\n[%s] ", function);
    }
}

void all_headers(const char *filename, const char *function) {
	file_header(filename);
	func_header(function);
}

int main()
{
	bitset_tests();
	page_tests();
	oarray_tests();
	heap_tests();
	utils_tests();
	printmem_tests();
	physical_heap_tests();
	printf("\n");
}
