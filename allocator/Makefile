CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../common

.PHONY: all clean

all: mymalloc

mymalloc: mymalloc.c policies.c mymalloc.h policies.h
	$(CC) $(CFLAGS) mymalloc.c policies.c -o mymalloc

clean:
	rm -f mymalloc