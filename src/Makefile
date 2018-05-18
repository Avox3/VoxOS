C_SOURCES = $(wildcard drivers/*.c kernel/interrupts/*.c)
HEADERS = $(wildcard drivers/*.h kernel/interrupts/*.h)
OBJS = $(C_SOURCES:.c=.o kernel/interrupts/interrupt.o kernel/interrupts/irqs.o)
TARGET = disk.img

CFLAGS = -g -m32
CC = gcc

all: run

boot.bin: boot/Stage2/Stage2.asm
	nasm -g -f elf32 -F dwarf -o $@ $<
	ld -melf_i386 -Ttext=0x7c00 -nostdlib --nmagic -o boot.elf $@
	objcopy -O binary boot.elf $@


kernel.o:
	${CC} ${CFLAGS} -c -ffreestanding -o $@ kernel/kernel.c -lgcc


kernel.bin: kernel.o $(OBJS)
	ld -melf_i386 -Tlinker.ld -nostdlib --nmagic -o kernel.elf $^
	objcopy -O binary kernel.elf $@


$(TARGET): boot.bin kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=boot.bin of=$@ bs=512 conv=notrunc
	dd if=kernel.bin of=$@ bs=512 seek=1 conv=notrunc

run: $(TARGET)
	qemu-system-i386 -fda $<

# create the object files
%.o: %.c $(HEADERS)
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@ -lgcc

%.o: %.asm
	nasm $< -f elf -o $@

clean:
	rm *.bin *.o *.elf drivers/*.o kernel/interrupts/*.o

.PHONY : all clean
