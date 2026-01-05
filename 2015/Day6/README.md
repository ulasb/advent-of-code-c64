# Advent of Code 2015 - Day 6 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 6, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

Because your neighbors keep defeating you in the holiday house decorating contest year after year, you've decided to deploy one million lights in a 1000x1000 grid.

### Part 1
The instructions include turning on, turning off, and toggling various ranges of lights. After following the instructions, how many lights are lit?

### Part 2
The light grid you bought actually has individual brightness controls; each light can have a brightness of zero or more. The lights all start at zero. The phrase "turn on" actually means that you should increase the brightness of those lights by 1, the phrase "turn off" actually means that you should decrease the brightness of those lights by 1 (to a minimum of zero), and "toggle" actually means that you should increase the brightness of those lights by 2.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day6.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases specified for validation:
- Part 1: Tests the three specified scenarios for counting lights
- Part 2: Tests the two specified scenarios for brightness calculation

All tests should pass with "(PASS)" indicators.

## Test Cases Included

### Part 1 Tests (50×50 Grid)
- `turn on 0,0 through 49,49` turns on all lights in the 50×50 grid (2,500 lights)
- `toggle 0,0 through 49,0` toggles the first row, turning off 50 lights (2,450 lights remain)
- `turn off 24,24 through 25,25` turns off a 2×2 square in the middle (4 lights) (2,446 lights remain)

Expected final result: 2,446 lights on

### Part 2 Tests (50×50 Grid)
- `turn on 0,0 through 0,0` increases brightness of one light by 1 (total brightness: 1)
- `toggle 0,0 through 49,49` increases brightness of all 2,500 lights by 2 each (total brightness: 1 + 5,000 = 5,001)

Expected final result: 5,001 total brightness

### Full 1000×1000 Scale
The algorithms are identical to the full problem - the 50×50 tests validate the same logic that would work on a 1000×1000 grid. The C64 simply lacks the memory for the full-scale problem.

## Technical Notes

Due to C64 memory constraints (approximately 64KB RAM), the program cannot store the full 1000x1000 grid in memory. Instead, it demonstrates the proper grid-based algorithms using a 50x50 grid that can run on actual C64 hardware.

### Memory Usage
- **50×50 grid for Part 1**: 2,500 bytes (~2.5KB) for on/off states (`char`)
- **50×50 grid for Part 2**: 10,000 bytes (~10KB) for brightness levels (`unsigned int`)
- **Total memory usage**: ~12.5KB for both grids, well within C64's ~40KB available program memory

### Data Type Choices
- **Part 1**: `char` (1 byte) sufficient for binary on/off states
- **Part 2**: `unsigned int` (2 bytes) prevents overflow from repeated toggle operations. The Advent of Code problem doesn't specify brightness limits, so larger types ensure correctness.

### Why Not 1000×1000?
A full 1000×1000 grid would require:
- **Part 1**: 1MB (1,000,000 bytes) - impossible in C64 RAM
- **Part 2**: 4MB (4,000,000 bytes) with integer brightness levels

While theoretically possible to store this on C64 floppy disks, it would be extremely slow (10+ minutes per operation) and impractical for demonstration.

### Algorithm Correctness

The program implements the proper grid manipulation algorithms that would work for any sequence of commands, not just the specific test cases. This ensures the solution is general and correct.

### Performance Optimizations

The program implements several optimizations for C64 performance:

- **Enum-based commands**: Command strings are parsed once into efficient enum values (`CMD_TURN_ON`, `CMD_TURN_OFF`, `CMD_TOGGLE`) instead of storing strings. This avoids 2,500+ `strcmp()` calls per command in the grid processing loops, using fast `switch` statements instead.

- **Incremental counting**: Both light counts (Part 1) and brightness totals (Part 2) are maintained incrementally during grid processing instead of rescanning the entire 2,500-cell grid after each command. This eliminates expensive grid-wide counting operations.

- **Proper data types**: Uses `unsigned int` for brightness values to prevent overflow from repeated operations, ensuring correctness even with unlimited toggle commands.

- **Custom parsing functions**: Manual character-by-character parsing instead of `strtok()` and `sscanf()` for C64 compatibility.

- **Simple atoi**: Custom integer parsing instead of standard `atoi()` for reliability and minimal overhead.

These optimizations ensure both correctness and good performance on the constrained C64 platform.

## Files
- `day6.c` - Main C source code
- `Makefile` - Build configuration
