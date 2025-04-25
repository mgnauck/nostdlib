#include "platform.h"

#define SYS_CLONE      220

#define CLONE_VM       0x00000100 // Both processes share memory space
#define CLONE_FS       0x00000200 // Share filesystem information
#define CLONE_FILES    0x00000400 // Share file descriptor table
#define CLONE_SIGHAND  0x00000800 // Shared table of signal handlers
#define CLONE_PARENT   0x00008000 // Same parent as calling process
#define CLONE_THREAD   0x00010000 // Share threadgroup of calling process
#define CLONE_IO       0x80000000 // Share I/O context

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

#endif
