
; org		0x7c00					; we will set regisers later

bits	16						; we are in 16 bit real mode

STAGE2_ADDR equ 0x07e0

global _start
_start:
	jmp	main					; jump to start of bootloader


load_stage2:
	mov		ah, 0					; reset floppy disk function
	mov   dl, [bootDrive]  ; drive number
	int		0x13					; call BIOS
	jc		load_stage2			; If Carry Flag (CF) is set, there was an error. Try resetting again

	mov		ax, STAGE2_ADDR     	; we are going to read sector straight after the boot sector in memory
	mov		es, ax  ; set es register with second sector address
	xor		bx, bx  ; offset to read sector to

	read_sector:
		mov		ah, 2				  ; function 2
		mov		al, 1					; read 1 sector
		mov		ch, 0					; we are reading the second sector past us, so its still on track 1(0)
		mov		cl, 2					; sector to read (The second sector)
		mov		dh, 0					; head number
		mov   dl, [bootDrive]  ; drive number
		int		0x13					; call BIOS - Read the sector
		jc		read_sector		; Error, so try again

		; load stage2
		mov si, msgStage2
		call Print

		jmp STAGE2_ADDR:0x0000


main:
	; At start bootloader sets DL to boot drive
	mov [bootDrive], dl

	; setup segments to insure they are 0
	xor	ax, ax
	mov	ds, ax
	mov	es, ax

	cli               ; Turn off interrupts for SS:SP update
										; to avoid a problem with buggy 8088 CPUs
	mov ss, ax        ; SS = 0x0000
	mov sp, 0x7c00    ; SP = 0x7c00
										; We'll set the stack starting just below
										; where the bootloader is at 0x0:0x7c00. The
										; stack can be placed anywhere in usable and
										; unused RAM.
	sti               ; Turn interrupts back on

	; print loading message
	mov si, msgLoading
	call Print

	jmp load_stage2
	cli
	hlt

;*************************************************;
;	Includes Section
;************************************************;

%include 'src/boot/Others/utils.inc'

;*************************************************;
;	Data Section
;************************************************;

msgLoading  db 13, 10, "Loading Boot Image ", 13, 10, 0
msgStage2 db 13, 10, "Loading the stage2 boot loader onto memory", 13, 10, 0
bootDrive db 0

times 510-($-$$) db 0
dw 0xAA55
