
bits 16		; we are still in real mode
					; this time, we have no limitation of 512 bytes...

org 0x0000

jmp stage2	; jump to main

stage2:

	; Set DS = CS
	mov ax, cs
	mov ds, ax

	cli	; clear interrupts

	mov	ax, 0x9000 ; stack begins at 0x9000-0xffff
	mov	ss, ax
	mov	sp, 0xFFFF
	sti ; enable interrupts

	; print loading message
	mov si, LoadMsg
	call Print

	; load global descriptor table
	call install_GDT

	cli	; clear interrupts
	mov	eax, cr0	; set bit 0 in cr0 - enter pmode
	or	eax, 1
	mov	cr0, eax

	; enable A20
	call enable_A20

	cli		; clear interrupts to prevent triple faults
	hlt		; hault the system

;*************************************************;
;	Includes Section
;************************************************;

%include 'src/boot/Others/utils.inc'
%include 'src/boot/Stage2/A20.inc'
%include 'src/boot/Stage2/Gdt.inc'

;*************************************************;
;	Data Section
;************************************************;
A20_success	db	13, 10, "Enabled A20 successfully...",13, 10, 0
LoadMsg	db	13, 10, "Preparing to load operating system...",13, 10, 0
