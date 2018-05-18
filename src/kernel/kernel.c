
#include "../drivers/display.h"
#include "../drivers/keyboard.h"
#include "interrupts/idt.h"

void kmain(void)
{
	char *str = "Welcom to VoxOS\n\n";

	// install IDT
	idt_initialize();
	// enable interrupts
	__asm__ __volatile__ ("sti");

	// initialize drivers
	initialize_keyboard();

	// welcome screen
	clear();
	kprint(str);

	for (;;);

}
