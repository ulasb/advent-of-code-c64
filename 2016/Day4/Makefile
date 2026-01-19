CC=cl65
CFLAGS=-O -t c64

.PHONY: all clean

all: day4.prg

day4.prg: day4.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f day4.prg *.o
