# Makefile for Advent of Code 2015 Day 25 - C64 version
# Requires cc65 to be installed

TARGET = day25.prg
SOURCES = day25.c algorithm.c
TEST_TARGET = test_algorithm
TEST_SOURCES = test_algorithm.c algorithm.c

# cc65 tools
CC = cl65
CFLAGS = -t c64 -O

# Host compiler for tests
HOST_CC = gcc
HOST_CFLAGS = -O2

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

$(TEST_TARGET): $(TEST_SOURCES)
	$(HOST_CC) $(HOST_CFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES)

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o

run: $(TARGET)
	x64 $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean run test
