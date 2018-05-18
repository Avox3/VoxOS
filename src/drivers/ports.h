
#ifndef PORTS_H
#define PORTS_H

#include "utils.h"

void outb(u16int port, u8int val);
u8int inb(u16int port);
void io_wait();

#endif
