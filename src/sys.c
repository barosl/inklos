#include "sys.h"
#include "vid.h"

void sys_panic(char *msg) {
	asm volatile("cli");

	vid_puts("Kernel panic: ");
	vid_puts(msg);
	vid_putc('\n');

	while (1);
}
