# Advent of Code 2016 - Day 6 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 6, written in C and compiled with cc65.

## The Problem

**Day 6: Signals and Noise**

Something is interfering with your communications system. You receive a series of messages, but each character is being slightly corrupted.

- **Part 1:** Find the most frequently occurring character at each position in the messages to reconstruct the original message.
- **Part 2:** Find the least frequently occurring character at each position in the messages.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day6.prg`.

### Testing
To run it in VICE:
```bash
make run
```

The program includes the sample input from the problem description and verifies the results for both Part 1 and Part 2.

## Technical Notes

### Data Structures
On the C64, memory is limited, but this problem is small enough to handle efficiently:
- A frequency table of `[COLUMN_COUNT][26]` is used to store character counts.
- For the sample input (6 columns), this table takes `6 * 26 * 1 = 156` bytes (using `unsigned char` for counts if the input is small, or `unsigned int` for larger inputs).
- The input is embedded as a static array of strings.

### C64-Specific Tweaks
- **conio.h UI:** Uses the fast `conio.h` library for screen output.
- **Static Allocation:** Avoids dynamic memory allocation to stay within the limited 64KB heap/stack constraints.

## Files

- `day6.c` - Main source code with frequency analysis logic.
- `Makefile` - Build script for cc65.
- `README.md` - This file.
- `day6.prg` - Compiled C64 executable.

## Comparison with Python
The Python implementation uses `collections.Counter`, which is extremely convenient but carries high overhead. The C version uses a simple 2D array and manual loops, which is much faster and suited for an 8-bit architecture.

Python solution:
https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_6/
