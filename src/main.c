#include "video.h"
#include "desc_tbls.h"
#include "isr.h"
#include "timer.h"
#include "kb.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

static void test_isr(regs_t *regs) {
	video_puts("Interrupted: ");
	video_write_dec(regs->int_no);
	video_putc('\n');
}

void kmain(void *mb_inf, unsigned int magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) return;

	desc_tbls_init();
	video_init();

	for (int i=0;i<16;i++) isr_reg(i, test_isr);

	video_puts("Hello, world!\n");
	video_puts("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n");

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	timer_init();
	kb_init();
}
