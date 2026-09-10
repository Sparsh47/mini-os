CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../common

TARGETS = asview basebounds
 
.PHONY: all clean
 
all: $(TARGETS)

asview: asview.c
	$(CC) $(CFLAGS) asview.c -o asview

basebounds: basebounds.c ../common/utils.c
	$(CC) $(CFLAGS) basebounds.c ../common/utils.c -o basebounds

clean:
	rm -f $(TARGETS) translate