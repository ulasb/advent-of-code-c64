CC = cl65
CFLAGS = -t c64 -O
TARGET = day2.prg
SOURCES = day2.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) day2.o

run: $(TARGET)
	x64sc -autostart $(TARGET)

.PHONY: all clean run
