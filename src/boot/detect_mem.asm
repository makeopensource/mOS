;code below is largely taken from osdev.org
;see https://wiki.osdev.org/Detecting_Memory_(x86) for more details

[bits 16]
detect_mem:
	mov di, 0x8004
	xor ebx, ebx
	xor bp, bp
	mov edx, 0x534D4150
	mov eax, 0xe820
	mov [es:di + 20], dword 1 ; force a valid ACPI 3.X entry
	mov ecx, 24	; ask for 24 bytes
	int 0x15
	jmp load_kernel

[bits 16]
.failed:
	stc	; "function unsupported" error exit
	ret
