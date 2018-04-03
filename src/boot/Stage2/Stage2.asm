
bits 16		; we are still in real mode
					; this time, we have no limitation of 512 bytes...

KERNEL_ADDR equ 0x9000
SECTORS equ 1
DATA_SEG equ 10h
CODE_SEG equ 8h

global _start
_start:

	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	; set stack pointer at SS:SP = 0x0000:0x8000
	mov sp, 0x8000

	; print loading message
	mov si, LoadMsg
	call Print

	; load the kernel
	call load_kernel

	; load global descriptor table
	call install_GDT

	; enable A20
	call enable_A20
	mov si, A20_success
	call Print

	; set bit 0 in cr0 - enter pmode
	mov	eax, cr0
	or eax, 1
	mov	cr0, eax

	jmp CODE_SEG:stage3


load_kernel:

	mov bx, KERNEL_ADDR
	mov al, SECTORS   ; Read DH sectors
  mov ah , 2       ; BIOS read sector function
  mov ch , 0       ; Select cylinder 0
  mov dh , 0       ; Select head 0
  mov cl , 2       ; Start reading from second sector ( i.e.
                      ; after the boot sector )

  int 0x13            ; BIOS interrupt
  jc disk_error       ; Jump if error ( i.e. carry flag set )

  cmp al, SECTORS      ; if AL ( sectors read ) != DH ( sectors expected )
  jne disk_error      ; display error message
  ret

disk_error:
  mov si, ErrorMsg
  call Print
  hlt


;*************************************************;
;	Includes Section
;************************************************;
%include 'src/boot/Others/utils.inc'
%include 'src/boot/Stage2/A20.inc'
%include 'src/boot/Stage2/Gdt.inc'

;*************************************************;
;	Data Section
;************************************************;
ErrorMsg db 13, 10, "Error :(", 13, 10, 0
A20_success	db	13, 10, "Enabled A20 successfully...",13, 10, 0
LoadMsg	db	13, 10, "Preparing to load operating system...",13, 10, 0



[bits 32]
stage3:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	jmp 0x9000

	cli
  hlt  ; halt the CPU


times 510-($-$$) db 0
dw 0xAA55
