# Advent of Code 2015 - Day 3 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 3, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

Santa is delivering presents to houses based on directional instructions:
- `^` means move north (increase y)
- `v` means move south (decrease y)
- `>` means move east (increase x)
- `<` means move west (decrease x)

### Part 1
Starting at (0,0), following all directions, how many unique houses receive at least one present?

### Part 2
Santa and Robot-Santa alternate taking turns following directions, both starting at (0,0). How many unique houses receive at least one present?

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `santa_tracker.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases from the problem description:
- Part 1: 3 test cases covering house visit counting
- Part 2: 3 test cases covering Santa and Robot-Santa alternating movement

All tests should pass with "(PASS)" indicators.

## Test Cases Included

### Part 1 Tests
- `>` delivers presents to 2 houses: one at the starting location, and one to the east
- `^>v<` delivers presents to 4 houses in a square, including twice to the house at his starting/ending location
- `^v^v^v^v^v` delivers a bunch of presents to some very lucky children at only 2 houses

### Part 2 Tests
- `^v` delivers presents to 3 houses, because Santa goes north, and then Robo-Santa goes south
- `^>v<` now delivers presents to 3 houses, and Santa and Robo-Santa end up back where they started
- `^v^v^v^v^v` now delivers presents to 11 houses, with Santa going one direction and Robo-Santa going the other

## Files
- `santa_tracker.c` - Main C source code
- `Makefile` - Build configuration
- `santa_tracker.prg` - Compiled Commodore 64 program
