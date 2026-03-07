# Advent of Code 2017 - Day 2 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 2, written in C and compiled with cc65.

## The Problem

**Day 2: Corruption Checksum**

As you walk through the door, a wall of computers greets you. Each row of the spreadsheet is filled with numbers.
- **Part 1:** Calculate the checksum, which is the sum of the difference between the largest value and the smallest value in each row. ✅ **Implemented**
- **Part 2:** Calculate the sum of each row's result, where the result is the quotient of the only two numbers in the row that evenly divide each other. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day2.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day2.prg` manually into your emulator.

The program will run automated tests using sample data and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic.

### Part 1 Tests
- **Input:** 
  - `5 1 9 5` (diff: 8)
  - `7 5 3` (diff: 4)
  - `2 4 6 8` (diff: 6)
- **Expected:** 18 ✅

### Part 2 Tests
- **Input:** 
  - `5 9 2 8` (8 / 2 = 4)
  - `9 4 7 3` (9 / 3 = 3)
  - `3 8 6 5` (6 / 3 = 2)
- **Expected:** 9 ✅

## Technical Notes

### Algorithm

**Checksum Calculation:**
- **Part 1:** For each row, track `min` and `max` values in a single pass.
- **Part 2:** For each row, use nested loops to find the pair of numbers `a, b` where `a % b == 0`.

### C64-Specific Considerations

#### 1. Screen Output
Used `conio.h` for reliable screen output and colors. This ensures the output is visible and stable on C64 hardware.

#### 2. Memory Management
Large arrays are avoided where possible. For the proof of concept, sample data is stored in localized arrays within the test function.

#### 3. C89 Compliance
The code follows strict C89 syntax as required by the `cc65` compiler (e.g., variable declarations at the top of scopes).

## Files

- `day2.c` - Main source code (Part 1 and 2 implementation)
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day2.prg` - Compiled C64 executable (after running `make`)

## Building from Scratch

```bash
make clean  # Remove all build artifacts
make        # Rebuild day2.prg
make run    # Compile and run in VICE
```
