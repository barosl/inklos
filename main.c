#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

void kmain(void *mb_inf, unsigned int magic) {
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		return;
	}

	volatile unsigned char *vid = (unsigned char*)0xB8000;
	vid[0] = 'A';
	vid[1] = 0x07;
	vid[2] = 'B';
	vid[3] = 0x1F;
	vid[4] = 'C';
	vid[5] = 0x2A;
}
