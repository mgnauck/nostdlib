.POSIX:
OBJ = main.o platform.o platform_x86-64.o platform_AArch64.o printf.o
OUT = a.out
CC = clang
CFLAGS = -std=c99 -march=native -Oz -Wall -Wextra -Wpedantic -Wshadow
CFLAGS += -flto
CFLAGS += -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables
#CFLAGS += -g
#CFLAGS += -fsanitize=undefined
CFLAGS += -DNDEBUG
#LDLIBS = -lm
#LDFLAGS += -fsanitize=undefined
#LDFLAGS = -flto -Oz -s
LDFLAGS = -fuse-ld=lld
LDFLAGS += -s
LDFLAGS += -nostdlib
# TODO strip -R .comment a.out

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -rf $(OUT) $(OBJ)

.PHONY: clean
