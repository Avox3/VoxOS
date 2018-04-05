
all: run


boot.bin: boot/Stage2/Stage2.asm
	nasm -g -f elf32 -F dwarf -o $@ $<
	ld -melf_i386 -Ttext=0x7c00 -nostdlib --nmagic -o boot.elf $@
	objcopy -O binary boot.elf $@


kernel.o:
	gcc -g -m32 -c -ffreestanding -o $@ kernel/kernel.c -lgcc


kernel.bin: kernel.o
	ld -melf_i386 -T linker.ld -nostdlib --nmagic -o kernel.elf $<
	objcopy -O binary kernel.elf $@


disk.img: boot.bin kernel.bin
	dd if=/dev/zero of=disk.img bs=512 count=2880
	dd if=boot.bin of=disk.img bs=512 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc


run: disk.img clean
	qemu-system-i386 -fda $<


clean:
	rm *.bin *.o *.elf
