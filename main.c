#include "video.h"
#include "desc_tbls.h"

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

void kmain(void *mb_inf, unsigned int magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		return;
	}

	desc_tbls_init();

	video_init();
	video_puts("Hello, world!\n");
	video_puts("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n");

	asm volatile("int $0x3");
	asm volatile("int $0x4");
}
