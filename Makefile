.POSIX:
OBJ = main.o platform.o platform_x64.o platform_AArch64.o printf.o
OUT = a.out
CC = clang
CFLAGS = -std=c99 -march=native -flto -O3 -Wall -Wextra -Wpedantic -Wshadow
CFLAGS += -fno-builtin -fno-stack-protector
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

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -rf $(OUT) $(OBJ)

.PHONY: clean
