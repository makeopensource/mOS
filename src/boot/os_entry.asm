global _start
[bits 32]

_start:
    [extern os_main]
    call os_main ; Calls the C function
    jmp $
