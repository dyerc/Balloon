C_SRC=$(wildcard *.c) $(wildcard **/*.c) $(wildcard **/**/*.c) $(wildcard **/**/**/*.c)
C_OBJ=$(patsubst %.c, %.o, $(C_SRC))
ASM_SRC=$(wildcard *.s) $(wildcard **/*.s) $(wildcard **/**/*.s) $(wildcard **/**/**/*.s)
ASM_OBJ=$(patsubst %.s, %.o, $(ASM_SRC))

KERNEL_FILE=../build/kernel
ISO_FILE=../build/balloon.iso
FS_ROOT=../root

CC=/usr/local/cross/bin/i586-elf-gcc
LD=/usr/local/cross/bin/i586-elf-ld

CFLAGS=-nostdlib -fno-builtin -I./include -g
LDFLAGS=-Tlinker.ld
NASMFLAGS=-felf

all: $(C_OBJ) $(ASM_OBJ) link clean iso

qemu:
	@qemu-system-i386 -cdrom $(ISO_FILE) -m 32

iso:
	@cp $(KERNEL_FILE) $(FS_ROOT)/boot/kernel
	@cp ../build/ramd $(FS_ROOT)/boot/ramdisk
	@mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o $(ISO_FILE) $(FS_ROOT)
	@echo Finished generating iso image

clean:
	@echo Cleaning...
	@rm $(C_OBJ) $(ASM_OBJ)

link:
	@echo Linking
	@$(LD) $(LDFLAGS) -o $(KERNEL_FILE) $(ASM_OBJ) $(C_OBJ)

ramdisk:
	@cd ../build/ramdisk
	../../scripts/ramdisk/generate

.s.o:
	@echo Assembling $<
	@nasm $(NASMFLAGS) $<

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o $@ -c $<
