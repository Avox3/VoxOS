;************************************************;
;	Prints a string
;	DS=>SI: 0 terminated string
;************************************************;

Print:
			pusha    ; store registers

			.Loop:
				lodsb				; load next byte from string from SI to AL
				or	al, al			; Does AL=0?
				jz	PrintDone		; Yep, null terminator found-bail out
				mov	ah, 0eh			; Nope-Print the character
				int	10h
				jmp	.Loop			; Repeat until null terminator found

	PrintDone:
			popa    ; restore registers to original state
			ret				; we are done, so return
