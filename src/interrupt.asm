extern isr_handler, irq_handler

%macro ISR_NO_ERR_CODE 1
global isr%1
isr%1:
	cli
	push 0 ; dummy error code
	push %1
	jmp isr_comm
%endmacro

%macro ISR_ERR_CODE 1
global isr%1
isr%1:
	cli
	push %1
	jmp isr_comm
%endmacro

%macro IRQ 2
global irq%1
irq%1:
	cli
	push 0
	push %2
	jmp irq_comm
%endmacro

ISR_NO_ERR_CODE 0
ISR_NO_ERR_CODE 1
ISR_NO_ERR_CODE 2
ISR_NO_ERR_CODE 3
ISR_NO_ERR_CODE 4
ISR_NO_ERR_CODE 5
ISR_NO_ERR_CODE 6
ISR_NO_ERR_CODE 7
ISR_ERR_CODE 8
ISR_NO_ERR_CODE 9
ISR_ERR_CODE 10
ISR_ERR_CODE 11
ISR_ERR_CODE 12
ISR_ERR_CODE 13
ISR_ERR_CODE 14
ISR_NO_ERR_CODE 15
ISR_NO_ERR_CODE 16
ISR_NO_ERR_CODE 17
ISR_NO_ERR_CODE 18
ISR_NO_ERR_CODE 19
ISR_NO_ERR_CODE 20
ISR_NO_ERR_CODE 21
ISR_NO_ERR_CODE 22
ISR_NO_ERR_CODE 23
ISR_NO_ERR_CODE 24
ISR_NO_ERR_CODE 25
ISR_NO_ERR_CODE 26
ISR_NO_ERR_CODE 27
ISR_NO_ERR_CODE 28
ISR_NO_ERR_CODE 29
ISR_NO_ERR_CODE 30
ISR_NO_ERR_CODE 31

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

%macro COMM_HEADER 0
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
%endmacro

%macro COMM_FOOTER 0
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa

	add esp, 8
	sti
	iret
%endmacro

isr_comm:
	COMM_HEADER
	call isr_handler
	COMM_FOOTER

irq_comm:
	COMM_HEADER
	call irq_handler
	COMM_FOOTER
