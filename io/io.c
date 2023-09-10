#include "io.h"

void write(int fd, const void* buf, int count)
{

	if (fd == KERNEL) {
		for (int i = 0; i < count; i++) {
			write_log(((char*)buf)[i]);
		}
		return;
	} else {
		for (int i = 0; i < count; i++) {
			screen_add_char(((char*)buf)[i], 15);
		}
		// We can only write() to logs now...
		return;
	}
}

#define IOBUF_SIZE 3000

char io_buffer[IOBUF_SIZE];
char* dest_buf = NULL;

int read_finished = 0;
int bytes_read = 0;
int desired_count = 0;

void io_add_char(char c, u8int color)
{
	if (c == '\n' || c == '\0') {
		read_finished = 1;
		screen_add_char('\n', color);
	} else {
		int ret = screen_add_char(c, color);
		if (ret != 0) {
			return;
		}
		io_buffer[bytes_read] = c;
		bytes_read += 1;
		if (bytes_read >= desired_count) {
			read_finished = 1;
		}
		}
}

void io_erase()
{
	if (bytes_read > 0) {
		bytes_read -= 1;
		screen_erase();
	}
}

void init_buffer()
{
	for (int i = 0; i < IOBUF_SIZE; i++) {
		io_buffer[i] = '\0';
	}
}

int read(int fd, void* buf, int count)
{
	set_leftscroll_cutoff();
	init_buffer();
	bytes_read = 0;
	read_finished = 0;
	desired_count = count;
	dest_buf = buf;
	enable_keyboard();
	enable_cursor(0, 15);
	while (read_finished == 0) {
		asm("hlt");
	}
	disable_keyboard();
	disable_cursor();
	for (int i = 0; i < bytes_read; i++) {
		((char*)buf)[i] = io_buffer[i];
	}
	return bytes_read;
}
