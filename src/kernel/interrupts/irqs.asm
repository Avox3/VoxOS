[extern irq_handler]

irq_common_stub:
    ; save all registers - CPU state
    pusha
    ; save the data segment descriptor
    mov ax, ds
    push eax
    ; load the Kernel data segment descriptor
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; call the interrupt service routine - handle the interrupt
    call irq_handler

    ; restore registers
    pop ebx
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    ; cleanup error code and IRQ - ISR number
    add esp, 8
    ; pop CS, EIP, EFLAGS, SS and ESP
    iret


global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
  cli
  push byte 0
  push byte 0
  jmp irq_common_stub

irq1:
  cli
  push byte 0
  push byte 1
  jmp irq_common_stub

irq2:
  cli
  push byte 0
  push byte 2
  jmp irq_common_stub

irq3:
  cli
  push byte 0
  push byte 3
  jmp irq_common_stub

irq4:
  cli
  push byte 0
  push byte 4
  jmp irq_common_stub

irq5:
  cli
  push byte 0
  push byte 5
  jmp irq_common_stub

irq6:
  cli
  push byte 0
  push byte 6
  jmp irq_common_stub

irq7:
  cli
  push byte 0
  push byte 7
  jmp irq_common_stub

irq8:
  cli
  push byte 8
  jmp irq_common_stub

irq9:
  cli
  push byte 0
  push byte 9
  jmp irq_common_stub

irq10:
  cli
  push byte 10
  jmp irq_common_stub

irq11:
  cli
  push byte 11
  jmp irq_common_stub

irq12:
  cli
  push byte 12
  jmp irq_common_stub

irq13:
  cli
  push byte 13
  jmp irq_common_stub

irq14:
  cli
  push byte 14
  jmp irq_common_stub

irq15:
  cli
  push byte 0
  push byte 15
  jmp irq_common_stub
