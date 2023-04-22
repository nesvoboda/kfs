#ifndef LOGS_H
# define LOGS_H

# include "idt.h"
# include "timer.h"

# define LOG_LINES 10

# define TICK_LENGTH (10 + 1) // 10 digits + sep
# define LEVEL_LENGTH (4 + 1) // how long is the level indicator i.e. INFO + sep 
# define LOG_LINE_LENGTH 64

enum log_level {
    EMPTY = 0,
    INFO = 1,
    ERROR = 2,
};

struct log
{
    u32int tick;
    enum log_level log_level;
    char line[LOG_LINE_LENGTH];
};

struct log *get_logs();

void start_log(enum log_level level);
void write_log(char c);
void end_log();
void init_logs();

#endif