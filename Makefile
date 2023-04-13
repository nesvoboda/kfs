
src_files = gdt/gdt.c idt/idt.c screen/screen.c utils/utils.c kernel.c

CFLAGS = -target i386-none-elf -ffreestanding -fno-builtin -nostdlib -I includes

OBJS = $(src_files:.c=.o)

all: iso
# Note: PHONY is important here. Without it, implicit rules will try to build the executable "all", since the prereqs are ".o" files.
.PHONY: all 

%.o : %.c
	clang -c $(CFLAGS) $< -o $@

boot.o: boot.asm
	nasm -felf32 boot.asm -o boot.o

myos.bin: $(OBJS) boot.o
	clang -T linker.ld $(CFLAGS) -o myos.bin $(OBJS) boot.o

isobuilder:
	docker build . --tag isobuilder

iso: isobuilder myos.bin
	docker run -it -v${PWD}:/kfs isobuilder /kfs/make_iso.sh

clean: 
	rm -fv *.o */*.o
# add -fno-builtin
# • -fno-exception
# • -fno-stack-protector
# • -fno-rtti
# • -nostdlib
# • -nodefaultlibs