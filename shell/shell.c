#include "shell.h"


char line[100];

void flush_line() {
    for (int i = 0; i < 100; i++)
    {
        line[i] = '\0';
    }
}

void halt()
{
    outw(0x604, 0x2000);
}

void shell()
{
    // char *line;
    char buf;
    while(1)
    {
        write(1, "> ", 2);
        flush_line();
        int ret = read(0, line, 99);
        
        if (!ft_strncmp(line, "kek", 3))
            write(1, "kek\n", 4);
        else if (!ft_strncmp(line, "halt", 4))
            halt();
        else if (!ft_strncmp(line, "reboot", 6))
            reboot();
        else if (!ft_strncmp(line, "backtrace", 9))
            backtrace(15);
        else
            write(1, "Unknown command\n", 16);
    }

}