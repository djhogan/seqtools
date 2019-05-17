.PHONY: all
all: fsplit kmerext

fsplit: fsplit.c
	$(CC) -Wall -Wextra -Wconversion -o $@ $<

kmerext: kmerext.c
	$(CC) -Wall -Wextra -Wconversion -o $@ $<
