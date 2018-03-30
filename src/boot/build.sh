# create bootloader image file
dd if=/dev/zero of=disk.img bs=1024 count=720
# assemble stage 1
nasm -f bin src/boot/Stage1/Boot1.asm -o bootload.bin
dd if=bootload.bin of=disk.img conv=notrunc

# assemble stage 1
nasm -f bin src/boot/Stage2/Stage2.asm -o stage2.bin
dd if=stage2.bin of=disk.img bs=512 seek=1 conv=notrunc

# delete bin files
rm bootload.bin stage2.bin

# run on qemu emulator
qemu-system-i386 -fda disk.img
