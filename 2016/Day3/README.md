# Advent of Code 2016 - Day 3 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 3, written in C and compiled with cc65.

## The Problem

**Day 3: Squares With Three Sides**

The North Pole design department needs to know which sets of side lengths can form valid triangles.

- **Part 1:** Each row in the input lists three side lengths. A triangle is valid if the sum of any two sides is greater than the third side. ✅ **Implemented**
- **Part 2:** The input interpretation changes. Now, triangles are formed by three consecutive numbers in the same column. ✅ **Implemented**

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

The program will run automated tests using hardcoded sample data and display results on screen. Press Enter to exit when complete.

## Sample Data Included

Since no input file is used, the program uses embedded test cases to verify the logic.

### Part 1 Test Cases:
- `(5, 10, 25)` -> Invalid
- `(10, 10, 10)` -> Valid
- `(2, 3, 4)` -> Valid
- `(1, 2, 3)` -> Invalid

### Part 2 Test Cases:
Uses two blocks of 3 rows (6 total). Triangles are formed by taking 3 consecutive numbers from the **same column**.

**Input Matrix:**
```
Row 1: {101, 301, 501}
Row 2: {102, 302, 502}
Row 3: {103, 303, 503}
Row 4: {201, 401, 601}
Row 5: {202, 402, 602}
Row 6: {203, 403, 603}
```

**Triangles Formed:**
- From Column 1: `(101, 102, 103)` and `(201, 202, 203)`
- From Column 2: `(301, 302, 303)` and `(401, 402, 403)`
- From Column 3: `(501, 502, 503)` and `(601, 602, 603)`

All 6 triangles in this test set are valid.

## Technical Notes

### Algorithm

**Triangle Equality Check:**
The core logic `is_valid_triangle(a, b, c)` checks the triangle inequality theorem:
```c
if (a + b > c && a + c > b && b + c > a) return 1;
```
For Part 1, it iterates through rows. For Part 2, it looks ahead across 3 rows and iterates through columns.

### C64-Specific Challenges and Solutions

Similar to previous projects, we adhere to C64 constraints:

#### 1. Integer Sizes
C64 `int` is 16-bit (signed -32768 to 32767). Since the puzzle side lengths are typically small 3-digit numbers, `int` is sufficient.

#### 2. Screen Output (conio.h)
Standard `printf` can be flaky on C64. We use `conio.h` for direct, fast, and stable screen access:
- `bgcolor()`, `bordercolor()`, `textcolor()` for aesthetics.
- `clrscr()` to clear the screen.
- `cprintf()` for reliability.

#### 3. Program Persistence
The C64 returns to BASIC immediately after `main()` finish. We use `cgetc()` to pause the program so the results can be read before exit.

## Files

- `day3.c` - Main source code (Part 1 and Part 2 implementation)
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day3.prg` - Compiled C64 executable

## Building and Cleaning

```bash
make clean  # Remove build artifacts
make        # Build day3.prg
```
