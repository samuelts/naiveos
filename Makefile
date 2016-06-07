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

kernel.bin: kmain.o boot.o common.o console.o printk.o string.o debug.o gdt_c.o gdt_asm.o idt_c.o idt_asm.o
	$(LD) $(LD_FLAGS)  $^  -o $@

kmain.o: arch/i386/init/kmain.c
	$(CC) $(C_FLAGS) -Iarch/i386/include  $<  -o $@ 

gdt_c.o: arch/i386/mm/gdt.c
	$(CC) $(C_FLAGS) -Iarch/i386/include  $<  -o $@

gdt_asm.o: arch/i386/mm/gdt.asm
	$(ASM) $(ASM_FLAGS)  $<  -o $@

idt_c.o: arch/i386/mm/idt.c
	$(CC) $(C_FLAGS) -Iarch/i386/include  $<  -o $@

idt_asm.o: arch/i386/mm/idt.asm
	$(ASM) $(ASM_FLAGS)  $<  -o $@

printk.o: kernel/printk.c
	$(CC) $(C_FLAGS)  $<  -o $@

string.o: lib/string.c
	$(CC) $(C_FLAGS)  $<  -o $@

boot.o: arch/i386/init/boot.asm
	$(ASM) $(ASM_FLAGS)  $<  -o $@

common.o: lib/common.c include/common.h include/types.h
	$(CC) $(C_FLAGS)  $<  -o $@ 

console.o: arch/i386/driver/console.c include/console.h include/common.h
	$(CC) $(C_FLAGS)  $<  -o $@ 

debug.o: arch/i386/debug/debug.c
	$(CC) $(C_FLAGS)  $<  -o $@

.PHONY:clean
clean:
	$(RM) *.o kernel.bin

.PHONY:qemu
qemu: kernel.bin
	$(MAKE) $<
	$(QEMU) -kernel $<
	$(RM) trace-*