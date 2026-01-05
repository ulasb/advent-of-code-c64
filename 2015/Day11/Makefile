CC = cl65
CFLAGS = -O -t c64
TARGET = day11.prg
SOURCES = day11.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) day11.o
