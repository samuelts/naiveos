#!Makefile

MAKE = mingw32-make

CC = i386-elf-gcc
LD = i386-elf-ld
ASM = nasm

QEMU = qemu-system-i386
RM = del

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include 
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

kernel.bin: entry.o boot.o common.o console.o
	$(LD) $(LD_FLAGS)  $^  -o $@

entry.o: init/entry.c include/types.h
	$(CC) $(C_FLAGS)  $<  -o $@ 

boot.o: boot/boot.asm
	$(ASM) $(ASM_FLAGS)  $<  -o $@

common.o: libs/common.c include/common.h include/types.h
	$(CC) $(C_FLAGS)  $<  -o $@ 

console.o: drivers/console.c include/console.h include/common.h
	$(CC) $(C_FLAGS)  $<  -o $@ 

.PHONY:clean
clean:
	$(RM) *.o kernel.bin

.PHONY:qemu
qemu: kernel.bin
	$(MAKE) $<
	$(QEMU) -kernel $<
	$(RM) trace-*