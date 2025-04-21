#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <sys/types.h> // size_t, off_t

#define min(a, b)  ((a) < (b) ? (a) : (b))
#define max(a, b)  ((a) > (b) ? (a) : (b))

#define cntof(a)  (ptrdiff_t)(sizeof(a) / sizeof(*(a)))
#define lenof(a)  (cntof(a) - 1)

#define assert(c)  while (!(c)) __builtin_unreachable()

void      _exit(int code);

void      *_mmap(void *ptr, size_t len, int prot, int flags,
                 int fd, off_t ofs);
int       _unmap(void *ptr, size_t len);

void      *_malloc(size_t len);
void      _free(void *ptr);

void      *_memset(void *dst, int c, size_t len);
void      *_memcpy(void *restrict dst, const void *restrict src, size_t len);

size_t    strlen(const char *s);
int       strcmp(const char *s1, const char *s2);
int       strncmp(const char *s1, const char *s2, size_t n);
char      *strstr(const char *str, const char *sub);

int       isdigit(int c);
int       isspace(int c);

// TODO
int       atoi(const char *s);
float     strtof(const char *restrict nptr, char **restrict endptr);

uint32_t  fetch_and_add(uint32_t *var, uint32_t val);

#endif
