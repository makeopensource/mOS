[org 0x7c00]
OS_OFFSET equ 0x1000

[bits 16]
begin:
    jmp code
    RETRY_COUNT dw 0
    DRIVE_N_HEADS db 0
    DRIVE_SECTS_PER_TRACK db 0
code:
    mov [BOOT_DRIVE], dl
    mov ax, 0
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov bp, 0x9000
    mov sp, bp

;;; Get Drive Parameters
    mov ah, 8
    int 0x13
    add dh, 1
    mov [DRIVE_N_HEADS], dh
    and cl, 0x3f
    mov [DRIVE_SECTS_PER_TRACK], cl
    jmp load_kernel

%include "src/boot/gdt.asm"
%include "src/boot/enter_pm.asm"

[bits 16]
load_kernel:

;;; Read Drive
    mov bl, 1
read:
    mov al, bl ;sector to read
    call read_drive
    add bl, 1
    cmp bl, 24
    jne read

    mov ax, [OS_OFFSET]
    cmp ax, 0x03e8
    ;; jne error
    call enter_pm

error:
        mov bx, OS_OFFSET
        mov cx, bx
        add cx, 0x200
error_out:
        mov ax, [bx]
        call print_word
        call print_nl
        add bx, 4
        cmp bx, cx
        jne error_out
        jmp $

;;; Reads from drive in reg dl, read sector LBA al
read_drive:
    push bx
    push cx
        
;;; Calculate Offset for Kernel
    mov cl, al
    mov ch, 0
    sub cl, 1
    shl cx, 9
    mov bx, OS_OFFSET
    add bx, cx
    push bx

;;; Calculate C,H,S using algorithm from OSDev:
;;; https://wiki.osdev.org/Disk_access_using_the_BIOS_(INT_13h)
    mov ah, 0
    div byte [DRIVE_SECTS_PER_TRACK]
    add ah, 1
    mov bl, ah
    mov ah, 0
    div byte [DRIVE_N_HEADS]
    mov dh, ah
    mov ch, al
    mov cl, bl

    pop bx
    push ax
    mov ax, es
    call print_word
    call print_cm
    mov ax, 0
    mov es, ax
    pop ax
    mov al, 1
    mov dl, [BOOT_DRIVE]
    clc
    mov ah, 2
    int 0x13 ;do read
    mov bx, 0
    mov es, bx
    call print_word
    call print_nl

    pop cx
    pop bx
    ret
        
retry_drive:
;;; Print Sectors Transferred
    push ax ; Save error code
    add al, 48
    call print_char
    mov al, 44
    call print_char
    pop ax ; Retreive error code
    mov al, ah
    add al, 48
    call print_char
    mov al, 10
    call print_char

    mov ax, [RETRY_COUNT]
    add ax, 1
    mov [RETRY_COUNT], ax
    cmp ax, 1000
    jne load_kernel
    jmp $

;;; Prints character in register al
print_char:
    push ax
    push bx
    mov ah, 0x0e
    mov bl, 1
    mov bh, 0
    int 0x10
    pop bx
    pop ax
    ret

print_half_byte:
    and al, 0x0F
    add al, 48
    cmp al, 57
    jle skip_hex
    add al, 7
skip_hex:
    call print_char
    ret
        
print_byte:
    push ax

    mov ax, [esp]
    shr al, 4
    call print_half_byte
    mov ax, [esp]
    mov ah, 0
    call print_half_byte

    pop ax
    ret

print_word:
    push ax
    mov al, ah
    call print_byte
    mov ax, [esp]
    call print_byte
    pop ax
    ret

print_nl:
    push ax
    mov al, 32
    call print_char
    mov al, 32
    call print_char
    pop ax
    ret

print_cm:
    push ax
    mov al, 44
    call print_char
    pop ax
    ret
        
[bits 32]
begin_pm:
    call OS_OFFSET
    hlt


times 509 - ($ - $$) db 0 ;padding


BOOT_DRIVE db 0 ;0x7dfd
;above is data that can always be found at 0x7dfd - n during boot process

dw 0xaa55 ;magic boot sector number
