#ifndef TYPES_H
#define TYPES_H

# ifndef TEST
typedef unsigned int size_t;
#define NULL ((void *)0)
typedef unsigned long uintptr_t;

# else
// This is only for tests
#include <stddef.h>
#include <stddef.h>
#include <stdint.h>
#endif

typedef unsigned long long u64int;
typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

#endif
