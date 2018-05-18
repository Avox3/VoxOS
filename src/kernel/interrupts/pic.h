#ifndef PIC_H
#define PIC_H

#include "../../drivers/utils.h"
#include "isr.h"

#define IMPLEMENTED_IRQS 16

// IRQS interrupt number
#define IRQ0 0
#define IRQ1 1
#define IRQ2 2
#define IRQ3 3
#define IRQ4 4
#define IRQ5 5
#define IRQ6 6
#define IRQ7 7
#define IRQ8 8
#define IRQ9 9
#define IRQ10 10
#define IRQ11 11
#define IRQ12 12
#define IRQ13 13
#define IRQ14 14
#define IRQ15 15

// PIC1 - Master PIC
// PIC2 - Slave PIC
#define PIC1		0x20  // IO base address for master PIC
#define PIC2		0xA0  // IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

// ICW1 - notify of the PIC initialization
#define ICW1_INIT 0x11

// ICW2 - map the base address of the IVT of which the PIC are to use
#define P1_INTERRUPT_20H 0x20  // primary pic handled IRQ 0-7(used), mapped to interrupt number 0x20
#define P2_INTERRUPT_28H 0x28  // secondrary pic handled IRQ 8-15(used), mapped to interrupt number 0x28

// ICW3 - set the communication between the PICs
#define P1_IRQ_LINE 0x4
#define P2_IRQ_LINE 0x2

// ICW4 - final initialization control words
#define ICW4_INIT 0x1

#define PIC_EOI	0x20  // End-of-interrupt command code


// access irqs from interrupt.asm
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void pic_initialize();
void irq_handler(registers_t r);
void irq_install_handler(int irq, isr_t handler);

#endif
