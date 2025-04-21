#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

#include "platform.h"

extern uint64_t syscall(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
  uint64_t, uint64_t);

void _exit(int code)
{
	syscall(60, code, 0, 0, 0, 0, 0);
}

void *_mmap(void *ptr, size_t len, int prot, int flags, int fd, off_t ofs)
{
	return (void *)syscall(9, (uint64_t)ptr, len, prot, flags, fd, ofs);
}

int _unmap(void *ptr, size_t len)
{
	return syscall(11, (long)ptr, len, 0, 0, 0, 0);
}

void *_malloc(size_t len)
{
	len += sizeof(len);
	void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE,
	  MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	if ((ptrdiff_t)ptr < 0)
		_exit(1);
	_memcpy(ptr, &len, sizeof(len));
	return (void *)((ptrdiff_t)ptr + (ptrdiff_t)sizeof(len));
}

void _free(void *ptr)
{
	size_t len;
	ptr = (void *)((ptrdiff_t)ptr - (ptrdiff_t)sizeof(len));
	_memcpy(&len, ptr, sizeof(len));
	if (munmap(ptr, len) < 0)
		_exit(1);
}

#ifndef __x86_64__

void *_memset(void *dst, int c, size_t len)
{
	unsigned char *d = dst;
	for (; len; len--, d++)
		*d = c;
	return dst;
}

void *_memcpy(void *restrict dst, void *restrict src, size_t len)
{
	unsigned char *d = dst;
	unsigned char *s = src;
	for (; len; len--, d++, s++)
		*d = *s;
	return dst;
}

#endif

size_t strlen(const char *p)
{
	size_t len = 0;
	while (*p++)
		len++;
	return len;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return 0;
	return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		int d = (unsigned char)s1[i] - (unsigned char)s2[i];
		if (d != 0 || s1[i] == '\0')
			return d;
	}
	return 0;
}

char *strstr(const char *str, const char *sub)
{
	size_t l = strlen(sub);
	char *p = (char *)sub;
	while (*str && *p) {
		if (*str++ == *p)
			p++;
		if (!*p)
			return (char *)str - l;
		if ((ptrdiff_t)l == p - sub)
			p = (char *)sub;
	}
	return NULL;
}
