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
