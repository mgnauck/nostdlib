#include "platform.h"

#ifdef __x86_64__

long long syscall(long long nr, long long a, long long b, long long c,
                  long long d, long long e, long long f)
{
	long long res;
	register long long r10 __asm("r10") = d;
	register long long r8 __asm("r8") = e;
	register long long r9 __asm("r9") = f;
	__asm volatile (
		"syscall"
		: "=a" (res)
		: "a"(nr), "D"(a), "S"(b), "d"(c), "r"(r10), "r"(r8), "r"(r9) 
		: "rcx", "r11", "cc", "memory"
	);
	return res;
}

#endif
