.PHONY: c clean

NAME = inklos

TARGET = $(NAME).bin
SRCS = $(wildcard *.[cs])
OBJS = $(subst .s,.o,$(SRCS:.c=.o))

$(TARGET): $(OBJS)
	ld -melf_i386 -T linker.ld -o $@ $+

entry.o: entry.s
	nasm -f elf32 -o entry.o entry.s

.c.o:
	gcc -m32 -c $< -std=gnu99

c: clean
clean:
	rm -f $(TARGET) $(OBJS)

r: run
run:
	qemu -kernel $(TARGET)
