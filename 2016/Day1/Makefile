CC = cl65
CFLAGS = -t c64 -O
TARGET = day1.prg
SOURCES = day1.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) day1.o

run: $(TARGET)
	x64sc -autostart $(TARGET)

.PHONY: all clean run
