#include "mem.h"

void *memcpy(void *dst, void *src, unsigned int len) {
	for (int i=0;i<len;i++) ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
	return dst;
}

void *memset(void *ptr, int val, unsigned int len) {
	for (int i=0;i<len;i++) ((unsigned char*)ptr)[i] = val;
}

void *wmemset(void *ptr, int val, unsigned int len) {
	for (int i=0;i<len;i++) ((unsigned short*)ptr)[i] = val;
}
