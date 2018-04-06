
#include "../drivers/display.h"

void kmain(void)
{
	char *str = "Welcom to VoxOS";

	clear();
	kprint(str);

	// halt the system
	return;
}
