#include "kb.h"
#include "isr.h"
#include "io.h"

char kb_us[] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
};

#include "video.h"
void kb_handler(regs_t *regs) {
	uint8_t scan_code = io_inb(0x60);

	if (!(scan_code & 0x80)) {
		video_putc(kb_us[scan_code]);
	}
}

void kb_init() {
	isr_reg(IRQ1, kb_handler);
}
