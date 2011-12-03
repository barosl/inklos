#pragma once

#include <stdint.h>

static inline void io_outb(uint16_t port, uint8_t data) {
	asm volatile("outb %1, %0" : : "dN"(port), "a"(data));
}

static inline void io_outw(uint16_t port, uint16_t data) {
	asm volatile("outw %1, %0" : : "dN"(port), "a"(data));
}

static inline void io_outl(uint16_t port, uint32_t data) {
	asm volatile("outl %1, %0" : : "dN"(port), "a"(data));
}

static inline uint8_t io_inb(uint16_t port) {
	uint8_t data;
	asm volatile("inb %1, %0" : "=a"(data) : "dN"(port));
	return data;
}

static inline uint16_t io_inw(uint16_t port) {
	uint16_t data;
	asm volatile("inw %1, %0" : "=a"(data) : "dN"(port));
	return data;
}

static inline uint32_t io_inl(uint16_t port) {
	uint32_t data;
	asm volatile("inl %1, %0" : "=a"(data) : "dN"(port));
	return data;
}
