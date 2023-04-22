#ifndef TIMER_H
#define TIMER_H
#include "idt.h"
#include "types.h"

void init_timer(u32int frequency);
void sleep(u32int time);
u32int gettick();
#endif