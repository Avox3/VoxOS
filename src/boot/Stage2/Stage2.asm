[bits 16]  ; System is in real mode

KERNEL_ADDR equ 0x9000
STACK_LIMIT equ 0x8000
SECTORS equ 17
CODE_SEG equ 8h
DATA_SEG equ 10h

global _start
_start:

    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, STACK_LIMIT      ; Stack pointer at SS:SP = 0x0000:0x8000

		; print loading message
		mov si, LoadMsg
		call Print

		mov [BOOT_DRIVE], dl  ; Boot drive passed to us by the BIOS

		; load the kernel
		call load_kernel

		; enable A20 gate
    call enable_A20

		; install gdt
    call install_GDT

		; set bit 0 in cr0 - enter pmode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp CODE_SEG:stage3


load_kernel:
                        ; load DH sectors to ES:BX from drive DL
    push dx             ; Store DX on stack so later we can recall
                        ; how many sectors were request to be read ,
                        ; even if it is altered in the meantime
		mov bx, KERNEL_ADDR
    mov ah, 2h       ; BIOS read sector function
    mov al, SECTORS         ; Read DH sectors
    mov ch, 0       ; Select cylinder 0
    mov dh, 0       ; Select head 0
    mov cl, 2h       ; Start reading from second sector ( i.e.
                        ; after the boot sector )
    int 13h            ; BIOS interrupt
    jc disk_error       ; Jump if error ( i.e. carry flag set )
    pop dx              ; Restore DX from the stack
    cmp al, SECTORS        ; if AL ( sectors read ) != DH ( sectors expected )
    jne disk_error      ; display error message
    ret

disk_error :
    mov si, ErrorMsg
    call Print
    hlt



;*************************************************;
;	Includes Section
;************************************************;
%include 'boot/Others/utils.inc'
%include 'boot/Stage2/A20.inc'
%include 'boot/Stage2/Gdt.inc'

[bits 32]
; protected mode "entry"
stage3:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call KERNEL_ADDR
    cli

	; halt cpu
	loopend:
	    hlt
	    jmp loopend

[bits 16]
;*************************************************;
;	Data Section
;************************************************;
ErrorMsg db 13, 10, "Error :(", 13, 10, 0
A20_success	db	13, 10, "Enabled A20 successfully...",13, 10, 0
LoadMsg	db	13, 10, "Preparing to load operating system...",13, 10, 0
BOOT_DRIVE db 0

times 510-($-$$) db 0  ; 512 boot sector
dw 0xAA55  ; bootable sector signature
