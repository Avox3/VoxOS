
#ifndef UTILS_H
#define UTILS_H

#define TRUE 1
#define FALSE 0

#define VIDEO_MEMORY_ADDRESS 0xb8000;

#define SCREEN_ROWS 25
#define SCREEN_COLUMNS 80

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

// define some types for all platforms
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

// struct which aggregates many registers
typedef struct {
    u32int ds; // data segment selector
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha registers
    u32int int_no, err_code; // interrupt number and error code (if applicable)
    u32int eip, cs, eflags, useresp, ss; // pushed by the processor automatically
} registers_t;

void int_to_ascii(u8int val, char str[], u8int len);

#endif
