.POSIX:
OBJ = main.o syscalls.o platform.o platform_x86.o
OUT = a.out
CC = clang
CFLAGS = -std=c99 -march=native -flto -O3 -fno-builtin -fno-stack-protector
CFLAGS += -Wall -Wextra -Wpedantic
#CFLAGS += $(shell sdl2-config --cflags)
CFLAGS += -g -Wshadow
#CFLAGS += -fsanitize=undefined
#CFLAGS += -DNDEBUG
#LDLIBS = -lm $(shell sdl2-config --libs)
#LDFLAGS += -fsanitize=undefined
#LDFLAGS = -flto -O3 -s
LDFLAGS = -fuse-ld=lld
#LDFLAGS += -s
LDFLAGS += -nostdlib
#-mno-stack-arg-probe

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
.c.o:
	$(CC) $(CFLAGS) -c $<
.s.o:
	$(CC) -c $<

clean:
	rm -rf $(OUT) $(OBJ)

.PHONY: clean
