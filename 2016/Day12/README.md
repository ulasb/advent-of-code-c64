# Advent of Code 2016 - Day 12 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 12, written in C and compiled with cc65.

## The Problem

**Day 12: Leonardo's Monorail**

You find yourself in a large building where you need to interpret some assembly language ("Assembunny").
- **Part 1:** Execute the instructions and find the value in register `a`. ✅ **Implemented**
- **Part 2:** Set register `c` to 1, then execute the instructions and find the value in register `a`. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day12.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day12.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press any key to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description and original unit tests to verify the solution logic.

### Included Tests
- **Test Example:** Verifies the small program provided in the problem description.
- **Test cpy register:** Verifies copying from one register to another.
- **Test jnz zero:** Verifies jump logic when the condition is zero.
- **Test jnz nonzero:** Verifies jump logic when the condition is non-zero.
- **Test peephole add:** Verifies the addition loop optimization.

## Technical Notes

### Algorithm

**Interpreter Strategy:**
- Instructions are pre-parsed into a struct for efficient execution.
- Registers are stored as `long` (32-bit) to handle large values.
- A peephole optimization is implemented to speed up addition loops (e.g., `inc a`, `dec b`, `jnz b -2` becomes `a += b; b = 0`).

### C64-Specific Challenges and Solutions

#### 1. Performance
**Problem:** The C64's 1 MHz processor is too slow to run the unoptimized interpreter for Part 2 in a reasonable time.

**Solution:** Implemented peephole optimization for addition loops. This pattern is common in the puzzle input and reduces millions of instructions to a single addition.

#### 2. Output Reliability
**Problem:** Standard `stdio.h` functions can be unreliable or inconsistent on C64.

**Solution:** Use `conio.h` for direct screen access and consistent output.

#### 3. C89 Compliance
**Problem:** `cc65` requires strict C89 compliance.

**Solution:** Declare all variables at the beginning of their scope.

## Files

- `day12.c` - Main source code
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day12.prg` - Compiled C64 executable (after running `make`)

## Building from Scratch

If you need to rebuild everything:
```bash
make clean  # Remove all build artifacts
make        # Rebuild day12.prg
make run    # Compile and run in VICE
```
