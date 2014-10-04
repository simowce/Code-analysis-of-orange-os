ENTRYPOINT	= 0x30400

#Program, flags
ASM		= nasm
LD		= ld
CC		= gcc

#flag for kernel
ASMKFLAG	= -I include/ -f elf

#flag for boot
ASMBFLAG	= -I boot/include/

CFLAG		= -I  include/ -c -fno-builtin -m32
LDFLAG		= -s -Ttext $(ENTRYPOINT) -m elf_i386
OBJS		= kernel/kernel.o kernel/start.o lib/kliba.o lib/string.o
KERNELBOOT	= boot/boot.bin boot/loader.bin
KERNEL		= kernel.bin

TARGIT	= $(KERNELBOOT) $(OBJS) $(KERNEL)

all : $(TARGIT) start

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAG) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/pm.inc boot/include/load.inc \
			boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAG) -o $@ $<

kernel/kernel.o : kernel/kernel.asm
	$(ASM) $(ASMKFLAG) -o $@ $<

$(KERNEL) : $(OBJS)
	$(LD) $(LDFLAG) -o $(KERNEL) $(OBJS)

kernel/start.o : kernel/start.c
	$(CC) $(CFLAG) -o $@ $<

lib/kliba.o : lib/kliba.asm
	$(ASM) $(ASMKFLAG) -o $@ $<

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAG) -o $@ $<
start:
	dd if=boot/boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp -fv boot/loader.bin /mnt/floppy
	sudo cp -fv kernel.bin /mnt/floppy
	sudo umount /mnt/floppy/
