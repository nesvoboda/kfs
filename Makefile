
src_files = gdt/gdt.c idt/idt.c screen/screen.c utils/utils.c kernel.c \
idt/keyboard.c screen/screen_handler.c screen/text_handler.c screen/cursor.c \
printf/ft_printf.c printf/ft_printf_string.c printf/ft_printf_pointer.c printf/ft_printf_hex.c \
printf/ft_printf_integer.c printf/ft_printf_uint.c printf/ft_printf_char.c printf/lft_lite.c printf/lft_lite_fd.c \
printf/ft_printf_format.c logs/logs.c timer/timer.c io/io.c shell/shell.c shell/reboot.c print_stack/stack.c 

CFLAGS = -target i386-none-elf -nodefaultlibs  -fno-rtti -fno-stack-protector -fno-exceptions -ffreestanding -fno-builtin -nostdlib -I includes

OBJS = $(src_files:.c=.o)

all: myos.iso
# Note: PHONY is important here. Without it, implicit rules will try to build the executable "all", since the prereqs are ".o" files.
.PHONY: all isobuilder qemu fclean re

%.o : %.c
	clang -c $(CFLAGS) $< -o $@

boot.o: boot.asm
	nasm -felf32 boot.asm -o boot.o

myos.bin: $(OBJS) boot.o
	clang -T linker.ld $(CFLAGS) -o myos.bin $(OBJS) boot.o

isobuilder: Dockerfile
	docker build . --tag isobuilder

myos.iso: isobuilder myos.bin
	docker run -it -v${PWD}:/kfs isobuilder /kfs/make_iso.sh

clean: 
	rm -rfv *.o */*.o  myos.bin isodir

fclean: 
	clean
	rm myos.iso

re:
	fclean
	all

qemu: all
	qemu-system-i386 -cdrom myos.iso

gdb: all
	qemu-system-i386 -cdrom myos.iso -Ss
