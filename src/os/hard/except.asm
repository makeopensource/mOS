%macro stub_setup 0
	push ds ; save all registers
	push es
	push fs
	push gs
	pushad

	cld	; c ABI stuff
	mov ax, 16 ; kernel data segment
	mov ds, ax ; set segment selectors
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp ; more c ABI

%endmacro

%macro stub_end 0
	pop eax ; pop saved esp

	popad	; restore all registers
	pop gs
	pop fs
	pop es
	pop ds

	add esp, 8 	; pop error code and isr index
%endmacro

; fun fact: this is very similar to how PintOS does it
%macro isr_err_stub 1
isr_stub_%+%1:
	push %1
	stub_setup
	;call isrHandler
	stub_end
	iret
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
	push 0 ; dummy error code
	push %1
	stub_setup
	;call isrHandler
	stub_end
	iret
%endmacro

%macro irq_stub 1
irq_stub_%+%1:
	push 0 ; dummy error code
	push %1
	stub_setup
	;call irqHandler
	stub_end
	iret
%endmacro

extern isrHandler
isr_no_err_stub 0 
isr_no_err_stub 1 
isr_no_err_stub 2 
isr_no_err_stub 3 
isr_no_err_stub 4 
isr_no_err_stub 5 
isr_no_err_stub 6 
isr_no_err_stub 7 
isr_err_stub	8 
isr_no_err_stub 9 
isr_err_stub	10 
isr_err_stub	11 
isr_err_stub	12
isr_err_stub	13
isr_err_stub	14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub	17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub	30
isr_no_err_stub 31

extern irqHandler
%assign i 0
%rep	16
irq_stub i
%assign i i+1
%endrep

global idt_stub_table
idt_stub_table:
%assign i 0
%rep	32
	dd isr_stub_%+i
%assign i i+1
%endrep

%assign i 0
%rep	16
	dd irq_stub_%+i
%assign i i+1
%endrep
