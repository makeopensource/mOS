[org 0x7c00]
OS_OFFSET equ 0x1000

[bits 16]
begin:
	mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp
    jmp detect_mem

%include "src/boot/gdt.asm"
%include "src/boot/enter_pm.asm"
%include "src/boot/detect_mem.asm"

[bits 16]
load_kernel:
	mov dl, [BOOT_DRIVE] ;reset dl
    mov ah, 2 ;read BIOS chs
    mov al, 42 ;sectors to read
    mov cl, 0x02 ;start at sector 2
    mov ch, 0x00 ;cylinder
    mov dh, 0x00 ;head
    mov bx, OS_OFFSET
    int 0x13 ;do read
    call enter_pm

[bits 32]
begin_pm:
    call OS_OFFSET
    hlt

times 509 - ($ - $$) db 0 ;padding


BOOT_DRIVE db 0 ;0x7dfd
;above is data that can always be found at 0x7dfd - n during boot process

dw 0xaa55 ;magic boot sector number
