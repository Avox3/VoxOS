// IDT - Interrupt Descriptor Table
#include "idt.h"
#include "isr.h"
#include "pic.h"


// all IDT enteries
idt_entry idt_entries[256];
// IDT pointer
idt_ptr idtp;

// installs single interrupt handler in the IDT
void idt_set_ir(u8int num, u32int base)
{
    idt_entries[num].base_lo = (base & 0xFFFF);
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = KERNEL_CS;
    idt_entries[num].reserved =	0;
    idt_entries[num].flags = 0x8E;
}

// load the IDT into IDTR with lidt instruction
void idt_load()
{
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idtp));
}

// install and load the IDT
void idt_initialize()
{
    // initalize the pointer to the IDT
    idtp.limit = sizeof(idt_entry) * MAX_INTERRUPTS - 1;
    idtp.base = (u32int) & idt_entries;

    // install isrs
    install_int_handlers();
    // install iqrs
    pic_initialize();
    // load the interrupt descriptor table
    idt_load();
}
