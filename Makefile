CC=gcc
CFLAGS=-Iinclude -Isrc -Wall  -Wextra -ggdb3

ODIR= ./build
LDIR = ./lib

LIBS=-lm

SRCS=$(wildcard  src/**/*.c  src/*.c)
OBJS=$(patsubst  %.c,%.o,$(SRCS))

all: snake

snake: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
		rm -f $(ODIR)/*.o *~ core snake
