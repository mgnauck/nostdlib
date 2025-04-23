#ifndef PLATFORM_H
#define PLATFORM_H

#include <sys/types.h> // size_t, off_t

#define min(a, b)  ((a) < (b) ? (a) : (b))
#define max(a, b)  ((a) > (b) ? (a) : (b))

#define arrsz(a)  (sizeof(a) / sizeof(*(a)))

void      exit(int code);

void      *mmap(void *ptr, size_t len, int prot, int flags,
                 int fd, off_t ofs);
int       unmap(void *ptr, size_t len);

void      *malloc(size_t len);
void      free(void *ptr);

void      *memset(void *dst, int c, size_t len);
void      *memcpy(void *restrict dst, const void *restrict src, size_t len);

size_t    strlen(const char *s);
int       strcmp(const char *s1, const char *s2);
int       strncmp(const char *s1, const char *s2, size_t n);
char      *strstr(const char *str, const char *sub);

int       isdigit(int c);
int       isspace(int c);

int       atoi(const char *s);
float     strtof(const char *restrict s, char **restrict endptr);
float     atof(const char *s);

int       fetch_and_add(int *var, int val);

#endif
