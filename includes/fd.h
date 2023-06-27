#ifndef FD_H
#define FD_H

typedef enum DESCRIPTORS {
	KERNEL = -42,
	STDOUT = 1,
	STDIN = 0
} fds;

#endif