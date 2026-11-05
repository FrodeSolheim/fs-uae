# Build the standalone SDL3 motion smoothness test. See motion-test.c for usage.

CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra
SDL3_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL3_LIBS := $(shell pkg-config --libs sdl3)

all: motion-test

motion-test: motion-test.c
	$(CC) $(CFLAGS) $(SDL3_CFLAGS) -o $@ $< $(SDL3_LIBS) -lm

run: motion-test
	./motion-test

clean:
	rm -f motion-test

.PHONY: all run clean
