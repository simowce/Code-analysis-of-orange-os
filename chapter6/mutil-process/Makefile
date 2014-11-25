ENTRYPOINT	= 0x30400

#Program, flags
ASM		= nasm
LD		= ld
CC		= gcc

#flag for kernel
ASMKFLAG	= -I include/ -f elf

#flag for boot
ASMBFLAG	= -I boot/include/

CFLAG		=  -c -fno-builtin -m32 -I include/ -fno-stack-protector
LDFLAG		= -s -Ttext $(ENTRYPOINT) -m elf_i386 
OBJS		= kernel/kernel.o  lib/kliba.o lib/string.o kernel/protect.o lib/klib.o kernel/global.o \
		  kernel/i8259.o  kernel/main.o kernel/start.o kernel/clock.o
KERNELBOOT	= boot/boot.bin boot/loader.bin
KERNEL		= kernel.bin

TARGIT	= $(KERNELBOOT) $(OBJS) $(KERNEL)

all : $(TARGIT) start

clean : 
	rm -f $(OBJS) $(KERNELBOOT) $(KERNEL)

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAG) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/pm.inc boot/include/load.inc \
			boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAG) -o $@ $<

kernel/kernel.o : kernel/kernel.asm
	$(ASM) $(ASMKFLAG) -o $@ $<

$(KERNEL) : $(OBJS)
	$(LD) $(LDFLAG) -o $(KERNEL) $(OBJS)

kernel/global.o : kernel/global.c include/type.h include/const.h include/protect.h \
			include/proto.h include/global.h
	$(CC) $(CFLAG) -o $@ $<

kernel/start.o : kernel/start.c include/type.h include/const.h include/protect.h \
			include/proto.h include/global.h include/string.h
	$(CC) $(CFLAG) -o $@ $<


lib/klib.o : lib/klib.c include/type.h include/const.h include/protect.h \
			include/proto.h include/global.h
	$(CC) $(CFLAG) -o $@ $<

lib/kliba.o : lib/kliba.asm 
	$(ASM) $(ASMKFLAG) -o $@ $<

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAG) -o $@ $<

kernel/i8259.o : kernel/i8259.c include/type.h include/const.h include/protect.h \
			include/proto.h
	$(CC) $(CFLAG) -o $@ $<

kernel/protect.o : kernel/protect.c include/type.h include/const.h include/protect.h \
			include/proto.h include/global.h
	$(CC) $(CFLAG) -o $@ $<

kernel/main.o : kernel/main.c include/type.h include/const.h include/protect.h \
			include/proto.h include/global.h include/proc.h
	$(CC) $(CFLAG) -o $@ $<

kernel/clock.o : kernel/clock.c include/type.h include/const.h include/proto.h include/global.h\
			include/protect.h
	$(CC) $(CFLAG) -o $@ $<

start:
	dd if=boot/boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp -fv boot/loader.bin /mnt/floppy
	sudo cp -fv kernel.bin /mnt/floppy
	sudo umount /mnt/floppy/
