#ifndef PRINTMEM_H
# define PRINTMEM_H

# include "types.h"
# include "utils.h"

typedef struct printmem_command_s {
	u32int address;
	u32int len;
	int is_error;
} printmem_command_t;

printmem_command_t parse_printmem_input(char *line);

void printmem(char *line);

#endif 
