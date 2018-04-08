#include "isr.h"
#include "idt.h"
#include "../../drivers/display.h"

// interrupt handlers' messages
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// this function installs the interrupt handlers
void install_int_handlers()
{
    u32int isrs[IMPLEMENTED_INTERRUPTS] = {
      (u32int)isr0, (u32int)isr1, (u32int)isr2, (u32int)isr3,
      (u32int)isr4, (u32int)isr5, (u32int)isr6, (u32int)isr7,
      (u32int)isr8, (u32int)isr9, (u32int)isr10, (u32int)isr11,
      (u32int)isr12, (u32int)isr13, (u32int)isr14, (u32int)isr15,
      (u32int)isr16, (u32int)isr17, (u32int)isr18, (u32int)isr19,
      (u32int)isr20, (u32int)isr21, (u32int)isr22, (u32int)isr23,
      (u32int)isr24, (u32int)isr25, (u32int)isr26, (u32int)isr27,
      (u32int)isr28, (u32int)isr29, (u32int)isr30, (u32int)isr31
    };

    // register the interrupts handlers - service routines
    for (int i=0; i < IMPLEMENTED_INTERRUPTS; i++)
    {
      idt_set_ir(i, isrs[i]);
    }
}

// this function handles the interrupts
void isr_handler(registers_t r) {

    kprint("\nReceived interrupt: ");

    // get interrupt number
    char s[3];
    int_to_ascii(r.int_no, s, sizeof(s));

    // print output
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r.int_no]);
    kprint("\n");
}
