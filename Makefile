.PHONY: c clean
.SUFFIXES: .asm

include config.mk

CFLAGS = -std=gnu99 -Wall -Wc++-compat -fno-stack-protector -Werror

SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/$(NAME).bin
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.asm)
OBJS = $(subst .asm,.o,$(SRCS:.c=.o))

$(TARGET): $(OBJS)
	ld -melf_i386 -T $(SRC_DIR)/linker.ld -o $@ $+

.asm.o:
	nasm -f elf32 -o $@ $<

.c.o %.o: %.c %.h
	gcc -m32 -c -o $@ $< $(CFLAGS)

c: clean
clean:
	rm -f $(TARGET) $(OBJS)

r: run
run: $(TARGET)
	qemu -kernel $(TARGET)
