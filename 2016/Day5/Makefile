# Makefile for Advent of Code 2016 Day 5 - C64 version
# Requires cc65 to be installed

TARGET = day5.prg
SOURCES = day5.c md5.c
OBJECTS = day5.o md5.o

# cc65 tools
CC = cl65
CFLAGS = -t c64 -O -Oi

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	x64sc $(TARGET)

.PHONY: all clean run
