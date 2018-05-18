#include "pic.h"
#include "idt.h"
#include "../../drivers/ports.h"


u32int irqs[IMPLEMENTED_IRQS] = {
  (u32int)irq0, (u32int)irq1, (u32int)irq2, (u32int)irq3,
  (u32int)irq4, (u32int)irq5, (u32int)irq6, (u32int)irq7,
  (u32int)irq8, (u32int)irq9, (u32int)irq10, (u32int)irq11,
  (u32int)irq12, (u32int)irq13, (u32int)irq14, (u32int)irq15
};

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// send end of interrupt, insure all hardware interrupts are enabled at the end
void send_EOI(u8int PICx)
{
  outb(PICx, PIC_EOI);
}

void disable_irq(unsigned char irq) {
    u16int port;
    u8int value;

    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

void enable_irq(unsigned char irq) {
    u16int port;
    u8int value;

    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void register_irq_handler(int irq, isr_t handler)
{
    irq_routines[irq] = handler;
}

void unregister_irq_handler(u8int irq)
{
    irq_routines[irq] = 0;
}

// this function handles the interrupts, called from ASM interrupt handler stub
void irq_handler(registers_t r) {

    // char *s = itoa(r.int_no, 10);
    // kprint(s);

    if(r.int_no >= 40)
    {
        // send EOI - reset signal to the secondrary PIC
        send_EOI(PIC2);
    }
    // send EOI - reset signal to the primary PIC
    send_EOI(PIC1);

    isr_t handler = irq_routines[r.int_no];

    if(handler != 0)  // check if there is an handler to the interrupt
    {
        handler(r);
    }

}

void pic_initialize()
{
    /*
    The initialization of the PICs - remap the PICs' IRQ numbers. Done through a series of command words,
    which are being sent as Initialization Control Words (ICW).
    */

    // save state
    u8int a1, a2;
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // ICW1 - notify of the PIC initialization
    outb(PIC1_COMMAND, ICW1_INIT);
    outb(PIC2_COMMAND, ICW1_INIT);

    // ICW2 - map the base address of the IVT of which the PIC are to use
    outb(PIC1_DATA, P1_INTERRUPT_20H);
    outb(PIC2_DATA, P2_INTERRUPT_28H);

    // ICW3 - set the communication between the PICs
    outb(PIC1_DATA, P1_IRQ_LINE);
    outb(PIC2_DATA, P2_IRQ_LINE);

    // ICW4 - final initialization control words
    outb(PIC1_DATA, ICW4_INIT);
    outb(PIC2_DATA, ICW4_INIT);

    // restore state
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a1);

    // set irqs
    for (int i = 0; i < IMPLEMENTED_IRQS; i++)
    {
        idt_set_ir(i + 32, irqs[i]);
    }

}
