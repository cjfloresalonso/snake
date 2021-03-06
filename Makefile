CC=gcc
CFLAGS=-Iinclude -Isrc -Wall  -Wextra -ggdb3

ODIR= ./bin
BDIR= ./build
LDIR = ./lib

LIBS=-lm -lncurses

SRCS=$(wildcard  src/**/*.c  src/*.c)
OBJS=$(patsubst  %.c,%.o,$(SRCS))

all: snake

snake: $(OBJS)
	mkdir -p $(ODIR) 
	$(CC) -o $(ODIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/* $(BDIR)/* *~ core snake
