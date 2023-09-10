#include "shell.h"

char line[3000];

void flush_line()
{
	for (int i = 0; i < 3000; i++) {
		line[i] = '\0';
	}
}

void halt()
{
	outw(0x604, 0x2000);
}

char* help_message = "Welcome to blashOS!\n\
Print \"help\" to see this message.\n\
\n\
halt, reboot - shutdown / reboot the system\n\
printmem 0x2A 10 - print 10 bytes at 0x2A\n\
memmap - show all mapped page regions\n\
help - show this message\n";

int verify_command(size_t len, char* command, char* input)
{
	return (!ft_strncmp(command, input, len));
}

void shell()
{
	// char *line;
	char buf;
	printf("%s\n", help_message);
	while (1) {
		write(1, "> ", 2);
		flush_line();
		int ret = read(0, line, 3000);

		if (verify_command(4, "halt", line))
			halt();
		else if (verify_command(6, "reboot", line))
			reboot();
		else if (verify_command(4, "help", line))
			printf("%s\n", help_message);
		else if (verify_command(6, "memmap", line))
			memory_map();
		else if (verify_command(8, "printmem", line))
			printmem(line);
		else	
			printf("command not found: %s\n", line);
	}
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

    void *addr = (void *)c.address;
    void *lim = addr + c.len;
    while (addr < lim) {
        printf("%p: ", addr);
        print_mem_line(addr, lim);
        addr += 16;
    }
}
