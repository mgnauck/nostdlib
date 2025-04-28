.POSIX:
OBJ = main.o platform.o platform_x86-64.o platform_AArch64.o printf.o
OUT = a.out
CC = clang
CFLAGS = -std=c99 -march=native -flto -O3 -Wall -Wextra -Wpedantic -Wshadow
CFLAGS += -fno-builtin -fno-stack-protector
#CFLAGS += -fdata-sections -Wa,--noexecstack -static -fdelete-null-pointer-checks
#CFLAGS += $(shell sdl2-config --cflags)
CFLAGS += -g
#CFLAGS += -fsanitize=undefined
#CFLAGS += -DNDEBUG
#LDLIBS = -lm $(shell sdl2-config --libs)
#LDFLAGS += -fsanitize=undefined
#LDFLAGS = -flto -O3 -s
LDFLAGS = -fuse-ld=lld
#LDFLAGS += -s
LDFLAGS += -nostdlib
# TODO strip -R .comment a.out

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -rf $(OUT) $(OBJ)

.PHONY: clean
