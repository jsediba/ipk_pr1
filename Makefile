CC=gcc
CFLAGS=-pedantic -Wall -Wextra -g -std=gnu99

.PHONY: clean zip

hinfosvc: server.c server.h
	$(CC) $(CFLAGS) $< -o $@ -lm

zip: Makefile server.c server.h README.md
	zip xsedib00.zip $^

clean:
	rm hinfosvc xsedib00.zip