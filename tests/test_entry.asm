global _test_start
[bits 32]

_test_start:
    [extern test_main]
    call test_main ; Calls the C function
    ret ; return control to the kernel (hopefully)
