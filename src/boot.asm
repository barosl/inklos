global _start

extern kmain

STACK_SIZE equ 0x4000

MULTIBOOT_PAGE_ALIGN equ 1 << 0
MULTIBOOT_MEM_INFO equ 1 << 1
MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEM_INFO
MULTIBOOT_HEADER_CHECKSUM equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

section .text

align 4
header:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_HEADER_CHECKSUM

_start:
	mov esp, stack
	push eax
	push ebx

	cli
	call kmain

	jmp $

	cli
	hlt

section .bss

align 4
	resb STACK_SIZE
stack:
