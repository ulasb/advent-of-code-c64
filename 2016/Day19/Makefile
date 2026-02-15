TARGET = day19.prg
C_SOURCES = day19.c
CC = cc65
CL = cl65
AS = ca65
LD = ld65

all: $(TARGET)

$(TARGET): $(C_SOURCES)
	$(CL) -t c64 -O -o $@ $^

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	x64 $(TARGET)
