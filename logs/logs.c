#include "logs.h"

// Kernel log buffer is a ring buffer of log lines
struct log _logs[LOG_LINES];

struct log *get_logs() {
    return _logs;
}

int current_log_line = 0;

void init_logs() {
    for (int i = 0; i < LOG_LINES; i++) {
        _logs[i].log_level = EMPTY;
        memset(_logs[i].line, '\0', LOG_LINE_LENGTH);
    }
}

// start_log starts a new log line and writes the moment it was entered
void start_log(enum log_level level) {
    _logs[current_log_line].log_level = level;
    _logs[current_log_line].tick = gettick();
}

int line_position = 0;

// write_log writes one character to the current log line
void write_log(char c) {
    if (line_position >= LOG_LINE_LENGTH) {
        return;
    }
    _logs[current_log_line].line[line_position] = c;
    line_position += 1;
}

// end_log closes the log line
void end_log() {

    current_log_line += 1;
    if (current_log_line % 9 == 0)
        current_log_line = 0;
    line_position = 0;
}