boot: boot.asm
	nasm -felf32 boot.asm -o boot.o

gdt: gdt.c
	clang -c gdt.c -target i386-none-elf -ffreestanding -fno-builtin -nostdlib

idt: idt.c
	clang -c idt.c -target i386-none-elf -ffreestanding -fno-builtin -nostdlib

kernel: kernel.c
	clang -c kernel.c -target i386-none-elf -ffreestanding -fno-builtin -nostdlib

link: kernel boot gdt idt
	clang -T linker.ld -target i386-none-elf -o myos.bin -ffreestanding -O2 -nostdlib idt.o kernel.o gdt.o boot.o 

isobuilder:
	docker build . --tag isobuilder

iso: isobuilder link
	docker run -it -v${PWD}:/kfs isobuilder /kfs/make_iso.sh


all: iso

# add -fno-builtin
# • -fno-exception
# • -fno-stack-protector
# • -fno-rtti
# • -nostdlib
# • -nodefaultlibs