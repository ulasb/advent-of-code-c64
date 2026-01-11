# Makefile for Advent of Code 2015 - Day 23 (C64 Edition)

CC = cc65
AS = ca65
LD = ld65
CL = cl65

TARGET = day23.prg
SOURCES = day23.c

# Flags for cl65: 
# -t c64: target system C64
# -O: optimize code
# -o: output file
CFLAGS = -t c64 -O

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CL) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) day23.o

run: $(TARGET)
	x64 $(TARGET)

.PHONY: all clean run
