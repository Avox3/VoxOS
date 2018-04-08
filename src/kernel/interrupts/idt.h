#ifndef IDT_H
#define IDT_H

#define MAX_INTERRUPTS 256
#define IMPLEMENTED_INTERRUPTS 32
#define KERNEL_CS 8

#include "../../drivers/utils.h"

// A struct describing an interrupt gate.
typedef struct
{
    u16int base_lo;             // the lower 16 bits of the interrupt routine address
    u16int sel;                 // kernel segment selector
    u8int  reserved;            // reserved - this must always be zero
    u8int  flags;               // flags
    u16int base_hi;             // the upper 16 bits of the interrupt routine address
} __attribute__((packed)) idt_entry;

// A struct describing a pointer to an array of interrupt handlers.
typedef struct
{
    u16int limit;               // the size of the interrupt descriptor table
    u32int base;                // the base address of the interrupt descriptor table
} __attribute__((packed)) idt_ptr;

void idt_set_ir(u8int num, u32int base);
void idt_initialize();

#endif
