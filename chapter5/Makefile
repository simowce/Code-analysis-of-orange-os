#Program, flags
ASM	= nasm
LD	= ld
ASMFLAG	= -f elf
LDFLAG	= -s -Ttext 0x30400 -m elf_i386

TARGIT	= boot.bin loader.bin kernel.o kernel.bin

all : $(TARGIT) start

boot.bin : boot.asm
	$(ASM) -o $@ $<

loader.bin : loader.asm lib.inc
	$(ASM) -o $@ $<

kernel.o : kernel.asm
	$(ASM) $(ASMFLAG) -o $@ $<

kernel.bin : kernel.o
	$(LD) $(LDFLAG) -o $@ $<
start:
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp loader.bin /mnt/floppy -v
	sudo cp kernel.bin /mnt/floppy -v
	sudo umount /mnt/floppy/
