#pragma once

extern void video_init(void);
extern void video_clear(void);
extern void video_putc(char ch);
extern void video_puts(char *text);
extern void video_write_dec(int num);
