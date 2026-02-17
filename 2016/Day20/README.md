# Advent of Code 2016 - Day 20 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 20, written in C and compiled with cc65.

## The Problem

**Day 20: Firewall Rules**

You'd like to set up a firewall to block IP addresses. You're given a list of blocked IP ranges (inclusive).
- **Part 1:** What is the lowest-valued IP that is not blocked?
- **Part 2:** How many IPs are allowed?

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day20.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day20.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Implementation Details

### Algorithm
1. **Range Storage:** Blocked IP ranges are stored as pairs of 32-bit unsigned integers (`unsigned long`).
2. **Sorting:** Ranges are sorted by their start value using `qsort`.
3. **Merging:** Overlapping or adjacent ranges are merged into a single contiguous block.
4. **Gap Finding:**
   - **Part 1:** The first gap between merged ranges (or between 0 and the first range) reveals the lowest allowed IP.
   - **Part 2:** Counting allowed IPs by subtracting blocked ranges from the total range (0 to 4294967295).

### Data Set
To comply with distribution rules, the full puzzle input is **not included**. The implementation uses a representative sample input for verification:
- **Sample Input:** `(0, 2), (4, 5), (1, 4), (10, 20), (15, 25), (30, 35)`
- **Expected Part 1:** `6`
- **Expected Part 2:** `4294967268`

## Files
- `day20.c`: Main source code.
- `Makefile`: Build script.
- `README.md`: This file.
