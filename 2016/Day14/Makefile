# Makefile for Advent of Code 2016 Day 14 - C64 version
# Requires cc65 to be installed

TARGET = day14.prg
SOURCES = day14.c md5.c
OBJECTS = day14.o md5.o

# cc65 tools
CC = cl65
CFLAGS = -t c64 -O

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	x64sc $(TARGET)

.PHONY: all clean run
