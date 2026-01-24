# Makefile for Advent of Code 2016 Day 9 - C64 version
# Requires cc65 to be installed

TARGET = day9.prg
SOURCE = day9.c
OBJECT = day9.o

# cc65 tools
CC = cl65
CFLAGS = -t c64 -O

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECT)

$(OBJECT): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE)

clean:
	rm -f $(TARGET) $(OBJECT)

run: $(TARGET)
	x64 $(TARGET)

.PHONY: all clean run
