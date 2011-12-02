#!/bin/sh

nasm -f elf32 -o entry.o entry.s
gcc -m32 -c main.c -std=gnu99
gcc -m32 -c video.c -std=gnu99
gcc -m32 -c mem.c -std=gnu99
gcc -m32 -c io.c -std=gnu99
ld -melf_i386 -T linker.ld -o inklos.bin entry.o main.o video.o mem.o io.o
