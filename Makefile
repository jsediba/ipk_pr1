CC=gcc
CFLAGS=-pedantic -Wall -Wextra -g -std=gnu99

.PHONY: clean

hinfosvc: server.c server.h
	$(CC) $(CFLAGS) $< -o $@ -lm

clean:
	rm hinfosvc