#include "platform.h"

#ifdef __aarch64__

long long syscall(long long nr, long long a, long long b, long long c,
                  long long d, long long e, long long f)
{
	register long long x8 __asm("x8") = nr;
	register long long x0 __asm("x0") = a;
	register long long x1 __asm("x1") = b;
	register long long x2 __asm("x2") = c;
	register long long x3 __asm("x3") = d;
	register long long x4 __asm("x4") = e;
	register long long x5 __asm("x5") = f;
	__asm volatile (
		"svc #0"
		: "=r" (x0)
		: "r"(x8), "r"(x0), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5)
		: "memory"
	);
	return x0;
}

int fetch_and_add(int *var, int val)
{
	// TODO
	return val;
}

#endif
