#include "isr.h"
#include <stdint.h>
#include "video.h"

typedef struct {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, user_esp, ss;
} regs_t;

void isr_handler(regs_t regs) {
	video_puts("Interrupted: ");
	video_write_dec(regs.int_no);
	video_putc('\n');
}
