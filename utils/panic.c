#include "panic.h"

void kpanic(int recoverable) {
    if (recoverable) {
        printk(ERROR, "Just had a recoverable kernel panic, be careful out there");
        return;
    }
    printf("\nIrrecoverable kernel panic! Kernel halted.");
    while (1) {
    }
}
