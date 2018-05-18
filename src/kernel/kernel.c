
#include "../drivers/display.h"
#include "interrupts/idt.h"

void kmain(void)
{
	char *str = "Welcom to VoxOS\n";

	// initialize IDT
	idt_initialize();
	// enable interrupts
	asm volatile("sti");

	// display output
	clear();
	kprint(str);

	return;
}
