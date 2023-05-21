#include "unistd.h"

void write(int fd, const void *buf, int count) {

	if (fd == -42) {
		for (int i = 0; i < count; i++) {
			write_log(((char *)buf)[i]);
		}
	}
	else {
        for (int i =0; i < count; i++) {
            screen_add_char(((char *)buf)[i], 15);
        }
		// We can only write() to logs now...
		return;
	}
}