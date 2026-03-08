# Advent of Code 2017 - Day 3 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 3, written in C and compiled with cc65.

## The Problem

**Day 3: Spiral Memory**

You are given a grid where each square is numbered in a spiral starting from square 1 at (0,0).
- **Part 1:** Calculate the Manhattan distance from the goal square to square 1. ✅ **Implemented**
- **Part 2:** Squares are filled by the sum of all adjacent squares (including diagonals). Find the first value written that is larger than the goal. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day3.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day3.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press any key to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic.

### Part 1 Tests
- **Input:** 1 -> **Expected:** 0 ✅
- **Input:** 12 -> **Expected:** 3 ✅
- **Input:** 23 -> **Expected:** 2 ✅
- **Input:** 1024 -> **Expected:** 31 ✅

### Part 2 Tests
- **Input:** Larger than 1 -> **Expected:** 2 ✅
- **Input:** Larger than 10 -> **Expected:** 11 ✅
- **Input:** Larger than 100 -> **Expected:** 122 ✅
- **Input:** Larger than 800 -> **Expected:** 806 ✅

## Technical Notes

### Part 1 Algorithm
- **Ring Identification:** Iterate using `side * side` to find the "ring" number and the maximum value in that ring.
- **Center Calculation:** Identify the four centers of the ring sides and find the minimum distance from the goal to any center.
- **Manhattan Distance:** The distance is `ring_number + min_distance_to_center`.

### Part 2 Algorithm
- **Grid Mapping:** Uses a 21x21 grid centered at (10, 10).
- **Spiral Traversal:** Start at center, move right, then turn left whenever the square to the left is empty.
- **Neighbor Summation:** For each new square, sum all 8 adjacent values.

### C64-Specific Challenges and Solutions

#### 1. Integer Precision
**Problem:** The goal value (277678) exceeds the capacity of a 16-bit integer (65535).
**Solution:** Use `long` (signed 32-bit) for goal and distance calculations, and `unsigned long` for Part 2 sums.

#### 2. Memory Constraints
**Problem:** Small hardware stack and 64KB RAM.
**Solution:** Large data structures like the `spiral_grid` are stored in the global segment (BSS) rather than on the stack.

#### 3. Platform-Specific I/O
**Problem:** Standard `printf` can be unreliable or require extra overhead on C64.
**Solution:** Use `conio.h` for direct character output, screen clearing, and color management.

#### 4. C89 Compliance
**Problem:** `cc65` requires strict C89 (variables declared at the beginning of blocks).
**Solution:** All function-level variables are declared at the top of their respective functions.

## Performance
- **Part 1:** Instantaneous.
- **Part 2:** Completes in under a second for the provided goal.

## Files
- `day3.c`: Main source code.
- `Makefile`: Build script.
- `README.md`: Documentation.
- `day3.prg`: Compiled C64 executable.

## Reference
Original Python implementation: [advent-of-code-python](https://github.com/ulasb/advent-of-code-python/tree/main/2017/day_03/)
