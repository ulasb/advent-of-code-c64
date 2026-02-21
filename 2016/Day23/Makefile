# Makefile for Advent of Code 2016 Day 23 - C64 version
# Requires cc65 to be installed

TARGET = day23.prg
SOURCE = day23.c

# cc65 tools
CC = cl65
CFLAGS = -t c64 -O

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	x64 $(TARGET)

.PHONY: all clean run
