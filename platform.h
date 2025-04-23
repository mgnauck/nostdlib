#ifndef PLATFORM_H
#define PLATFORM_H

#define INCLUDE_STRTOF

#define min(a, b)  ((a) < (b) ? (a) : (b))
#define max(a, b)  ((a) > (b) ? (a) : (b))

#define arrsz(a)  (sizeof(a) / sizeof(*(a)))

void                exit(int code);

void                *mmap(void *ptr, unsigned long long len, int prot,
                          int flags, int fd, unsigned long long ofs);
int                 unmap(void *ptr, unsigned long long len);

void                *malloc(unsigned long long len);
void                free(void *ptr);

void                *memset(void *dst, int c, unsigned long long len);
void                *memcpy(void *restrict dst, const void *restrict src,
                            unsigned long long len);

unsigned long long  strlen(const char *s);
int                 strcmp(const char *s1, const char *s2);
int                 strncmp(const char *s1, const char *s2,
                            unsigned long long n);
char                *strstr(const char *str, const char *sub);

long long           write(int fd, const void *buf, unsigned long long cnt);
void                _putchar(char character); // For external custom printf

int                 isdigit(int c);
int                 isspace(int c);

int                 atoi(const char *s);
#ifdef INCLUDE_STRTOF
float               strtof(const char *restrict s, char **restrict endptr);
float               atof(const char *s);
#endif

int                 fetch_and_add(int *var, int val);

#endif
