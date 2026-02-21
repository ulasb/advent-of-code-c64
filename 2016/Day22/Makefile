CC = cc65
AS = ca65
LD = ld65
CFLAGS = -t c64 -O
LDFLAGS = -t c64

SOURCES = day22.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = day22.prg

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ c64.lib

%.o: %.c
	$(CC) $(CFLAGS) -o $(@:.o=.s) $<
	$(AS) -t c64 -o $@ $(@:.o=.s)

clean:
	rm -f $(OBJECTS) $(SOURCES:.c=.s) $(EXECUTABLE)
