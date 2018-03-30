
bits 16		; we are still in real mode
					; this time, we have no limitation of 512 bytes...

org 0x0000

jmp stage2	; jump to main

stage2:
			; Set DS = CS
			mov ax, cs
			mov ds, ax

			mov si, Msg
			call Print

			cli		; clear interrupts to prevent triple faults
			hlt		; hault the system

;*************************************************;
;	Includes Section
;************************************************;

%include 'src/boot/Others/utils.inc'

;*************************************************;
;	Data Section
;************************************************;

Msg	db	13, 10, "Preparing to load operating system...",13, 10, 0
