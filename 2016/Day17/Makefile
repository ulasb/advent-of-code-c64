# Makefile for Advent of Code 2016 Day 17 - C64 version
# Requires cc65 to be installed

TARGETS = day17.prg test_md5.prg

# cc65 tools
CC = cl65
CFLAGS = -t c64 -DFORCE_PETSCII_TO_ASCII
# Uncomment to force PETSCII->ASCII conversion if your toolchain doesn't
# automatically define __CC65__
# (the flag is enabled by default in this Makefile)

all: $(TARGETS)

day17.prg: day17.c md5.c
	$(CC) $(CFLAGS) -o day17.prg day17.c md5.c

test_md5.prg: test_md5.c md5.c
	$(CC) $(CFLAGS) -DMD5_DEBUG -o test_md5.prg test_md5.c md5.c

print_chars.prg: print_chars.c
	$(CC) $(CFLAGS) -o print_chars.prg print_chars.c

clean:
	rm -f $(TARGETS) *.o

run: day17.prg
	x64sc day17.prg

run_test: test_md5.prg
	x64sc test_md5.prg

.PHONY: all clean run run_test
