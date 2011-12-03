#include "vid.h"
#include "mem.h"
#include "io.h"
#include <stdint.h>
#include <stdbool.h>

#define SCREEN_W 80
#define SCREEN_H 25
#define DEFAULT_ATTR 0x07
#define BLANK_CH (' ' | (cur_attr << 8))

static int cur_x, cur_y;
static int cur_attr;
static volatile uint16_t *vid_mem = (uint16_t*)0xB8000;

static void vid_scroll() {
	if (cur_y < SCREEN_H) return;

	int lines = (cur_y - SCREEN_H) + 1;
	memcpy((uint16_t*)vid_mem, (uint16_t*)vid_mem + lines*SCREEN_W, (SCREEN_H-lines)*SCREEN_W*sizeof(*vid_mem));
	wmemset((uint16_t*)vid_mem + (SCREEN_H-lines)*SCREEN_W, BLANK_CH, SCREEN_W);

	cur_y = SCREEN_H - 1;
}

static void vid_update_cur() {
	uint16_t pos = cur_x + cur_y*SCREEN_W;

	io_outb(0x3D4, 0xE);
	io_outb(0x3D5, (pos >> 8) & 0xFF);
	io_outb(0x3D4, 0xF);
	io_outb(0x3D5, pos & 0xFF);
}

void vid_clear() {
	cur_x = cur_y = 0;
	cur_attr = DEFAULT_ATTR;

	wmemset((uint16_t*)vid_mem, BLANK_CH, SCREEN_W*SCREEN_H);

	vid_update_cur();
}

void vid_putc(char ch) {
	if (ch == '\r') {
		cur_x = 0;
	} else if (ch == '\n') {
		cur_x = 0;
		cur_y++;
	} else {
		vid_mem[cur_x + cur_y*SCREEN_W] = ch | (cur_attr << 8);
		cur_x++;
	}

	if (cur_x >= SCREEN_W) {
		cur_x = 0;
		cur_y++;
	}

	vid_scroll();
	vid_update_cur();
}

void vid_puts(char *text) {
	while (*text) vid_putc(*text++);
}

void vid_init() {
	vid_clear();
}

void vid_write_dec(unsigned int num) {
	char num_s[12];
	char *num_s_ptr = num_s;

	/*
	bool is_neg = false;
	if (num < 0) {
		is_neg = true;
		num = -num;
	}
	*/

	if (num) {
		while (num) {
			*num_s_ptr++ = '0' + num % 10;
			num /= 10;
		}
	} else {
		*num_s_ptr++ = '0';
	}

//	if (is_neg) vid_putc('-');

	while (--num_s_ptr >= num_s) vid_putc(*num_s_ptr);
}
