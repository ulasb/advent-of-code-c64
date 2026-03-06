# Advent of Code 2017 - Day 1 (C64 Edition)

This is a Commodore 64 implementation of [Advent of Code 2017 Day 1](https://adventofcode.com/2017/day/1), written in C and compiled with [cc65](https://cc65.github.io/).

## The Problem

**Day 1: Inverse Captcha**

The goal is to find the sum of all digits that match the next digit in the list. The list is circular, so the digit after the last digit is the first digit in the list.

- **Part 1:** Check the *next* digit. ✅ **Implemented**
- **Part 2:** Check the digit *halfway around* the circular list. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day01.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day01.prg` manually into your emulator.

The program will run automated tests (derived from the problem description) and display results on screen. Press any key to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic.

### Part 1 Tests
- `1122` -> `3` ✅
- `1111` -> `4` ✅
- `1234` -> `0` ✅
- `91212129` -> `9` ✅

### Part 2 Tests
- `1212` -> `6` ✅
- `1221` -> `0` ✅
- `123425` -> `4` ✅
- `123123` -> `12` ✅
- `12131415` -> `4` ✅

## Technical Notes

### C64-Specific Challenges and Solutions

#### 1. Stack Limitations
**Problem:** The 6502 processor has a very small hardware stack (256 bytes), and `cc65` enforces strict limits on local variables.
**Solution:** Large buffers are moved to global scope (BSS segment).

#### 2. Screen I/O
**Problem:** Standard `stdio.h` functions can be slow or inconsistent on older hardware.
**Solution:** We use `conio.h` for direct, high-performance screen access (`cprintf`, `clrscr`, etc.), which is the standard practice for C64 development with `cc65`.

#### 3. Memory Constraints
**Problem:** The C64 only has 64 KB of RAM.
**Solution:** The algorithm is designed to process the input string in-place with minimal additional memory allocation.

### Performance
The implementation is highly efficient. Even on a 1MHz 6502, it calculates the results for the test cases near-instantaneously.

## Files
- `day01.c` - Main source code (Parts 1 & 2 implementation)
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day01.prg` - Compiled C64 executable (after running `make`)

## Building from Scratch

```bash
make clean  # Remove all build artifacts
make        # Rebuild day01.prg
make run    # Compile and run in VICE
```
