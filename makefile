KERNEL_ENTRY = 0x30400
IMG = boot.img

ASM = nasm
DASM = ndisasm
CC = gcc

ASMBOOTFLAG = -I boot/include/ -f bin
ASMKERNELFLAG = -I include/ -f elf
CFLAG = -c -fno-builtin -I include/
LDFLAG = -s -Ttext $(KERNEL_ENTRY)

TARGET = boot/boot.bin boot/loader.bin kernel/kernel.bin
OBJ = kernel/kernel.o kernel/start.o lib/kliba.o lib/string.o kernel/global.o \
	kernel/i8259.o kernel/protect.o lib/klib.o kernel/main.o \
	kernel/proc.o kernel/sys_call.o kernel/i8253.o kernel/keyboard.o \
	kernel/tty.o kernel/clock.o kernel/syscall.o kernel/vga.o kernel/debug.o

vpath %.asm	boot:kernel:lib
vpath %.c	kernel:lib
vpath %.h	include
vpath %.inc	include:boot/include

.PHONY: all img clean

all:$(TARGET)

boot/boot.bin:boot.asm fat12hdr.inc load.inc
	$(ASM) $(ASMBOOTFLAG) -o $@ $<
	@echo boot.bin OK.

boot/loader.bin:loader.asm fat12hdr.inc load.inc lib.inc
	$(ASM) $(ASMBOOTFLAG) -o $@ $<
	@echo Loader OK.

kernel/kernel.bin:$(OBJ)
	$(LD) $(LDFLAG) -o $@ $(OBJ) 
	@echo Kernel OK.

kernel/kernel.o:kernel.asm sconst.inc 
	$(ASM) $(ASMKERNELFLAG) -o $@ $<

kernel/sys_call.o:sys_call.asm
	$(ASM) $(ASMKERNELFLAG) -o $@ $<

lib/kliba.o:kliba.asm sconst.inc
	$(ASM) $(ASMKERNELFLAG) -o $@ $<

lib/string.o:string.asm
	$(ASM) $(ASMKERNELFLAG) -o $@ $<

kernel/start.o:start.c protect.h type.h klib.h global.h
	$(CC) $(CFLAG) -o $@ $<

kernel/global.o:global.c global.h clock.h syscall.h
	$(CC) $(CFLAG) -o $@ $<

kernel/i8259.o:i8259.c i8259.h global.h klib.h
	$(CC) $(CFLAG) -o $@ $<

kernel/protect.o:protect.c type.h  i8259.h i8253.h protect.h klib.h global.h \
		syscall.h
	$(CC) $(CFLAG) -o $@ $<

lib/klib.o:klib.c type.h const.h klib.h
	$(CC) $(CFLAG) -o $@ $<
kernel/main.o:main.c klib.h global.h clock.h keyboard.h proc.h
	$(CC) $(CFLAG) -o $@ $<
kernel/proc.o:proc.c proc.h type.h klib.h global.h protect.h debug.h
	$(CC) $(CFLAG) -o $@ $<
kernel/i8253.o:i8253.c i8253.h klib.h
	$(CC) $(CFLAG) -o $@ $<
kernel/keyboard.o:keyboard.c keyboard.h klib.h i8259.h type.h keymap.h
	$(CC) $(CFLAG) -o $@ $<
kernel/tty.o:tty.c keyboard.h debug.h
	$(CC) $(CFLAG) -o $@ $<
kernel/clock.o:clock.c clock.h global.h i8259.h type.h proc.h
	$(CC) $(CFLAG) -o $@ $<
kernel/syscall.o:syscall.c syscall.h global.h klib.h
	$(CC) $(CFLAG) -o $@ $<
kernel/vga.o:vga.c vga.h global.h klib.h
	$(CC) $(CFLAG) -o $@ $<
kernel/debug.o:debug.c debug.h syscall.h
	$(CC) $(CFLAG) -o $@ $<

img:$(TARGET)
	dd if=boot/boot.bin of=$(IMG) bs=512 count=1 conv=notrunc
	mount -o loop $(IMG) /mnt/flopy
	cp ./boot/loader.bin ./kernel/kernel.bin /mnt/flopy
	umount /mnt/flopy
	@echo OS Image OK.
clean:
	rm -f $(TARGET) $(OBJ)

