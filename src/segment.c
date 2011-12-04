#include "segment.h"
#include <stdint.h>

extern void gdt_flush(uint32_t gdt_ptr);

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t gran;
	uint8_t base_high;
} __attribute__((packed)) gdt_ent_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

gdt_ent_t gdt[5];
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[idx].base_low = base & 0xFFFF;
	gdt[idx].base_mid = (base >> 16) & 0xFF;
	gdt[idx].base_high = (base >> 24) & 0xFF;

	gdt[idx].limit_low = limit & 0xFFFF;
	gdt[idx].gran = (limit >> 16) & 0x0F;

	gdt[idx].gran |= gran & 0xF0;
	gdt[idx].access = access;
}

void segment_init() {
	gdt_ptr.limit = sizeof(gdt)-1;
	gdt_ptr.base = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((uint32_t)&gdt_ptr);
}
