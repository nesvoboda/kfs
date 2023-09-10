#include "printmem.h"
// printmem 0x... 500




printmem_command_t parse_printmem_input(char *line) {
	printmem_command_t res;
	
	char **fields = ft_split(line, ' ');

	int fields_len = 0;
	while (fields[fields_len]) {
		fields_len++;
	}
	if (fields_len != 3) {
		// print usage
		res.is_error = 1;
		return res;
	}
	if (ft_strncmp(fields[1], "0x", 2)) {
		res.is_error = 1;
		return res;
	}
	int first_operand = ft_atoi_hex(&(fields[1][2]));
	int second_operand = ft_atoi(fields[2]);
	res.address = first_operand;
	res.len = second_operand;
	res.is_error = 0;
	return res;
}

void print_mem_line(void *addr, void *lim) {
    void *line_lim = addr + 16;
    while (addr < lim && addr < line_lim) {
        printf(" %010p", *(u32int *)addr);
        addr += 4;
    }
    printf("\n");
}

void printmem(char *line) {
    printmem_command_t c = parse_printmem_input(line);

    if (c.is_error == 1) {
        printf("Usage: printmem 0x2A 500\n");
        return;
    }

    void *addr = c.address;
    void *lim = addr + c.len;
    while (addr < lim) {
        printf("%p: ", addr);
        print_mem_line(addr, lim);
        addr += 16;
    }
}
