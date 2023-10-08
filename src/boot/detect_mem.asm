;code below is largely taken from osdev.org, all of which is on the Public Domain
;see https://wiki.osdev.org/Detecting_Memory_(x86) for more details
;for license information, see https://wiki.osdev.org/OSDev_Wiki:License

[bits 16]
detect_mem:
	mov di, 0x8004
	xor ebx, ebx
	xor bp, bp
	mov edx, 0x0534D4150
	mov eax, 0xe820
	mov [es:di + 20], dword 1 ; force a valid ACPI 3.X entry
	mov ecx, 24	; ask for 24 bytes
	int 0x15
	jc short .failed
	mov edx, 0x0534D4150
	cmp eax, edx
	jne short .failed
	test ebx, ebx ; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp load_kernel

.next_item:
	

[bits 16]
.failed:
	stc	; "function unsupported" error exit
	ret
