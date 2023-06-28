#include "shell.h"

char line[100];

void flush_line()
{
	for (int i = 0; i < 100; i++) {
		line[i] = '\0';
	}
}

void halt()
{
	outw(0x604, 0x2000);
}

char* help_message = "Welcome to blashOS!\n\
Print \"help\" to see this message.\n\
Available commands:\n\
\n\
halt - shutdown the system\n\
reboot - reboot the system\n\
backtrace - show a stack trace with symbols\n\
help - show this message\n";

void shell()
{
	// char *line;
	char buf;
	printf("%s\n", help_message);
	while (1) {
		write(1, "> ", 2);
		flush_line();
		int ret = read(0, line, 3000);

		if (!ft_strncmp(line, "halt", 4))
			halt();
		else if (!ft_strncmp(line, "reboot", 6))
			reboot();
		else if (!ft_strncmp(line, "backtrace", 9))
			backtrace(15);
		else if (!ft_strncmp(line, "help", 4))
			printf("%s\n", help_message);
		else
			write(1, "Unknown command\n", 16);
	}
}