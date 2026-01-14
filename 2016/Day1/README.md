# Advent of Code 2016 - Day 1: No Time for a Taxicab (C64 Edition)

This project implements the solution for Advent of Code 2016, Day 1, optimized to run on a Commodore 64.

## The Problem
The goal is to navigate a grid based on instructions like `R2, L3`. 
- **Part 1**: Find the Manhattan distance of the final destination from the starting point.
- **Part 2**: Find the Manhattan distance of the first location visited twice.

## Project Structure
- `day1.c`: The main C source code.
- `Makefile`: Build script for the `cc65` compiler.
- `README.md`: Project documentation.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite.
- A Commodore 64 emulator like [VICE](https://vice-emu.sourceforge.io/).

### Compilation
To compile the program, run:
```bash
make
```
This will generate `day1.prg`.

### Execution
You can run the program in VICE using:
```bash
make run
```
Or manually load `day1.prg` into your favorite C64 emulator.

## Testing
The program includes several test cases from the official Advent of Code problem description:
1. `R2, L3` -> Distance: 5
2. `R2, R2, R2` -> Distance: 2
3. `R5, L5, R5, R3` -> Distance: 12
4. `R8, R4, R4, R8` -> Part 2 HQ Distance: 4

The output is displayed directly on the C64 screen using `conio.h`.

## Optimizations for C64
- **Memory Management**: Uses a static array of structs to track visited locations. Includes explicit bounds checking to halt the program with an error message if the `MAX_VISITED` limit is reached, ensuring incorrect results are never produced for Part 2.
- **Safe String Handling**: Uses `snprintf` instead of `strcpy` for processing input strings to prevent potential buffer overflows.
- **Efficient Searching**: Employs a simple linear search through the visited list. While $O(N^2)$ for the whole process, the list size is small enough (max 1024 points) that it runs quickly on a 1 MHz processor.
- **Screen Output**: Uses the `conio.h` library for fast, direct-to-screen text output.
