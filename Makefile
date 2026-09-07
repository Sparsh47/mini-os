CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Icommon

MODULES = allocator translate paging tlb replace shell fs net

.PHONY: all clean

all:
	@echo "MiniOs build"
	@echo "Modules: $(MODULES)"

clean:
	@echo "Cleaning..."
