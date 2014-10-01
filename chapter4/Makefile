#Program, flags
ASM	= nasm

TARGIT	= boot.bin loader.bin

everything : $(TARGIT) start

boot.bin : boot.asm
	nasm -o $@ $<

loader.bin : loader.asm
	nasm -o $@ $<

start:
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
	sudo mount -o loop a.img /mnt/floppy/
	sudo cp loader.bin /mnt/floppy -v
	sudo umount /mnt/floppy/
