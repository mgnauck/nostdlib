#include <stdbool.h>
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

// strtof taken LibCL. Does not handle nan or inf.
// https://github.com/ochafik/LibCL/blob/master/src/main/resources/LibCL/strtof.c

// Largest possible base 10 exponent. Any exponent larger than this will
// already produce underflow or overflow, so there's no need to worry about
// additional digits.
const int maxExponent = 38;

// Table giving binary powers of 10. Entry is 10^2^i. Used to convert decimal
// exponents into floating-point numbers.
const float _strtof_powersOf10_[] = {
  10.0f, 100.0f, 1.0e4f, 1.0e8f, 1.0e16f, 1.0e32f};

// A decimal ASCII floating-point number, optionally preceded by white space.
// Must have form "-I.FE-X", where I is the integer part of the mantissa, F is
// the fractional part of the mantissa, and X is the exponent. Either of the
// signs may be "+", "-", or omitted. Either I or F may be omitted, or both.
// The decimal point isn't necessary unless F is present. The "E" may actually
// be an "e".  E and X may both be omitted (but not just one).
float strtof(const char *restrict str, char **restrict endPtr)
{
	int sign, expSign = false;
	float fraction, dblExp;
	const float* d;
	const char* p;
	int c;
	int exp = 0; // Exponent read from "EX" field.

	// Exponent that derives from the fractional part. Under normal
	// circumstatnces, it is the negative of the number of digits in F.
	// However, if I is very long, the last digits of I get dropped
	// (otherwise a long I with a large negative exponent could cause an
	// unnecessary overflow on I alone). In this case, fracExp is
	// incremented one for each dropped digit.
	int fracExp = 0;

	int mantSize; // Number of digits in mantissa.
	int decPt; // Number of mantissa digits BEFORE decimal point.
	const char* pExp; // Temporarily holds location of exponent in str.

	// Strip off leading blanks and check for a sign.
	p = str;
	while (isspace(*p))
	{
		p += 1;
	}

	if (*p == '-')
	{
		sign = true;
		p += 1;
	}
	else
	{
		if (*p == '+')
		{
			p += 1;
		}
		sign = false;
	}

	// Count the number of digits in the mantissa (including the decimal
	// point), and also locate the decimal point.
	decPt = -1;
	for (mantSize = 0;; mantSize += 1)
	{
		c = *p;
		if (!isdigit(c))
		{
			if ((c != '.') || (decPt >= 0))
			{
				break;
			}
			decPt = mantSize;
		}
		p += 1;
	}

	// Now suck up the digits in the mantissa.  Use two integers to
	// collect 9 digits each (this is faster than using floating-point).
	// If the mantissa has more than 18 digits, ignore the extras, since
	// they can't affect the value anyway.
	pExp = p;
	p -= mantSize;
	if (decPt < 0)
	{
		decPt = mantSize;
	}
	else
	{
		mantSize -= 1; // One of the digits was the point.
	}

	if (mantSize > 18)
	{
		fracExp = decPt - 18;
		mantSize = 18;
	}
	else
	{
		fracExp = decPt - mantSize;
	}

	if (mantSize == 0)
	{
		fraction = 0.0;
		p = str;
		goto done;
	}
	else
	{
		int frac1, frac2;
		frac1 = 0;
		for (; mantSize > 9; mantSize -= 1)
		{
			c = *p;
			p += 1;
			if(c == '.')
			{
				c = *p;
				p += 1;
			}
			frac1 = 10 * frac1 + (c - '0');
		}

		frac2 = 0;
		for (; mantSize > 0; mantSize -= 1)
		{
			c = *p;
			p += 1;
			if (c == '.')
			{
				c = *p;
				p += 1;
			}
			frac2 = 10 * frac2 + (c - '0');
		}
		fraction = (1.0e9f * frac1) + frac2;
	}

	// Skim off the exponent.
	p = pExp;
	if ((*p == 'E') || (*p == 'e'))
	{
		p += 1;
		if (*p == '-')
		{
			expSign = true;
			p += 1;
		}
		else
		{
			if (*p == '+')
			{
				p += 1;
			}
			expSign = false;
		}

		while (isdigit(*p))
		{
			exp = exp * 10 + (*p - '0');
			p += 1;
		}
	}

	if (expSign)
	{
		exp = fracExp - exp;
	}
	else
	{
		exp = fracExp + exp;
	}

	// Generate a floating-point number that represents the exponent.
	// Do this by processing the exponent one bit at a time to combine
	// many powers of 2 of 10. Then combine the exponent with the fraction.
	if (exp < 0)
	{
		expSign = true;
		exp = -exp;
	}
	else
	{
		expSign = false;
	}

	if (exp > maxExponent)
	{
		exp = maxExponent;
	}

	dblExp = 1.0f;

	for (d = _strtof_powersOf10_; exp != 0; exp >>= 1, d += 1)
	{
		if(exp & 01)
		{
			dblExp *= *d;
		}
	}

	if (expSign)
	{
		fraction /= dblExp;
	}
	else
	{
		fraction *= dblExp;
	}

done:
	if (endPtr)
	{
		*endPtr = (char *)p;
	}

	if (sign)
	{
		return -fraction;
	}

	return fraction;
}

float atof(const char *str)
{
	return strtof(str, 0);
}
