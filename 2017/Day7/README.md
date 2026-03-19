# Advent of Code 2017 - Day 7 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 7, written in C and compiled with cc65.

## The Problem

**Day 7: Recursive Circus**

Wandering further through the circuits of the computer, you come upon a tower of programs that have gotten themselves into a bit of trouble.
- **Part 1:** Find the name of the bottom program. ✅ **Implemented**
- **Part 2:** Find the corrected weight of the program causing the tower to be imbalanced. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day07.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day07.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic. This acts as a proof of concept avoiding complex string parsing or memory allocation that would cause instability.

### Part 1 Test
- **Input:** Sample tree structure with 13 programs
  - **Expected:** `tknk` ✅

### Part 2 Test
- **Input:** Same sample tree
  - **Expected:** `60` ✅

## Technical Notes

### C64-Specific Optimizations
- **Static Memory:** Used pre-allocated structure arrays to map the tree instead of dynamic parsing.
- **Recursion Depth:** The stack string length and total recursion depth are bounded tightly to fit into the Commodore 64's extremely limited 256 byte hardware stack.
- **No Dynamic Allocations:** Avoiding heap usage (`malloc`/`free`) helps with performance and avoids crash issues with `cc65`.

## Files

- `day07.c` - Main source code (Part 1 and 2 implemented)
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day07.prg` - Compiled C64 executable (after running `make`)
