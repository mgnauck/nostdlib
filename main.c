#include <stdbool.h>

#include "platform.h"
#include "printf.h"

void *mapfile_read(const char *relpathname, unsigned long long *size)
{
	int fd = openat(AT_FDCWD, relpathname, O_RDONLY);
	if (fd < 0)
		exit(1);

	struct stat st = {0};
	if (fstatat(AT_FDCWD, relpathname, &st, 0) < 0)
		exit(1);

	void *buf = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
	  fd, 0);
	if ((long long)buf < 0)
		exit(1);

	if (close(fd) < 0)
		exit(1);

	*size = st.st_size;
	return buf;
}

// From man futex
void fwait(unsigned int *fut)
{
	while (1) {
		// Check if futex is available (compare to 1)
		unsigned int one = 1; // On false, will become value of *futexp
		if (__atomic_compare_exchange_n(fut, &one, 0, false,
		  __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
			break;
		// Futex unavailable, so wait
		int s = futex(fut, FUTEX_WAIT, 0, NULL, NULL, 0);
		if (s == -1 && s != -11 /* -EAGAIN */)
			exit(1);
	}
}

void fpost(unsigned int *fut)
{
	unsigned int zero = 0;
	if (__atomic_compare_exchange_n(fut, &zero, 1, false,
	  __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
		// Wake any futex waiters
		int s = futex(fut, FUTEX_WAKE, 1, NULL, NULL, 0);
		if (s < 0 && s >= -4095)
			exit(1);
	}
}


#define THRDCNT  3
#define TASKCNT  30

unsigned int *faddr, taskcnt, fincnt;

void thrdfn(void *p)
{
	unsigned int num = *(unsigned int *)p;

	while (1) {
		unsigned int cnt = __atomic_fetch_add(&taskcnt, 1,
		  __ATOMIC_SEQ_CST);
		if (cnt >= TASKCNT)
			break;

		// printf is not thread safe
		//printf("%d", cnt); // task num
		printf("%d", num); // child num
	}

	if (__atomic_add_fetch(&fincnt, 1, __ATOMIC_SEQ_CST) == THRDCNT)
		fpost(faddr);

	exit(0);
}

void _start(void)
{
	// Test: Read Makefile
	unsigned long long sz = 0;
	void *buf = mapfile_read("Makefile", &sz);

	write(STDOUT_FILENO, buf, sz);

	if (munmap(buf, sz) < 0)
		exit(1);

	// Test: Threading
	faddr = mmap(NULL, sizeof(unsigned int), PROT_READ | PROT_WRITE,
	  MAP_ANONYMOUS | MAP_SHARED, -1, 0);
	if ((long long)faddr < 0)
		exit(1);

	void *stacks[THRDCNT];
	int cnums[THRDCNT];
	for (unsigned int i = 0; i < THRDCNT; i++) {
		stacks[i] = create_stack(1024);
		cnums[i] = i;
	}

	for (unsigned int j = 0; j < 5; j++) {
		*faddr = 0;
		taskcnt = 0;
		fincnt = 0;

		// Spawn
		for (unsigned int i = 0; i < THRDCNT; i++)
			create_thread(thrdfn, &cnums[i], stacks[i]);
		// Join
		fwait(faddr);
		printf("\nparent continues after iteration %d\n", j);
	}

	for (unsigned int i = 0; i < THRDCNT; i++)
		release_stack(stacks[i], 1024);

	munmap(faddr, sizeof(unsigned int));

	exit(0);
}
