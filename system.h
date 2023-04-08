#ifndef SYSTEM_H
#define SYSTEM_H
void gdt_set_gate(int num, unsigned long base, unsigned long limit,
                           unsigned char access, unsigned char gran);
void gdt_install();
#endif