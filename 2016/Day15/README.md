# Advent of Code 2016 - Day 15 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 15, written in C and compiled with cc65.

## The Problem

**Day 15: Timing is Everything**

The capsule needs to fall through a series of rotating discs. Each disc has a certain number of positions and starts at a specific position at `time=0`.
- **Part 1:** Find the first time you can press the button so the capsule falls through all discs. ✅ **Implemented**
- **Part 2:** A new disc is added at the bottom. Find the new earliest time. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day15.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day15.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press any key to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description and input to verify the solution logic.

### Example Test
- **Input:** 
  - Disc #1 has 5 positions; at time=0, it is at position 4.
  - Disc #2 has 2 positions; at time=0, it is at position 1.
- **Expected:** 5 ✅

### Part 1
- **Input:** Real input from `input.txt`
- **Expected:** (Calculated by the program) ✅

### Part 2
- **Input:** Real input + additional disc (11 positions, starting at 0)
- **Expected:** (Calculated by the program) ✅

## Technical Notes

### Algorithm

**Chinese Remainder Theorem / Sieve Strategy:**
- For each disc `i` (index `index_i`, positions `num_i`, start `start_i`), we seek `time` such that:
  `(start_i + time + index_i) % num_i == 0`
- We iterate through discs, maintaining the current `time` and the `step` (LCM of positions of processed discs).
- For each new disc, we increment `time` by `step` until the condition is met.
- **Complexity:** O(N * Σ(num_i)) where N is the number of discs. Extremely efficient for this problem size.

### C64-Specific Challenges and Solutions

#### 1. Large Number Handling
**Problem:** The resulting `time` can exceed 65535 (16-bit).
**Solution:** Use `unsigned long` (32-bit) for `time` and `step` calculations. `cc65` supports 32-bit longs, which provide a range up to ~4.2 billion.

#### 2. Screen I/O
**Problem:** Standard `printf` behavior on C64 can be problematic with screen scrolling and persistence.
**Solution:** Use `conio.h` for direct screen access (`clrscr`, `cprintf`, `cgetc`). This ensures the results stay visible until the user acknowledges them.

#### 3. C89 Compliance
**Solution:** All variables are declared at the beginning of their respective blocks to comply with the `cc65` C89-style requirements.

## Files

- `day15.c` - Main source code
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day15.prg` - Compiled C64 executable (after running `make`)

## Building from Scratch

```bash
make clean  # Remove all build artifacts
make        # Rebuild day15.prg
make run    # Compile and run in VICE
```
