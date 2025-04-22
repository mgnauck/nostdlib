#include "platform.h"

#ifdef __x86_64__

long long syscall(long long call, long long a, long long b, long long c,
                  long long d, long long e, long long f)
{
	long long res;
	__asm__ volatile (
		"movq %[d], %%r10  \n\t"
		"movq %[e], %%r8   \n\t"
		"movq %[f], %%r9   \n\t"
		"syscall           \n\t"
		: "=a" (res)
		: "0" (call),"D" (a),"S" (b), "d" (c),
		  [d] "g" (d), [e] "g" (e), [f] "g" (f)
		: "r11","rcx","r8","r10","r9","memory"
	);
	return res;
}

void *memset(void *dst, int c, size_t len)
{
	void *d = dst;
	__asm__ volatile (
		"rep stosb"
		: "+D"(dst), "+c"(len)
		: "a"(c)
		: "memory"
	);
	return d;
}

void *memcpy(void *restrict dst, const void *restrict src, size_t len)
{
	void *d = dst;
	__asm__ volatile (
		"rep movsb"
		: "+D"(dst), "+S"(src), "+c"(len)
		:
		: "memory"
	);
	return d;
}

int fetch_and_add(int *var, int val)
{
	__asm__ volatile (
		"lock; xaddl %0, %1"
		: "+r" (val), "+m" (*var)
		:
		: "memory"
	);
	return val;
}

#endif
