CC = gcc
CFLAGS ?= -O2
CFLAGS += -Wall -Wextra
RM = rm -f

.PHONY: all clean

all: udp2nexus

udp2nexus: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) udp2nexus
