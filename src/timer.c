#include "timer.h"
#include "isr.h"
#include "io.h"

static int32_t tick;

static void timer_cb(regs_t *regs) {
	tick++;
}

static void timer_set_freq(int freq) {
	int divisor = 1193180 / freq;

	io_outb(0x43, 0x36);

	io_outb(0x40, divisor & 0xFF);
	io_outb(0x40, (divisor >> 8) & 0xFF);
}

void timer_init() {
	timer_set_freq(1000);
	isr_reg(IRQ0, timer_cb);
}
