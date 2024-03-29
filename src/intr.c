#include "intr.h"
#include <stdint.h>
#include "mem.h"
#include "io.h"

extern void isr0(); extern void isr1(); extern void isr2(); extern void isr3(); extern void isr4(); extern void isr5(); extern void isr6(); extern void isr7();
extern void isr8(); extern void isr9(); extern void isr10(); extern void isr11(); extern void isr12(); extern void isr13(); extern void isr14(); extern void isr15();
extern void isr16(); extern void isr17(); extern void isr18(); extern void isr19(); extern void isr20(); extern void isr21(); extern void isr22(); extern void isr23();
extern void isr24(); extern void isr25(); extern void isr26(); extern void isr27(); extern void isr28(); extern void isr29(); extern void isr30(); extern void isr31();

extern void irq0(); extern void irq1(); extern void irq2(); extern void irq3(); extern void irq4(); extern void irq5(); extern void irq6(); extern void irq7();
extern void irq8(); extern void irq9(); extern void irq10(); extern void irq11(); extern void irq12(); extern void irq13(); extern void irq14(); extern void irq15();

static const void (*isrs[]) = {
	isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
	isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
	isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
	isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,

	irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7,
	irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15,
};

extern void idt_flush(uint32_t gdt_ptr);

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

void intr_init() {
	idt_ptr.limit = sizeof(idt)-1;
	idt_ptr.base = (uint32_t)&idt;

	memset(&idt, 0, sizeof(idt));

	io_outb(0x20, 0x11);
	io_outb(0xA0, 0x11);
	io_outb(0x21, 0x20);
	io_outb(0xA1, 0x28);
	io_outb(0x21, 0x04);
	io_outb(0xA1, 0x02);
	io_outb(0x21, 0x01);
	io_outb(0xA1, 0x01);
	io_outb(0x21, 0x0);
	io_outb(0xA1, 0x0);

	for (int i=0;i<sizeof(isrs)/sizeof(isrs[0]);i++) {
		idt_set_gate(i, (uint32_t)isrs[i], 0x08, 0x8E);
	}

	idt_flush((uint32_t)&idt_ptr);

	asm volatile("sti");
}
