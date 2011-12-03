#pragma once

#include <stdint.h>

static inline void io_outb(uint16_t port, uint8_t data) {
	asm volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

static inline uint8_t io_inb(uint16_t port) {
	uint8_t data;
	asm volatile("inb %1, %0" : "=a"(data) : "dN"(port));
	return data;
}
