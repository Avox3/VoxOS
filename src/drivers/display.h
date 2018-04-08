#ifndef DISPLAY_H
#define DISPLAY_H

#include "utils.h"

// void settextcolor(u8int forecolor, u8int backcolor);
void clear();
void putc(char c);
void kprint(char *c);
void update_cursor(int x, int y);
void settextcolor(u16int forecolor, u16int backcolor);

#endif
