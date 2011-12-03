#include "heap.h"

extern int krnl_end;

uint32_t heap_placement_addr = (uint32_t)&krnl_end;

uint32_t kmalloc_real(uint32_t size, int align, uint32_t *phys) {
	if (align && (heap_placement_addr & 0x0FFF)) {
		heap_placement_addr = (heap_placement_addr & 0xFFFFF000) + 0x1000;
	}

	uint32_t res = heap_placement_addr;
	heap_placement_addr += size;

	if (phys) *phys = res;

	return res;
}

uint32_t kmalloc(uint32_t size) {
	return kmalloc_real(size, 0, 0);
}

uint32_t kmalloc_a(uint32_t size) {
	return kmalloc_real(size, 1, 0);
}

uint32_t kmalloc_p(uint32_t size, uint32_t *phys) {
	return kmalloc_real(size, 0, phys);
}

uint32_t kmalloc_ap(uint32_t size, uint32_t *phys) {
	return kmalloc_real(size, 1, phys);
}
