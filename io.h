#pragma once

static inline void io_outb(unsigned short port, unsigned char data) {
	asm volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

static inline unsigned char io_inb(unsigned short port) {
	unsigned char data;
	asm volatile("inb %1, %0" : "=a"(data) : "dN"(port));
	return data;
}
