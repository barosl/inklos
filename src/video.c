#include "video.h"
#include "mem.h"
#include "io.h"
#include <stdint.h>
#include <stdbool.h>

#define SCREEN_W 80
#define SCREEN_H 25
#define DEFAULT_ATTR 0x07
#define BLANK_CH (' ' | (cur_attr << 8))

static int cur_x, cur_y;
static int cur_attr = DEFAULT_ATTR;
static uint16_t *video_mem = (uint16_t*)0xB8000;

static void video_scroll() {
	if (cur_y < SCREEN_H) return;

	int lines = (cur_y - SCREEN_H) + 1;
	memcpy(video_mem, video_mem + lines*SCREEN_W, (SCREEN_H-lines)*SCREEN_W*sizeof(*video_mem));
	wmemset(video_mem + (SCREEN_H-lines)*SCREEN_W, BLANK_CH, SCREEN_W);
}

static void video_update_cur() {
	uint16_t pos = cur_x + cur_y*SCREEN_W;

	io_outb(0x3D4, 0xE);
	io_outb(0x3D5, (pos >> 8) & 0xFF);
	io_outb(0x3D4, 0xF);
	io_outb(0x3D5, pos & 0xFF);
}

void video_clear() {
	cur_x = cur_y = 0;
	cur_attr = DEFAULT_ATTR;

	wmemset(video_mem, BLANK_CH, SCREEN_W*SCREEN_H);

	video_update_cur();
}

void video_putc(char ch) {
	if (ch == '\r') {
		cur_x = 0;
	} else if (ch == '\n') {
		cur_x = 0;
		cur_y++;
	} else {
		video_mem[cur_x + cur_y*SCREEN_W] = ch | (cur_attr << 8);
		cur_x++;
	}

	if (cur_x >= SCREEN_W) {
		cur_x = 0;
		cur_y++;
	}

	video_scroll();
	video_update_cur();
}

void video_puts(char *text) {
	while (*text) video_putc(*text++);
}

void video_init() {
	video_clear();
}

void video_write_dec(int num) {
	char num_s[sizeof(int)*8+2];
	char *num_s_ptr = num_s;

	bool is_neg = false;
	if (num < 0) {
		is_neg = true;
		num = -num;
	}

	while (num) {
		*num_s_ptr++ = '0' + num % 10;
		num /= 10;
	}

	if (is_neg) video_putc('-');
	while (--num_s_ptr >= num_s) video_putc(*num_s_ptr);
}
