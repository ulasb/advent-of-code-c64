.PHONY: all clean

PROJECT_NAME = day18
SOURCES = main.c
PROGRAM = $(PROJECT_NAME).prg
CC65_TARGET = c64

all: $(PROGRAM)

$(PROGRAM): $(SOURCES)
	cl65 -t $(CC65_TARGET) -O -o $(PROGRAM) $(SOURCES)

clean:
	rm -f $(PROGRAM) *.o
