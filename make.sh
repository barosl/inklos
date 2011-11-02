#!/bin/sh

nasm -f elf32 -o loader.o loader.s
gcc -m32 -c main.c
ld -melf_i386 -T linker.ld -o inklos.bin loader.o main.o
