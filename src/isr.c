#include "isr.h"
#include "io.h"

isr_t isrs[256];

void isr_handler(regs_t *regs) {
	if (isrs[regs->int_no]) isrs[regs->int_no](regs);
}

void irq_handler(regs_t *regs) {
	if (regs->int_no >= 32+8) io_outb(0xA0, 0x20);
	io_outb(0x20, 0x20);

	if (isrs[regs->int_no]) isrs[regs->int_no](regs);
}

void isr_reg(int no, isr_t isr) {
	isrs[no] = isr;
}
