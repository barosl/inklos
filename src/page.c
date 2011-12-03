#include "page.h"
#include <stdint.h>
#include "sys.h"
#include "isr.h"
#include "heap.h"
#include "mem.h"

typedef struct {
	int present : 1;
	int rw : 1;
	int usr : 1;
	int accessed : 1;
	int dirty : 1;
	int unused : 7;
	int frm : 20;
} page_t;

typedef struct {
	page_t pages[1024];
} page_tbl_t;

typedef struct {
	page_tbl_t *tbls[1024];
	uint32_t tbls_phys[1024];
	uint32_t phys_addr;
} page_dir_t;

static uint32_t *frm_bits;
static uint32_t frm_cnt;

static page_dir_t *page_krnl_dir;
static page_dir_t *page_cur_dir;

#define FRM_SET(frm_no) (frm_bits[(frm_no)/32] |= (1 << ((frm_no)%32)))
#define FRM_CLR(frm_no) (frm_bits[(frm_no)/32] &= ~(1 << ((frm_no)%32)))
#define FRM_TEST(frm_no) (frm_bits[(frm_no)/32] & (1 << ((frm_no)%32)))

static uint32_t find_first_frm() {
	uint32_t frm_bit_cnt = frm_cnt/32;
	for (int i=0;i<frm_bit_cnt;i++) {
		if (frm_bits[i] == 0xFFFFFFFF) continue;

		uint32_t mask = 1;
		for (int j=0;j<32;j++) {
			if (!(frm_bits[i] & mask)) return i*32 + j;
			mask <<= 1;
		}
	}

	return -1;
}

static void frm_alloc(page_t *page, int is_krnl, int is_rd_only) {
	if (page->frm) return;

	uint32_t frm_no = find_first_frm();
	if (frm_no == -1) sys_panic("No free frames.");

	FRM_SET(frm_no);

	page->present = 1;
	page->rw = is_rd_only ? 0 : 1;
	page->usr = is_krnl ? 0 : 1;
	page->frm = frm_no;
}

/*
static void frm_free(page_t *page) {
	if (!page->frm) return;

	FRM_CLR(page->frm);
	page->frm = 0;
}
*/

static void page_fault(regs_t *regs) {
	sys_panic("Page fault.");
}

static page_t *page_get(uint32_t addr, int creat, page_dir_t *dir) {
	uint32_t frm_no = addr >> 12;
	uint32_t tbl_idx = frm_no/1024;

	if (!dir->tbls[tbl_idx]) {
		if (!creat) return 0;

		dir->tbls[tbl_idx] = (page_tbl_t*)kmalloc_a(sizeof(*dir->tbls[tbl_idx]));

		memset(dir->tbls[tbl_idx], 0, sizeof(*dir->tbls[tbl_idx]));

		dir->tbls_phys[tbl_idx] = (uint32_t)dir->tbls[tbl_idx] | 0x7;
	}

	return &dir->tbls[tbl_idx]->pages[frm_no%1024];
}

static void page_switch_dir(page_dir_t *dir) {
	page_cur_dir = dir;

	asm volatile("mov %0, %%cr3" : : "r"(dir->tbls_phys));

	uint32_t cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));

	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

void page_init() {
	uint32_t mem_size = 128*1024*1024; /* FIXME */

	frm_cnt = mem_size >> 12;

	frm_bits = (uint32_t*)kmalloc(sizeof(uint32_t)*(frm_cnt/32));
	memset(frm_bits, 0, sizeof(uint32_t)*(frm_cnt/32));

	page_krnl_dir = (page_dir_t*)kmalloc_a(sizeof(*page_krnl_dir));
	memset(page_krnl_dir, 0, sizeof(*page_krnl_dir));

	uint32_t addr = 0;
	while (addr < heap_placement_addr) {
		frm_alloc(page_get(addr, 1, page_krnl_dir), 0, 0);
		addr += 0x1000;
	}

	isr_reg(14, page_fault);

	page_switch_dir(page_krnl_dir);
}
