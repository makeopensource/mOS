;code below is largely taken from osdev.org, all of which is on the Public Domain
;see https://wiki.osdev.org/Detecting_Memory_(x86) for more details
;for license information, see https://wiki.osdev.org/OSDev_Wiki:License

num_ent equ 0x8000 ;stores number of entries
struct_base equ 0x8004 ;base ptr of structure

[bits 16]
detect_mem:
	mov di, struct_base
    xor ebx, ebx
    xor bp, bp
    jmp detect_chunk

[bits 16]
detect_chunk:
	mov edx, 0x0534D4150
	mov eax, 0xe820
	mov [es:di + 20], dword 1 ; force a valid ACPI 3.X entry
	mov ecx, 24	; ask for 24 bytes
	int 0x15

    cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne failed

    cmp bp, 0
    je short check_first
    jmp detect_chunk_2

detect_chunk_2:
    inc bp
    add di, 24

    cmp bp, 10
	jne detect_chunk

	jmp post_process

[bits 16]
check_first:
	jc short failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short failed
    jmp short detect_chunk_2

[bits 16]
failed:
	stc
	ret

[bits 16]
skip_ent:
	test ebx, ebx ;second 0 indicates end of list
	jne short post_process

[bits 16]
post_process:
	mov [num_ent], bp
	jmp load_kernel
