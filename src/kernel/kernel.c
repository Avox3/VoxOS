
#include "../drivers/display.h"
#include "interrupts/idt.h"

void kmain(void)
{
	char *str = "Welcom to VoxOS";
	settextcolor(WHITE, LIGHT_BLUE);
	clear();
	kprint(str);

	// set up the interrupt descriptor table
	idt_initialize();

	// halt the system
	return;
}
