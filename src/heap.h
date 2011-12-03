#pragma once

#include <stdint.h>

extern uint32_t heap_placement_addr;

extern uint32_t kmalloc(uint32_t size);
extern uint32_t kmalloc_a(uint32_t size);
extern uint32_t kmalloc_p(uint32_t size, uint32_t *phys);
extern uint32_t kmalloc_ap(uint32_t size, uint32_t *phys);
