# Advent of Code 2016 - Day 13 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 13: "A Maze of Twisty Little Cubicles", written in C and compiled with cc65.

## The Problem

**Day 13: A Maze of Twisty Little Cubicles**

You find yourself in a building where each coordinate (x, y) is either a wall or an open space, determined by a mathematical formula and a "favorite number" (puzzle input).

- **Part 1:** Find the shortest path (minimum moves) from (1, 1) to (31, 39). ✅ **Implemented**
- **Part 2:** How many unique locations (including your starting position) can you reach in at most 50 steps? ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day13.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day13.prg` manually into your emulator.

The program will run automated tests and display results on screen, followed by the solutions for Part 1 and Part 2. Press Enter to exit when complete.

## Test Cases Included

- **Part 1 Test:**
  - **Input:** Favorite number `10`, Target `(7, 4)`
  - **Expected:** 11 steps ✅

## Technical Notes

### Algorithm

**Breadth-First Search (BFS):**
- Used to find the shortest path in an unweighted grid.
- A `visited` bitmask tracks explored coordinates to avoid cycles and redundant work.
- A circular queue stores the frontier of the search.
- The BFS is implemented in a "level-by-level" manner to track distance without needing a full `distance` array for every coordinate.

### C64-Specific Optimizations

#### 1. Memory Management
- **Visited Bitmask:** A 64x64 grid is mapped to a bitmask of 512 bytes (`64*64/8`). This saves significant memory compared to a byte-per-cell approach.
- **Global Buffers:** Large structures like the queue and bitmask are placed in the global BSS segment to avoid overflowing the limited 6502 stack.
- **Circular Queue:** Used for BFS to efficiently manage memory without dynamic allocation.

#### 2. Performance
- **Popcount:** The `is_wall` function calculates the number of set bits in a 16-bit integer. This is done using a simple loop, which is efficient enough for this scale.
- **Formula Optimization:** The formula `x*x + 3*x + 2*x*y + y + y*y` is simplified to `(x+y)*(x+y) + 3*x + y` to reduce multiplications.

#### 3. Constraints
- **Map Size:** The implementation uses a 64x64 mapped region. This is sufficient for both Part 1 (target at 31,39) and Part 2 (radius of 50 steps).
- **Queue Size:** A 512-entry circular queue is used, which is ample for the expected breadth of the search on a 64x64 grid.

## Files

- `day13.c` - Main source code (Parts 1 & 2)
- `Makefile` - Build script for cc65
- `README.md` - This file

*Note: `day13.prg` is a compiled build artifact and should not be committed to source control.*

---
For more solutions and information, please visit the [main project repository](https://github.com/ulasb/advent-of-code-c64).
