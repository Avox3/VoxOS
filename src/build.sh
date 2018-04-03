
# compile bootloader
nasm -g -f elf32 -F dwarf -o boot.o src/boot/Stage2/Stage2.asm
ld -melf_i386 -Ttext=0x7c00 -nostdlib --nmagic -o boot.elf boot.o
objcopy -O binary boot.elf boot.bin

# compile and link kernel
gcc -g -m32 -c -ffreestanding -o kernel.o src/kernel/kernel.c -lgcc
ld -melf_i386 -T src/linker.ld -nostdlib --nmagic -o kernel.elf kernel.o
objcopy -O binary kernel.elf kernel.bin

# build disk image
dd if=/dev/zero of=disk.img bs=512 count=2880
dd if=boot.bin of=disk.img bs=512 conv=notrunc
dd if=kernel.bin of=disk.img bs=512 seek=1 conv=notrunc

# run on qemu emulator
qemu-system-i386 -fda disk.img
