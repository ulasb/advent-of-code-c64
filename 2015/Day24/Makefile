# Makefile for Advent of Code 2015 Day 24 C64

PROGRAM = day24
SOURCES = $(PROGRAM).c
CC = cl65
CFLAGS = -t c64 -O -o $(PROGRAM).prg

all: $(PROGRAM).prg

$(PROGRAM).prg: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm -f $(PROGRAM).prg $(PROGRAM).o

run: $(PROGRAM).prg
	@echo "To run, use a C64 emulator like VICE:"
	@echo "x64sc -autostart $(PROGRAM).prg"

.PHONY: all clean run
