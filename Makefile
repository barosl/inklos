.PHONY: c clean
.SUFFIXES: .asm

include config.mk

CFLAGS = -std=gnu99 -Wall -Wc++-compat -fno-stack-protector

TARGET = $(NAME).bin
SRCS = $(wildcard *.c) $(wildcard *.asm)
OBJS = $(subst .asm,.o,$(SRCS:.c=.o))

$(TARGET): $(OBJS)
	ld -melf_i386 -T linker.ld -o $@ $+

.asm.o:
	nasm -f elf32 $<

.c.o:
	gcc -m32 -c $< $(CFLAGS)

c: clean
clean:
	rm -f $(TARGET) $(OBJS)

r: run
run: $(TARGET)
	qemu -kernel $(TARGET)
