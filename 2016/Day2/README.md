# Advent of Code 2016 - Day 2: Bathroom Security (C64 Edition)

This project implements the solution for Advent of Code 2016, Day 2, optimized to run on a Commodore 64.

## The Problem
The goal is to determine the bathroom code by following a series of movement instructions (U, D, L, R) on a keypad.
- **Part 1**: Use a standard 1-9 keypad.
- **Part 2**: Use a custom diamond-shaped keypad with digits 1-9 and letters A-D.

## Project Structure
- `day2.c`: The main C source code.
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
This will generate `day2.prg`.

### Execution
You can run the program in VICE using:
```bash
make run
```
Or manually load `day2.prg` into your favorite C64 emulator.

## Testing
The program includes the full input from the user's puzzle. 
- Part 1 starts at '5' on a 3x3 grid.
- Part 2 starts at '5' on a diamond grid.

The output is displayed directly on the C64 screen using `conio.h`.

## Optimizations for C64
- **Input Handling**: The puzzle input is stored as an array of static constant strings to avoid file I/O overhead and minimize memory usage during processing.
- **Keypad Logic**: Uses simple coordinate bounding and lookup tables for both Part 1 and Part 2, minimizing computational complexity on the 1 MHz 6502 processor.
- **Screen Output**: Uses the `conio.h` library for fast, color-coded text output.
