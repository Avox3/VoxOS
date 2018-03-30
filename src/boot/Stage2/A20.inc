
; Function: check_A20
;
; Purpose: to check the status of the a20 line in a completely self-contained state-preserving way.
;          The function can be modified as necessary by removing push's at the beginning and their
;          respective pop's at the end if complete self-containment is not required.
;
; Returns: 0 in ax if the a20 line is disabled (memory wraps around)
;          1 in ax if the a20 line is enabled (memory does not wrap around)

check_A20:
    pushf
    push ds
    push es
    push di
    push si

    cli

    xor ax, ax ; ax = 0
    mov es, ax

    not ax ; ax = 0xFFFF
    mov ds, ax

    mov di, 0x0500
    mov si, 0x0510

    mov al, byte [es:di]
    push ax

    mov al, byte [ds:si]
    push ax

    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF

    cmp byte [es:di], 0xFF

    pop ax
    mov byte [ds:si], al

    pop ax
    mov byte [es:di], al

    mov ax, 0
    je check_A20_exit

    mov ax, 1

check_A20_exit:
    pop si
    pop di
    pop es
    pop ds
    popf

    ret


; wait for input buffer to be clear
wait_input:
  in      al,0x64		; read status register
  test    al,2		; test bit 2 (Input buffer status)
  jnz     wait_input	; jump if its not 0 (not empty) to continue waiting


; wait for output buffer to be clear
wait_output:
  in      al,0x64
  test    al,1
  jz      wait_output
  ret


enable_A20:

  ; check if enabling a20 succeed
  call check_A20
  cmp ax, 1
  je enable_A20_success

  ; fast method of enabling
  ; but not a portable method, it's not gonna work with all hardwares
  mov	al, 2	; set bit 2 (enable a20)
  out	0x92, al

  ; check if enabling a20 succeed
  call check_A20
  cmp ax, 1
  je enable_A20_success

  ; try through bios's interrupt
  mov ax, 0x2401
  int 0x15

  ; check if enabling a20 succeed
  call check_A20
  cmp ax, 1
  je enable_A20_success

  ; try with keyboard controller, most portable method

  ; send read output port command
  mov al,0xD0
  out 0x64,al
  call wait_output

  ; read input buffer and store on stack. This is the data read from the output port
  in      al,0x60
  push    eax
  call    wait_input

  ; send write output port command
  mov     al,0xD1
  out     0x64,al
  call    wait_input

  ; pop the output port data from stack and set bit 1 (A20) to enable
  pop     eax
  or      al, 2
  out     0x60, al

  enable_A20_success:
    mov si, A20_success
    call Print
    ret
