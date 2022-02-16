CC=gcc
CFLAGS=-lm -pedantic -Wall -Wextra -g

.PHONY: clean

hinfosvc: server.c server.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm hinfosvc