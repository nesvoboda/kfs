boot:
	nasm -felf32 boot.asm -o boot.o

kernel:
	clang -c kernel.c -target i386-none-elf -ffreestanding -fno-builtin -nostdlib

link:
	clang -T linker.ld -target i386-none-elf -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o

isobuilder:
	docker build . --tag isobuilder

iso:
	docker run -it -v$PWD:/kfs isobuilder /kfs/make_iso.sh
