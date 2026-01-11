# Makefile for Advent of Code 2015 Day 22
# Target: C64

PROGRAM = day22
SOURCES = day22.c
TARGET = c64
CC = cl65
CFLAGS = -t $(TARGET) -O

all: $(PROGRAM).prg

$(PROGRAM).prg: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(PROGRAM).prg $(PROGRAM).o
