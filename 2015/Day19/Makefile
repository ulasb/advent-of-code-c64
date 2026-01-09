# Makefile for Advent of Code 2015 Day 19 - C64 version
# Requires cc65 to be installed

TARGET = day19.prg
SOURCE = day19.c

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
