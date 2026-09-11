CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../common

TARGETS = asview basebounds segment
 
.PHONY: all clean
 
all: $(TARGETS)

asview: asview.c
	$(CC) $(CFLAGS) asview.c -o asview

basebounds: basebounds.c ../common/utils.c
	$(CC) $(CFLAGS) basebounds.c ../common/utils.c -o basebounds

segment: segment.c ../common/utils.c
	$(CC) $(CFLAGS) segment.c ../common/utils.c -o segment

clean:
	rm -f $(TARGETS) translate