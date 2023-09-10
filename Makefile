
src_files = gdt/gdt.c idt/idt.c screen/screen.c utils/utils.c kernel.c \
idt/keyboard.c screen/screen_handler.c screen/text_handler.c screen/cursor.c \
printf/ft_printf.c printf/ft_printf_string.c printf/ft_printf_pointer.c printf/ft_printf_hex.c \
printf/ft_printf_integer.c printf/ft_printf_uint.c printf/ft_printf_char.c printf/lft_lite.c printf/lft_lite_fd.c \
printf/ft_printf_format.c logs/logs.c timer/timer.c io/io.c shell/shell.c shell/reboot.c backtrace/backtrace.c \
elf/elf.c paging/paging.c paging/page.c paging/bitset.c paging/internal_allocate.c \
heap/heap.c heap/oarray.c paging/printmem.c utils/split.c heap/malloc.c paging/brk.c utils/panic.c

CFLAGS = -target i386-none-elf -nodefaultlibs  -fno-rtti -fno-stack-protector -fno-exceptions -ffreestanding -fno-builtin -nostdlib -g -I includes

OBJS = $(src_files:.c=.o)

all: myos.iso
# Note: PHONY is important here. Without it, implicit rules will try to build the executable "all", since the prereqs are ".o" files.
.PHONY: all isobuilder qemu fclean re test_debug test test_coverage

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
	rm -rfv *.o */*.o  myos.bin isodir test_binary test_debug

fclean: clean
	rm -rfv myos.iso

re: fclean all

qemu: all
	qemu-system-i386 -cdrom myos.iso

gdb: all
	qemu-system-i386 -cdrom myos.iso -S -s &
	gdb myos.bin --eval-command="target remote localhost:1234" ; killall qemu-system-i386

test_files = paging/bitset_tests.c paging/bitset.c test/main.c \
paging/page_tests.c utils/utils.c paging/page.c \
heap/oarray_tests.c heap/oarray.c heap/heap_tests.c heap/heap.c \
utils/utils_test.c paging/printmem_test.c paging/printmem.c \
utils/split.c printf/lft_lite.c
# test_objs = $(test_files:.c=.o)
test_flags = -g -I includes --define-macro TEST=true

coverage_flags = -fprofile-instr-generate -fcoverage-mapping

test:
	clang $(test_files) $(test_flags) -o test_binary 
	./test_binary

test_coverage:
	clang $(test_files) $(test_flags) $(coverage_flags) -o test_binary
	./test_binary
	xcrun llvm-profdata merge -sparse default.profraw -o default.profdata
	xcrun llvm-cov export ./test_binary -instr-profile=default.profdata --format lcov > lcov.info

test_debug:
	clang $(test_files) $(test_flags) -o test_debug

debug: all
	qemu-system-i386 -cdrom myos.iso -S -s
