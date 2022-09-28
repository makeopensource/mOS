OUTPUT_FORMAT("elf32-i386");

ENTRY(start);

SECTIONS {
    . = 0x00007E00;
    .text :  ALIGN(4) {
        os_entry.o(.text);
	. = ALIGN(4);        
	*(.text);
    }

    .data :  ALIGN(4) {
        *(.data);
        *(.rodata*);
    }

    .bss :   ALIGN(4) {
        __bss_start = .;
        *(COMMON);
        *(.bss)
        . = ALIGN(4);
        __bss_end = .;
    }
    __bss_sizeb = SIZEOF(.bss);

    /DISCARD/ : {
        *(.eh_frame);
        *(.comment);
    }
}

