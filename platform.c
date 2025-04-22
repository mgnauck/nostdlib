#include <sys/mman.h>

#include "platform.h"

extern long long syscall(long long call, long long a, long long b, long long c,
                         long long d, long long e, long long f);

void exit(int code)
{
	syscall(60, code, 0, 0, 0, 0, 0);
}

void *mmap(void *ptr, size_t len, int prot, int flags, int fd, off_t ofs)
{
	return (void *)syscall(9, (long long)ptr, len, prot, flags, fd, ofs);
}

int unmap(void *ptr, size_t len)
{
	return syscall(11, (long long)ptr, len, 0, 0, 0, 0);
}

void *malloc(size_t len)
{
	len += sizeof(len);
	void *ptr = mmap(0, len, PROT_READ | PROT_WRITE,
	  MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	if ((long long)ptr < 0)
		exit(1);
	memcpy(ptr, &len, sizeof(len));
	return (void *)((unsigned long long)ptr +
	  (unsigned long long)sizeof(len));
}

void free(void *ptr)
{
	size_t len;
	ptr = (void *)((unsigned long long)ptr -
	  (unsigned long long)sizeof(len));
	memcpy(&len, ptr, sizeof(len));
	if (munmap(ptr, len) < 0)
		exit(1);
}

#ifndef __x86_64__

void *memset(void *dst, int c, size_t len)
{
	if (!len)
		return dest;
	unsigned char *d = dst;
	for (; len; len--, d++)
		*d = c;
	return dst;
}

void *memcpy(void *restrict dst, const void *restrict src, size_t len)
{
	unsigned char *d = dst;
	const unsigned char *s = src;
	for (; len; len--, d++, s++)
		*d = *s;
	return dst;
}

#endif

size_t strlen(const char *s)
{
	const char *o = s;
	for (; *s; s++);
	return s - o;
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
		if ((long long)l == p - sub)
			p = (char *)sub;
	}
	return 0;
}

int isdigit(int c)
{
	return (unsigned int)c - '0' < 10;
}

int isspace(int c)
{
	return c == ' ' || (unsigned int)c - '\t' < 5;
}

int atoi(const char *s)
{
	int neg = 0;
	int n = 0;

	while (isspace(*s))
		s++;

	switch (*s) {
		case '-': neg = 1;
		case '+': s++;
	}

	// Compute as negative number avoiding overflow at INT_MIN
	while (isdigit(*s))
		n = 10 * n - (*s++ - '0');

	return neg ? n : -n;
}
