#include "desc_tbls.h"
#include <stdint.h>
#include "mem.h"

extern void gdt_flush(uint32_t gdt_ptr);
extern void idt_flush(uint32_t gdt_ptr);
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

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

static void gdt_init() {
	gdt_ptr.limit = sizeof(gdt)-1;
	gdt_ptr.base = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdt_flush((uint32_t)&gdt_ptr);
}

typedef struct {
	uint16_t base_low;
	uint16_t sel;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed)) idt_ent_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

idt_ent_t idt[256];
idt_ptr_t idt_ptr;

static void idt_set_gate(int idx, uint32_t base, uint16_t sel, uint8_t flags) {
	idt[idx].base_low = base & 0xFFFF;
	idt[idx].base_high = (base >> 16) & 0xFFFF;

	idt[idx].sel = sel;

	idt[idx].always0 = 0;

	idt[idx].flags = flags/* | 0x60*/;
}

static void idt_init() {
	idt_ptr.limit = sizeof(idt)-1;
	idt_ptr.base = (uint32_t)&idt;

	memset(&idt, 0, sizeof(idt));

	static const void (*isrs[]) = {
		isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
		isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
		isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
		isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31};

	for (int i=0;i<sizeof(isrs)/sizeof(isrs[0]);i++) {
		idt_set_gate(i, (uint32_t)isrs[i], 0x08, 0x8E);
	}

	idt_flush((uint32_t)&idt_ptr);
}

void desc_tbls_init() {
	gdt_init();
	idt_init();
}
