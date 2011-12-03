#include "vid.h"
#include "desc_tbls.h"
#include "isr.h"
#include "timer.h"
#include "kb.h"
#include "page.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

static void test_isr(regs_t *regs) {
	vid_puts("Interrupted: ");
	vid_write_dec(regs->int_no);
	vid_putc('\n');
}

void kmain(void *mb_inf, unsigned int magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) return;

	desc_tbls_init();
	vid_init();

	for (int i=0;i<16;i++) isr_reg(i, test_isr);

	vid_puts("Hello, world!\n");
	vid_puts("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n");

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	timer_init();
	kb_init();
	page_init();

	uint32_t *ptr = (uint32_t*)0x0FFFFD;
	uint32_t tmp = *ptr;
	(void)tmp;

	vid_puts("kmain() ended.\n");
}
