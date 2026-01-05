# Makefile for Advent of Code 2015 Day 5 - C64 version
# Requires cc65 to be installed

TARGET = day5.prg
SOURCE = day5.c

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
