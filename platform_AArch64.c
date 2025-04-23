#include "platform.h"

#ifdef __aarch64__

long long syscall(long long call, long long a, long long b, long long c,
                  long long d, long long e, long long f)
{
	// TODO
	return 0;
}

int fetch_and_add(int *var, int val)
{
	// TODO
	return val;
}

#endif
