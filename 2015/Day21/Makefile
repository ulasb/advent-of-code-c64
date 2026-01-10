CC = cl65
CFLAGS = -t c64 -O
TARGET = day21.prg
SOURCE = day21.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

.PHONY: clean
clean:
	rm -f $(TARGET) *.o
