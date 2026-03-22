# Advent of Code 2017 - Day 8 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 8, written in C and compiled with cc65.

## The Problem

**Day 8: I Heard You Like Registers**

You receive a set of instructions to modify a series of registers. Each instruction consists of:
1.  A register to modify.
2.  Whether to increase (`inc`) or decrease (`dec`) it.
3.  The amount by which to modify it.
4.  A condition that must be met for the instruction to execute.

- **Part 1:** What is the largest value in any register after completing the instructions? ✅ **Implemented**
- **Part 2:** What is the highest value held in any register during the process? ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day8.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day8.prg` manually into your emulator.

The program will run the sample instructions from the problem description and display the register states and final results. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use a hardcoded sample case derived from the problem description to verify the solution logic.

### Sample Instructions
```
b inc 5 if a > 1
a inc 1 if b < 5
c dec -10 if a >= 1
c inc -20 if c == 10
```

### Expected Results
- **Part 1 (Max after):** 1 (a=1, b=0, c=-10) ✅
- **Part 2 (Max ever):** 10 (c reached 10) ✅

## Technical Notes

### C64-Specific Challenges and Solutions

#### 1. Integer Precision
**Problem:** Standard `int` on the 6502 (C64) is 16-bit (-32768 to 32767). Advent of Code values can easily exceed this range.

**Solution:** Use `long` (32-bit) for register values and intermediate calculations. This ensures compliance with the puzzle's requirements.

#### 2. Memory Management
**Problem:** The C64 has only 64KB of RAM. Dynamic memory allocation (`malloc`) is risky and often unnecessary for these puzzles.

**Solution:** Use static global arrays for registers.
```c
#define MAX_REGISTERS 16
Register registers[MAX_REGISTERS];
```

#### 3. Input Parsing
**Problem:** Reading from disk (`input.txt`) is slow and complex on C64.

**Solution:** For this proof of concept, instructions are stored as an array of strings and processed sequentially. This mirrors the logic of the Python solution while keeping the implementation focused on the core algorithm.

#### 4. Display Logic
**Problem:** Standard `printf` can be unreliable or require significant overhead on C64.

**Solution:** Use `conio.h` for direct, efficient screen output. This provides better control over colors and screen positioning.

## Files

- `day8.c` - Main source code
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day8.prg` - Compiled C64 executable

## Building from Scratch

```bash
make clean  # Remove build artifacts
make        # Rebuild day8.prg
make run    # Compile and run in x64
```

---

For the original Python implementation, see: https://github.com/ulasb/advent-of-code-python/tree/main/2017/day_08/
