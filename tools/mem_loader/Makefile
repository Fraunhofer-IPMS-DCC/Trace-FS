HOST_ARCH := $(shell uname -m)

ifeq ($(HOST_ARCH),aarch64)
# Building on the target (e.g. the Kria KR260 itself).
CC       = gcc
LDFLAGS  =
else
# Building on an x86_64 (or other non-aarch64) dev host: cross-compile
# for aarch64 Linux and link statically so glibc versions on the target
# don't matter.
CC       = aarch64-linux-gnu-gcc
LDFLAGS  = -static
endif

CFLAGS = -Wall -Wextra -Werror -O2
RM = rm -f

.PHONY: all clean

all: mem_loader mem_dump

mem_loader: mem_loader.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

mem_dump: mem_dump.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	$(RM) mem_loader
	$(RM) mem_dump
