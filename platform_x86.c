#include "platform.h"

#ifdef __x86_64__

void *_memset(void *dst, int c, size_t len)
{
	void *d = dst;
	__asm__ volatile ("rep stosb"
		: "+D"(dst), "+c"(len)
		: "a"(c)
		: "memory"
	);
	return d;
}

void *_memcpy(void *restrict dst, void *restrict src, size_t len)
{
	void *d = dst;
	__asm__ volatile ("rep movsb"
		: "+D"(dst), "+S"(src), "+c"(len)
		:
		: "memory"
	);
	return d;
}

uint32_t fetch_and_add(uint32_t *var, uint32_t val)
{
	__asm__ volatile ("lock; xaddl %0, %1"
		: "+r" (val), "+m" (*var)
		:
		: "memory"
	);
	return val;
}

#endif
